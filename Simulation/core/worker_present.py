from core.constants import *
from utils.modbus_utils import FloatModbusClient
from utils.redis_client import SignalClient
import time
import signal
import sys
import random
import socket

def handle_shutdown(signum, frame):
    if modbus_client:
        modbus_client.close()
    if redis_client:
        redis_client.close()
    sys.exit(0)

signal.signal(signal.SIGTERM, handle_shutdown)
signal.signal(signal.SIGINT, handle_shutdown)

# Stagger startup based on IP to avoid overwhelming Redis
ip = socket.gethostbyname(socket.gethostname())
last_octet = int(ip.split('.')[-1])
stagger_delay = (last_octet % 20) * 0.5
print(f"Staggering startup by {stagger_delay} seconds...")
time.sleep(stagger_delay)


print("Connecting to OpenPLC...")
modbus_client = None
while modbus_client is None:
    try:
        modbus_client = FloatModbusClient(host=OPENPLC_HOST, port=OPENPLC_PORT, auto_open=True, auto_close=False)
    except Exception as e:
        print(f"Failed to connect to OpenPLC: {e}. Retrying in 1 second..")
        time.sleep(1 + random.uniform(-0.2, 0.2))
print("Connected to OpenPLC successfully.")

print("Connecting to Redis server...")
redis_client = None
while redis_client is None:
    try:
        redis_client = SignalClient(host=REDIS_HOST, port=REDIS_PORT)
    except Exception as e:
        print(f"Failed to connect to Redis server: {e}. Retrying in 1 second..")
        time.sleep(1 + random.uniform(-0.2, 0.2))
print("Connected to Redis server successfully.")

while not redis_client.get_value("shutdown"):
    reset = redis_client.get_value("reset")
    if reset:
        time.sleep(1)
        continue
    
    worker_present = redis_client.get("worker_present")
    if worker_present.get_value() != modbus_client.read_signal(worker_present):
        worker_present.set_value(modbus_client.read_signal(worker_present))
        redis_client.set_value("worker_present", worker_present.get_value())
    modbus_client.write_signal(worker_present)
    time.sleep(1 + random.uniform(-0.2, 0.2))