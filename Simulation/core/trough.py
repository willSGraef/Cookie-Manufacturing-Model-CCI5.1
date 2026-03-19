from core.constants import *
from utils.modbus_utils import FloatModbusClient
from utils.redis_client import SignalClient
import time
import signal
import sys

def handle_shutdown(signum, frame):
    if modbus_client:
        modbus_client.close()
    if redis_client:
        redis_client.close()
    sys.exit(0)

signal.signal(signal.SIGTERM, handle_shutdown)
signal.signal(signal.SIGINT, handle_shutdown)

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

# Check shutdown signal from redis server every loop iteration. If shutdown is true, break the loop and end the program.
shutdown = redis_client.get_value("shutdown")

while not shutdown:
    # Update shutdown value
    shutdown = redis_client.get_value("shutdown")
    
    # Pull all relevant signals from redis server
    trough_signal = redis_client.get("trough_weight")
    trough_value = trough_signal.get_value()
    wirecutter = redis_client.get_value("wirecutter")
    wirecut_cpm = redis_client.get_value("wirecut_cpm")
    sheet_count = redis_client.get_value("sheet_count")
    row_count = redis_client.get_value("row_count")

    # Transfer material from silo to hopper
    if trough_value > 0 and wirecutter:
        rate = ((wirecut_cpm * COOKIE_COUNT * COOKIE_WEIGHT)/16.0)/SECONDS_PER_MIN
        if (trough_value - rate) < 0.0: # Prevent negative number in trough
            trough_value = 0.0
        else:
            trough_value -= rate
        row_count += wirecut_cpm/SECONDS_PER_MIN # How many rows are cut per second
        if row_count >= 5: 
            row_count -= 5
            sheet_count += 1 # Increase sheet count when 5 rows have been cut & reset row count leaving room for excess
    elif trough_value < 0:
        trough_value = 0.0

    # Push trough weight to OpenPLC
    trough_signal.set_value(trough_value)

    modbus_client.write_signal(trough_signal)

    # Update redis server with new trough weight, sheet count, and row count values
    redis_client.set_value("trough_weight", trough_value)
    redis_client.set_value("sheet_count", sheet_count)
    redis_client.set_value("row_count", row_count)

    time.sleep(1)