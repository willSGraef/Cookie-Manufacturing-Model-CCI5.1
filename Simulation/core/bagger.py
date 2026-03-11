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
    bagger = redis_client.get("bagger")
    bagging_signal = redis_client.get("bagging")
    bagging_value = redis_client.get_value("bagging")
    conveying_2 = redis_client.get_value("conveying_2")
    conveyor_2 = redis_client.get_value("conveyor_2")
    reset = redis_client.get_value("reset")

    if reset:
        counter = 0

    # Compare redis value to OpenPLC value and update if necessary
    if bagger.get_value() != modbus_client.read_signal(bagger):
        bagger.set_value(modbus_client.read_signal(bagger))
        redis_client.set_value("bagger", bagger.get_value())
    if bagging_value != modbus_client.read_signal(bagging_signal):
        bagging_signal.set_value(modbus_client.read_signal(bagging_signal))
        redis_client.set_value("taping", bagging_value)

    # If conveyor 2 is on and running, increment the boxer counter. 
    # If the boxer counter has reached 2, reset it and set ps_2 to true to 
    # simulate a box being present
    if bagging_value:
        if counter >= 1:
            counter = 0
            bagging_value = False
        else:
            counter += 1

    # Write updated value back to OpenPLC
    bagging_signal.set_value(bagging_value)
    modbus_client.write_signal(bagging_signal)
    modbus_client.write_signal(bagger)

    # Update redis server with new sensor value and counter value
    redis_client.set_value("bagging", bagging_value)
    redis_client.set_value("bagger", bagger.get_value())

    time.sleep(1)