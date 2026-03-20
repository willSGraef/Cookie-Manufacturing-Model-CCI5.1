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


# Establish modbus client connection
print("Connecting to OpenPLC...")
modbus_client = None
while modbus_client is None:
    try:
        modbus_client = FloatModbusClient(host = OPENPLC_HOST, port = OPENPLC_PORT, auto_open= True, auto_close= False)
    except Exception as e:
        print(f"Failed to connect to OpenPLC: {e}. Retrying in 1 second..")
        time.sleep(1 + random.uniform(-0.2, 0.2))
print("Connected to OpenPLC successfully.")

# Establish redis client connection
print("Connecting to Redis server...")
redis_client = None
while redis_client is None:
    try:
        redis_client = SignalClient(host=REDIS_HOST, port=REDIS_PORT)
    except Exception as e:
        print(f"Failed to connect to Redis server: {e}. Retrying in 1 second..")
        time.sleep(1 + random.uniform(-0.2, 0.2))
print("Connected to Redis server successfully.")

counter = 0

# Check shutdown signal from redis server every loop iteration. If shutdown is true, break the loop and end the program.
shutdown = redis_client.get_value("shutdown")

while not shutdown:
    # Update shutdown value
    shutdown = redis_client.get_value("shutdown")
    
    # Pull relevant signal from redis server
    ps_1_signal = redis_client.get("ps_1")
    ps_1_value = redis_client.get_value("ps_1")
    conveying_2 = redis_client.get_value("conveying_2")
    conveyor_2 = redis_client.get_value("conveyor_2")
    conveying_1 = redis_client.get_value("conveying_1")
    conveyor_1 = redis_client.get_value("conveyor_1")
    reset = redis_client.get_value("reset")

    if reset:
        counter = 0

    # If conveyor 1 is on and running increment the counter for ps_1. Reset counter if it has reached 2 and set ps_1 to true
    if conveying_1 and conveyor_1:
        if counter >= 2:
            counter = 0
            ps_1_value = True
        else:
            counter += 1
    # If conveyor 2 is on and running and the value of ps_1 is not already false, set it to false
    if conveying_2 and ps_1_value and conveyor_2:
        ps_1_value = False

    # Write updated value back to OpenPLC
    ps_1_signal.set_value(ps_1_value)
    modbus_client.write_signal(ps_1_signal)

    # Update redis server with new sensor value
    redis_client.set_value("ps_1", ps_1_value)

    time.sleep(1 + random.uniform(-0.2, 0.2))