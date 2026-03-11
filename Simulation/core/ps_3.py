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
    ps_3_signal = redis_client.get("ps_3")
    ps_3_value = redis_client.get_value("ps_3")
    conveying_3 = redis_client.get_value("conveying_3")
    packing = redis_client.get_value("packing")
    sheet_count = redis_client.get_value("sheet_count")
    reset = redis_client.get_value("reset")

    if reset:
        counter = 0

    # If packing is true, increment the packing counter. If the counter has reached 8, reset it, set packing to false, 
    # decrement sheet count by 4, and set ps_3 to true to simulate a box being packed and leaving the station
    if packing:
        if counter >= 8:
            counter = 0
            packing = False
            sheet_count -= 4 # Pack a box full of 4 cookie sheets
            ps_3_value = True
        else:
            counter += 1
    # If conveyor 3 is running and ps_3 is true, set ps_3 to false to simulate the box leaving the station
    if conveying_3 and ps_3_value:
        ps_3_value = False

    # Write updated value back to OpenPLC
    ps_3_signal.set_value(ps_3_value)
    modbus_client.write_signal(ps_3_signal)

    # Update redis server with new sensor, sheet, and packing values
    redis_client.set_value("ps_3", ps_3_value)
    redis_client.set_value("sheet_count", sheet_count)
    redis_client.set_value("packing", packing)

    time.sleep(1)