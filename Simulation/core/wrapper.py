from core.constants import *
from utils.modbus_utils import FloatModbusClient
from utils.redis_client import SignalClient
import time

# Establish modbus client connection
print("Connecting to OpenPLC...")
modbus_client = None
while modbus_client is None:
    try:
        modbus_client = FloatModbusClient(host = OPENPLC_HOST, port = OPENPLC_PORT, auto_open= True, auto_close= False)
    except Exception as e:
        print(f"Failed to connect to OpenPLC: {e}. Retrying in 1 second..")
        time.sleep(1)
print("Connected to OpenPLC successfully.")

# Establish redis client connection
print("Connecting to Redis server...")
redis_client = None
while redis_client is None:
    try:
        redis_client = SignalClient(host=REDIS_HOST, port=REDIS_PORT)
    except Exception as e:
        print(f"Failed to connect to Redis server: {e}. Retrying in 1 second..")
        time.sleep(1)
print("Connected to Redis server successfully.")

counter = 0

# Check shutdown signal from redis server every loop iteration. If shutdown is true, break the loop and end the program.
shutdown = redis_client.get_value("shutdown")

while not shutdown:
    # Update shutdown value
    shutdown = redis_client.get_value("shutdown")
    
    # Pull relevant signal from redis server
    wrapper = redis_client.get("wrapper")
    wrapping_signal = redis_client.get("wrapper_wrapping")
    wrapping_value = redis_client.get_value("wrapper_wrapping")
    wrapper_conveying_signal = redis_client.get("wrapper_conveying")
    wrapper_conveying_value = redis_client.get_value("wrapper_conveying")
    reset = redis_client.get_value("reset")

    if reset:
        counter = 0

    # Compare redis value to OpenPLC value and update if necessary
    if wrapper.get_value() != modbus_client.read_signal(wrapper):
        wrapper.set_value(modbus_client.read_signal(wrapper))
        redis_client.set_value("wrapper", wrapper.get_value())
    if wrapping_value != modbus_client.read_signal(wrapping_signal):
        wrapping_signal.set_value(modbus_client.read_signal(wrapping_signal))
        redis_client.set_value("wrapper_wrapping", wrapping_value)
    if wrapper_conveying_value != modbus_client.read_signal(wrapper_conveying_signal):
        wrapper_conveying_signal.set_value(modbus_client.read_signal(wrapper_conveying_signal))
        redis_client.set_value("wrapper_conveying", wrapper_conveying_value)

    # If wrapper conveying is on and running, increment the wrapper counter. 
    # If the wrapper counter has reached 1, reset it and set wrapper_conveying to false
    if wrapper_conveying_value:
        if counter >= 1:
            counter = 0
            wrapper_conveying_value = False
        else:
            counter += 1
    # If wrapper is on and running, increment the wrapper counter. If the wrapper counter has reached 3, reset it and set wrapping to false
    elif wrapping_value:
        if counter >= 3:
            counter = 0
            wrapping_value = False
        else:
            counter += 1

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

    time.sleep(1)