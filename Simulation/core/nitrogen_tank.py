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

# Check shutdown signal from redis server every loop iteration. If shutdown is true, break the loop and end the program.
shutdown = redis_client.get_value("shutdown")

while not shutdown:
    # Update shutdown value
    shutdown = redis_client.get_value("shutdown")
    
    # Pull all relevant signals from redis server
    gv_1 = redis_client.get_value("gv_1")
    nitrogen_volume_signal = redis_client.get("nitrogen_volume")
    nitrogen_volume_value = redis_client.get_value("nitrogen_volume")

    # Transfer material from tank to tunnel
    if gv_1:
        rate = 1365.0/3600.0
        if nitrogen_volume_value < rate or nitrogen_volume_value < 0:
            nitrogen_volume_value = 0
        else:
            nitrogen_volume_value -= rate

    # Push nitrogen volume to OpenPLC
    nitrogen_volume_signal.set_value(nitrogen_volume_value)

    modbus_client.write_signal(nitrogen_volume_signal)

    # Update redis server with new nitrogen volume value
    redis_client.set_value("nitrogen_volume", nitrogen_volume_signal)

    time.sleep(1)