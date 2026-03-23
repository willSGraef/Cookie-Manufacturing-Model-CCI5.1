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

wrapping_counter = 0
conveying_counter = 0

# Check shutdown signal from redis server every loop iteration. If shutdown is true, break the loop and end the program.
shutdown = redis_client.get_value("shutdown")

while not shutdown:
    reset = redis_client.get_value("reset")
    if reset:
        wrapping_counter = 0
        conveying_counter = 0
        time.sleep(1)
        continue

    # Update shutdown value
    shutdown = redis_client.get_value("shutdown")
    
    # Pull relevant signal from redis server
    wrapper = redis_client.get("wrapper")
    wrapping_signal = redis_client.get("wrapper_wrapping")
    wrapping_value = redis_client.get_value("wrapper_wrapping")
    wrapper_conveying_signal = redis_client.get("wrapper_conveying")
    wrapper_conveying_value = redis_client.get_value("wrapper_conveying")
    # Compare redis value to OpenPLC value and update if necessary
    if wrapper.get_value() != modbus_client.read_signal(wrapper):
        wrapper.set_value(modbus_client.read_signal(wrapper))
        redis_client.set_value("wrapper", wrapper.get_value())

    # Only read from Modbus if not currently timing
    if not wrapping_value:
        read = modbus_client.read_signal(wrapping_signal)
        if read is not None:
            wrapping_value = read
            wrapping_signal.set_value(wrapping_value)
            redis_client.set_value("wrapper_wrapping", wrapping_value)

    if not wrapper_conveying_value:
        read = modbus_client.read_signal(wrapper_conveying_signal)
        if read is not None:
            wrapper_conveying_value = read
            wrapper_conveying_signal.set_value(wrapper_conveying_value)
            redis_client.set_value("wrapper_conveying", wrapper_conveying_value)

    # Wrapping timer
    if wrapping_value:
        if wrapping_counter >= 3 * COUNTER_TIME_SCALE:
            wrapping_counter = 0
            wrapping_value = False
        else:
            wrapping_counter += 1

    # Conveying timer
    if wrapper_conveying_value:
        if conveying_counter >= 1 * COUNTER_TIME_SCALE:
            conveying_counter = 0
            wrapper_conveying_value = False
        else:
            conveying_counter += 1

    # Write updated value back to OpenPLC
    wrapping_signal.set_value(wrapping_value)
    wrapper_conveying_signal.set_value(wrapper_conveying_value)
    modbus_client.write_signal(wrapper_conveying_signal)
    modbus_client.write_signal(wrapping_signal)
    modbus_client.write_signal(wrapper)

    # Update redis server with new sensor value and counter value
    redis_client.set_value("wrapper_wrapping", wrapping_value)
    redis_client.set_value("wrapper_conveying", wrapper_conveying_value)
    redis_client.set_value("wrapper", wrapper.get_value())

    time.sleep(1 + random.uniform(-0.2, 0.2))