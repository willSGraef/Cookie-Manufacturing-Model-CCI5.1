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

counter = 0

# Check shutdown signal from redis server every loop iteration. If shutdown is true, break the loop and end the program.
shutdown = redis_client.get_value("shutdown")

while not shutdown:
    # Update shutdown value
    shutdown = redis_client.get_value("shutdown")
    
    # Pull relevant signal from redis server
    boxer = redis_client.get("boxer")
    boxing_signal = redis_client.get("boxing")
    boxing_value = redis_client.get_value("boxing")
    conveying_2 = redis_client.get_value("conveying_2")
    conveyor_2 = redis_client.get_value("conveyor_2")
    reset = redis_client.get_value("reset")

    if reset:
        counter = 0

    # Compare redis value to OpenPLC value and update if necessary
    if boxer.get_value() != modbus_client.read_signal(boxer):
        boxer.set_value(modbus_client.read_signal(boxer))
        redis_client.set_value("boxer", boxer.get_value())
    if boxing_value != modbus_client.read_signal(boxing_signal):
        boxing_signal.set_value(modbus_client.read_signal(boxing_signal))
        redis_client.set_value("boxing", boxing_value)

    # If the boxer is boxing increment the counter, if the counter has reached 3, reset the counter and set boxing to false
    if boxing_value:
        if counter >= 3:
            counter = 0
            boxing_value = False
        else:
            counter += 1

    # Write updated value back to OpenPLC
    boxing_signal.set_value(boxing_value)
    modbus_client.write_signal(boxing_value)
    modbus_client.write_signal(boxer)

    # Update redis server with new values
    redis_client.set_value("boxing", boxing_value)
    redis_client.set_value("boxer", boxer.get_value())

    time.sleep(1)