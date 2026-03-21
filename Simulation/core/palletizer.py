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

# Check shutdown signal from redis server every loop iteration. If shutdown is true, break the loop and end the program.
shutdown = redis_client.get_value("shutdown")

while not shutdown:
    # Update shutdown value
    shutdown = redis_client.get_value("shutdown")
    
    # Pull relevant signal from redis server
    palletizer = redis_client.get("palletizer")
    palletizer_rotation = redis_client.get("palletizer_rotation")
    palletizer_rotation_value = palletizer_rotation.get_value()
    palletizer_target_rotation = redis_client.get("palletizer_target_rotation")
    palletizer_moving = redis_client.get("palletizer_moving")
    palletizer_grabbing = redis_client.get("palletizer_grabbing")

    # Compare redis value to OpenPLC value and update if necessary
    if palletizer.get_value() != modbus_client.read_signal(palletizer):
        palletizer.set_value(modbus_client.read_signal(palletizer))
        redis_client.set_value("palletizer", palletizer.get_value())
    if palletizer_target_rotation.get_value() != modbus_client.read_signal(palletizer_target_rotation):
        palletizer_target_rotation.set_value(modbus_client.read_signal(palletizer_target_rotation))
        redis_client.set_value("palletizer_target_rotation", palletizer_target_rotation.get_value())
    if palletizer_moving.get_value() != modbus_client.read_signal(palletizer_moving):
        palletizer_moving.set_value(modbus_client.read_signal(palletizer_moving))
        redis_client.set_value("palletizer_moving", palletizer_moving.get_value())
    if palletizer_grabbing.get_value() != modbus_client.read_signal(palletizer_grabbing):
        palletizer_grabbing.set_value(modbus_client.read_signal(palletizer_grabbing))
        redis_client.set_value("palletizer_grabbing", palletizer_grabbing.get_value())

    print("Target rotation: ", palletizer_target_rotation)
    print("Actual rotation: ", palletizer_rotation_value)

    if palletizer_moving.get_value():
        if palletizer_target_rotation.get_value() == palletizer_rotation_value:
            print("Target and actual equal, no longer rotating...")
            if palletizer_rotation_value == 225:
                redis_client.set_value("roe_1", True)
                redis_client.set_value("roe_2", False)
                redis_client.set_value("roe_3", False)
            elif palletizer_rotation_value == 180:
                redis_client.set_value("roe_2", True)
                redis_client.set_value("roe_1", False)
                redis_client.set_value("roe_3", False)
            elif palletizer_rotation_value == 130:
                redis_client.set_value("roe_3", True)
                redis_client.set_value("roe_1", False)
                redis_client.set_value("roe_2", False)
            else:
                redis_client.set_value("roe_1", False)
                redis_client.set_value("roe_2", False)
                redis_client.set_value("roe_3", False)
        else:
            print("Rotating...")
            if palletizer_rotation_value + ARM_ROTATION_SPEED < palletizer_target_rotation.get_value():
                palletizer_rotation_value += ARM_ROTATION_SPEED
                if palletizer_rotation_value > palletizer_target_rotation.get_value():
                    palletizer_rotation_value = palletizer_target_rotation.get_value()
            elif palletizer_rotation_value + ARM_ROTATION_SPEED > palletizer_target_rotation.get_value():
                palletizer_rotation_value -= ARM_ROTATION_SPEED
                if palletizer_rotation_value < palletizer_target_rotation.get_value():
                    palletizer_rotation_value = palletizer_target_rotation.get_value()

    # Push mixer weight to OpenPLC
    modbus_client.write_signal(palletizer)
    modbus_client.write_signal(palletizer_target_rotation)
    modbus_client.write_signal(palletizer_moving)
    modbus_client.write_signal(palletizer_grabbing)

    # Update redis server with new palletizer values
    redis_client.set_value("palletizer", palletizer.get_value())
    redis_client.set_value("palletizer_target_rotation", palletizer_target_rotation.get_value())
    redis_client.set_value("palletizer_moving", palletizer_moving.get_value())
    redis_client.set_value("palletizer_grabbing", palletizer_grabbing.get_value())
    redis_client.set_value("palletizer_rotation", palletizer_rotation_value)

    time.sleep(1 + random.uniform(-0.2, 0.2))