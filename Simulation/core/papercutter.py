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
    
    # Pull relevant signal from redis server
    paper_cutter = redis_client.get("paper_cutter")
    papercut_cpm = redis_client.get("papercut_cpm")

    # Compare redis value to OpenPLC value and update if necessary
    if paper_cutter.get_value() != modbus_client.read_signal(paper_cutter):
        paper_cutter.set_value(modbus_client.read_signal(paper_cutter))
        redis_client.set_value("paper_cutter", paper_cutter.get_value())

    if papercut_cpm.get_value() != modbus_client.read_signal(papercut_cpm):
        papercut_cpm.set_value(modbus_client.read_signal(papercut_cpm))
        redis_client.set_value("papercut_cpm", papercut_cpm.get_value())

    # Write updated value back to OpenPLC
    modbus_client.write_signal(paper_cutter)
    modbus_client.write_signal(papercut_cpm)

    time.sleep(1)