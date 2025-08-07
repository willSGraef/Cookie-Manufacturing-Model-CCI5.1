import json
import time
import threading
import socket
import os
import paho.mqtt.client as mqtt
from paho import mqtt as mqtt_consts

BROKER = "ualr-core-f8adc79d.a02.usw2.aws.hivemq.cloud"
PORT = 8883
TOPIC_PREFIX = os.environ.get('BUILD_ID', 'DEFAULT_TOPIC')
USERNAME = "cyberarena"
PASSWORD = os.environ.get('JWT_TOKEN', 'TOKEN_NOT_SET')

class MqttClient:
    def __init__(self, signals, modbus_client):
        self.client = mqtt.Client(client_id="", protocol=mqtt.MQTTv5)
        self.client.tls_set(tls_version=mqtt_consts.client.ssl.PROTOCOL_TLS)
        self.client.username_pw_set(USERNAME, PASSWORD)
        self.signals = signals
        self.modbus_client = modbus_client
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        self.hostname = socket.gethostname()

    def connect(self):
        if PASSWORD == 'TOKEN_NOT_SET':
            print("ERROR: JWT_TOKEN environment variable not set. Exiting.")
            import sys
            sys.exit(1)
        self.client.connect(BROKER, PORT, keepalive=60)
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
                return # Ignore messages from the simulation itself
            signal_name = msg.topic.split("/")[-1]
            for signal in self.signals:
                if signal.get_name() == signal_name:
                    value = payload['value']
                    if type(signal.get_value()) == bool:
                        self.modbus_client.write_single_coil(signal.get_address(), bool(value))
                    elif type(signal.get_value()) == int:
                        self.modbus_client.write_single_register(signal.get_address(), int(value))
                    elif type(signal.get_value()) == float:
                        self.modbus_client.write_float(signal.get_address(), float(value))
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