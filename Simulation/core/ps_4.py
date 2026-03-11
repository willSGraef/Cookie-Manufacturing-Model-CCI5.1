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
    ps_4_signal = redis_client.get("ps_4")
    ps_4_value = redis_client.get_value("ps_4")
    conveying_3 = redis_client.get_value("conveying_3")
    conveying_4 = redis_client.get_value("conveying_4")
    reset = redis_client.get_value("reset")

    if reset:
        counter = 0

    # If conveyor 3 is running, increment the counter for ps_4. Reset counter if it has reached 2 and set ps_4 to true
    if conveying_3:
        if counter >= 2:
            counter = 0
            ps_4_value = True
        else:
            counter += 1
    # If conveyor 4 is running and ps_4 is true, set ps_4 to false
    if conveying_4 and ps_4_value:
        ps_4_value = False

    # Write updated value back to OpenPLC
    ps_4_signal.set_value(ps_4_value)
    modbus_client.write_signal(ps_4_signal)

    # Update redis server with new sensor value
    redis_client.set_value("ps_4", ps_4_value)
    
    time.sleep(1)