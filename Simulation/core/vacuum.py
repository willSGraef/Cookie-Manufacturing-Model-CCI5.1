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
    vacuum = redis_client.get("vacuum")
    vacuum_rpm = redis_client.get("vacuum_rpm")
    vacuum_manual = redis_client.get("vacuum_manual")

    # Compare redis value to OpenPLC value and update if necessary
    if vacuum.get_value() != modbus_client.read_signal(vacuum):
        vacuum.set_value(modbus_client.read_signal(vacuum))
        redis_client.set_value("vacuum", vacuum.get_value())

    if vacuum_rpm.get_value() != modbus_client.read_signal(vacuum_rpm):
        vacuum_rpm.set_value(modbus_client.read_signal(vacuum_rpm))
        redis_client.set_value("vacuum_rpm", vacuum_rpm.get_value())

    if vacuum_manual.get_value() != modbus_client.read_signal(vacuum_manual):
        vacuum_manual.set_value(modbus_client.read_signal(vacuum_manual))
        redis_client.set_value("vacuum_manual", vacuum_manual.get_value())

    # Write updated value back to OpenPLC
    modbus_client.write_signal(vacuum)
    modbus_client.write_signal(vacuum_rpm)
    modbus_client.write_signal(vacuum_manual)

    time.sleep(1)