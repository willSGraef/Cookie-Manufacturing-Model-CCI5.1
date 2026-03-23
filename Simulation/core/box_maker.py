from core.constants import *
from utils.modbus_utils import FloatModbusClient
from utils.redis_client import SignalClient
import time
import signal
import sys
import random
import socket

def handle_shutdown(signum, frame):
    if modbus_client:
        modbus_client.close()
    if redis_client:
        redis_client.close()
    sys.exit(0)

signal.signal(signal.SIGTERM, handle_shutdown)
signal.signal(signal.SIGINT, handle_shutdown)

# Stagger startup based on IP to avoid overwhelming Redis
ip = socket.gethostbyname(socket.gethostname())
last_octet = int(ip.split('.')[-1])
stagger_delay = (last_octet % 20) * 0.5
print(f"Staggering startup by {stagger_delay} seconds...")
time.sleep(stagger_delay)


# Establish modbus client connection
print("Connecting to OpenPLC...")
modbus_client = None
while modbus_client is None:
    try:
        modbus_client = FloatModbusClient(host = OPENPLC_HOST, port = OPENPLC_PORT, auto_open= True, auto_close= False)
    except Exception as e:
        print(f"Failed to connect to OpenPLC: {e}. Retrying in 1 second..")
        time.sleep(1 + random.uniform(-0.2, 0.2))
print("Connected to OpenPLC successfully.")

# Establish redis client connection
print("Connecting to Redis server...")
redis_client = None
while redis_client is None:
    try:
        redis_client = SignalClient(host=REDIS_HOST, port=REDIS_PORT)
    except Exception as e:
        print(f"Failed to connect to Redis server: {e}. Retrying in 1 second..")
        time.sleep(1 + random.uniform(-0.2, 0.2))
print("Connected to Redis server successfully.")

counter = 0

# Check shutdown signal from redis server every loop iteration. If shutdown is true, break the loop and end the program.
shutdown = redis_client.get_value("shutdown")

while not shutdown:
    reset = redis_client.get_value("reset")
    if reset:
        counter = 0
        time.sleep(1)
        continue

    # Update shutdown value
    shutdown = redis_client.get_value("shutdown")
    
    # Pull relevant signal from redis server
    ps_2 = redis_client.get_value("ps_1")
    box_maker = redis_client.get("box_maker")
    boxing_signal = redis_client.get("boxing")
    boxing_value = redis_client.get_value("boxing")
    conveying_2 = redis_client.get_value("conveying_2")
    conveyor_2 = redis_client.get_value("conveyor_2")
    # Compare redis value to OpenPLC value and update if necessary
    if box_maker.get_value() != modbus_client.read_signal(box_maker):
        box_maker.set_value(modbus_client.read_signal(box_maker))
        redis_client.set_value("box_maker", box_maker.get_value())

    # If photoelectric sensor 2 is off, set boxing to true
    if not ps_2:
        boxing_value = True

    # If the box_maker is boxing increment the counter, if the counter has reached 3, reset the counter and set boxing to false
    if boxing_value:
        if counter >= 3 * COUNTER_TIME_SCALE:
            counter = 0
            boxing_value = False
        else:
            counter += 1

    # Write updated value back to OpenPLC
    boxing_signal.set_value(boxing_value)
    modbus_client.write_signal(boxing_signal)
    modbus_client.write_signal(box_maker)

    # Update redis server with new values
    redis_client.set_value("boxing", boxing_value)
    redis_client.set_value("box_maker", box_maker.get_value())

    time.sleep(1 + random.uniform(-0.2, 0.2))