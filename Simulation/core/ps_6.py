from core.constants import *
from utils.modbus_utils import FloatModbusClient
from utils.redis_client import SignalClient
import random
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
    ps_6_signal = redis_client.get("ps_6")
    ps_6_value = redis_client.get_value("ps_6")
    conveying_5 = redis_client.get_value("conveying_5")
    reset = redis_client.get_value("reset")

    if reset:
        counter = 0

    # If ps_6 is true, randomly set it to false to simulate the time it takes for a forklift driver to pick up a finished pallet
    if ps_6_value and random.randint(1,5) == 3:
            ps_6_value = False

    # If conveyor 5 is running, increment the counter for ps_6. Reset counter if it has reached 8 and set ps_6 to true
    if conveying_5:
        if counter >= 8:
            counter = 0
            ps_6_value = True
        else:
            counter += 1

    # Write updated value back to OpenPLC
    ps_6_signal.set_value(ps_6_value)
    modbus_client.write_signal(ps_6_signal)

    # Update redis server with new sensor value
    redis_client.set_value("ps_6", ps_6_value)

    time.sleep(1)