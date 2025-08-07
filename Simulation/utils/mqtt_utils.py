import json
import time
import threading
import paho.mqtt.client as mqtt
from paho import mqtt as mqtt_consts

BROKER = "ualr-core-f8adc79d.a02.usw2.aws.hivemq.cloud"
PORT = 8883
TOPIC_PREFIX = "BUILD_ID"
USERNAME = "cyberarena"
PASSWORD = "eyJhbGciOiJSUzI1NiIsImtpZCI6InhKRGZ5d0NJNEFuZVBpMzExMWh1dndVSTVFWHc0c09vbVZZbVlCUDItZVEiLCJ0eXAiOiJKV1QifQ.eyJleHAiOjE3NTUxODM2MzYsImlhdCI6MTc1Mzk3NDAzNiwic3ViIjoiaGZ5aWZkYmNuciJ9.YSQGM1tXRVqI4CPGyKlvytlvXRDyMXRnYt9_eaAWta4k2OLd5sHChsYOJ9dpUpVE8LXscyq0KKjtlz1o9_0EXMEV7P8A4alZcoJDzwS2wnz00DkNjWlO2zFBpyCJrrg4GvAwJtEfKVCfgl7sMPDUoPBxi6gAgRJmrvdIA2ajeRNZ35smp-4yDAl-ZxsewWvGGhydcBbJ-9vgGrBJHCfhgY3hmSsm4TYF3k6wllecmDefGV4UCHOAVEtTufsbH9zSIg_hWYyKTO2q0NCjH-0Agb8XsSx27ZgD2oOl4rDoUUbFGzwv6PSZpTfg6_Qj0pzNUBCPtWUTARwoOxiZXBO8Hw"

class MqttClient:
    def __init__(self, signals, modbus_client):
        self.client = mqtt.Client(client_id="", protocol=mqtt.MQTTv5)
        self.client.tls_set(tls_version=mqtt_consts.client.ssl.PROTOCOL_TLS)
        self.client.username_pw_set(USERNAME, PASSWORD)
        self.signals = signals
        self.modbus_client = modbus_client
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

    def connect(self):
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
            signal_name = msg.topic.split("/")[-1]
            for signal in self.signals:
                if signal.get_name() == signal_name:
                    value = payload['value']
                    if isinstance(signal.get_value(), bool):
                        self.modbus_client.write_single_coil(signal.get_address(), value)
                    elif isinstance(signal.get_value(), int):
                        self.modbus_client.write_single_register(signal.get_address(), value)
                    elif isinstance(signal.get_value(), float):
                        self.modbus_client.write_float(signal.get_address(), value)
                    break
        except Exception as e:
            print(f"Failed to parse message: {e}")

    def publish_signal(self, signal_name, signal):
        topic = f"{TOPIC_PREFIX}/{signal_name}"
        payload = {
            "name": signal_name,
            "dataType": type(signal.get_value()).__name__,
            "value": signal.get_value(),
            "timestamp": int(time.time()),
        }
        self.client.publish(topic, json.dumps(payload))