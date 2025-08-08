import json
import time
import threading
import socket
import os
import sys
import paho.mqtt.client as mqtt
from paho import mqtt as mqtt_consts
from core import constants

TOPIC_PREFIX = os.environ.get('BUILD_ID', 'BUILD_ID_NOT_SET')

class MqttClient:
    def __init__(self, signals, modbus_client):
        self.client = mqtt.Client(client_id="", protocol=mqtt.MQTTv5)
        self.client.tls_set(tls_version=mqtt_consts.client.ssl.PROTOCOL_TLS)
        self.signals = signals
        self.modbus_client = modbus_client
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        self.hostname = socket.gethostname()
    
    def wait_for_token(timeout=300):
        print("Waiting for JWT_TOKEN environment variable to be set...")
        waited = 0
        while waited < timeout:
            token = os.environ.get('JWT_TOKEN', 'TOKEN_NOT_SET')
            if token and token != 'TOKEN_NOT_SET':
                return token
            time.sleep(1)
            waited += 1
        print("ERROR: JWT_TOKEN environment variable not set after waiting. Exiting.")
        sys.exit(1)
    
    def connect(self):
        PASSWORD = self.wait_for_token(timeout=60)
        self.client.username_pw_set(constants.USERNAME, PASSWORD)
        self.client.connect(constants.BROKER, constants.PORT, keepalive=60)
        thread = threading.Thread(target=self.client.loop_forever)
        thread.daemon = True
        thread.start()

    def on_connect(self, client, userdata, flags, rc, properties=None):
        for signal in self.signals:
            topic = f"{TOPIC_PREFIX}/{signal.get_name()}"
            client.subscribe(topic)

    def on_message(self, client, userdata, msg):
        try:
            payload = json.loads(msg.payload.decode())
            if payload.get('source') == self.hostname:
                return
            print(f"Received message on topic {msg.topic}: {payload}")
            signal_name = msg.topic.split("/")[-1]
            for signal in self.signals:
                if signal.get_name() == signal_name:
                    value = payload['value']
                    data_type = payload.get('dataType', '').lower()
                    # Cast value based on dataType in payload
                    if 'bool' in data_type:
                        if isinstance(value, str):
                            value = value.lower() == 'true'
                        else:
                            value = bool(value)
                        self.modbus_client.write_single_coil(signal.get_address(), value)
                    elif 'int' in data_type:
                        if isinstance(value, str):
                            try:
                                value = int(value)
                            except ValueError:
                                print(f"Invalid integer value '{value}' for signal {signal_name}")
                                break
                        self.modbus_client.write_single_register(signal.get_address(), value)
                    elif 'float' in data_type:
                        if isinstance(value, str):
                            try:
                                value = float(value)
                                print(f"Converted string to float: {value} for signal {signal_name}")
                            except ValueError:
                                print(f"Invalid float value '{value}' for signal {signal_name}")
                                break
                        self.modbus_client.write_float(signal.get_address(), value)
                    else:
                        print(f"Unknown dataType '{data_type}' for signal {signal_name}")
                    break
        except Exception as e:
            print(f"Failed to parse message: {e}")

    def publish_signal(self, signal_name, signal):
        if signal.get_value() != signal.latest_published_value or signal.latest_published_value is None:
            topic = f"{TOPIC_PREFIX}/{signal_name}"
            payload = {
                "name": signal_name,
                "dataType": type(signal.get_value()).__name__,
                "value": signal.get_value(),
                "timestamp": int(time.time()),
                "source": self.hostname
            }
            self.client.publish(topic, json.dumps(payload))
            signal.latest_published_value = signal.get_value()