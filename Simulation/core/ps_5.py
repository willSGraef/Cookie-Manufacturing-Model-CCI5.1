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
    reset = redis_client.get_value("reset")
    if reset:
        counter = 0
        time.sleep(1)
        continue

    # Update shutdown value
    shutdown = redis_client.get_value("shutdown")
    
    # Pull relevant signal from redis server
    ps_5_signal = redis_client.get("ps_5")
    ps_5_value = redis_client.get_value("ps_5")
    conveying_4 = redis_client.get_value("conveying_4")
    palletizer_rotation = redis_client.get_value("palletizer_rotation")
    palletizer_grabbing = redis_client.get_value("palletizer_grabbing")
    palletizer_moving = redis_client.get_value("palletizer_moving")
    # If conveyor 4 is running, increment the counter for ps_5. Reset counter if it has reached 2 and set ps_5 to true
    if conveying_4:
        if counter >= 2:
            counter = 0
            ps_5_value = True
        else:
            counter += 1
    # If the palletizer is moving, ps_5 is true, the palletizer rotation is at 225, and the palletizer is grabbing, set ps_5
    # to false to simulate the box being removed from the conveyor by the palletizer
    if palletizer_moving:
        if ps_5_value and palletizer_rotation == 225 and palletizer_grabbing:
            ps_5_value = False

    # Write updated value back to OpenPLC
    ps_5_signal.set_value(ps_5_value)
    modbus_client.write_signal(ps_5_signal)

    # Update redis server with new sensor value
    redis_client.set_value("ps_5", ps_5_value)

    time.sleep(1 + random.uniform(-0.2, 0.2))