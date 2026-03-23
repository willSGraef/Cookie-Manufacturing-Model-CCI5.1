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
    ps_2_signal = redis_client.get("ps_2")
    ps_2_value = redis_client.get_value("ps_2")
    conveying_2 = redis_client.get_value("conveying_2")
    conveyor_2 = redis_client.get_value("conveyor_2")
    packing = redis_client.get_value("packing")
    sheet_count = redis_client.get_value("sheet_count")
    worker_present = redis_client.get_value("worker_present")
    # If conveyor 2 is on and running, increment the boxer counter. 
    # If the boxer counter has reached 2, reset it and set ps_2 to true to 
    # simulate a box being present
    if conveying_2 and conveyor_2:
        if counter >= 2 * COUNTER_TIME_SCALE:
            counter = 0
            ps_2_value = True
        else:
            counter += 1
    # If ps_2 is true, there are at least 4 sheets, packing is not already in progress, and the worker is present, 
    # set packing to true and reset ps_2 to false to simulate the worker starting the packing process
    if ps_2_value and (sheet_count >= 4) and packing == False and worker_present:
        packing = True
        ps_2_value = False

    # Write updated value back to OpenPLC
    ps_2_signal.set_value(ps_2_value)
    modbus_client.write_signal(ps_2_signal)

    # Update redis server with new sensor value and packing value
    redis_client.set_value("ps_2", ps_2_value)
    redis_client.set_value("packing", packing)

    time.sleep(1 + random.uniform(-0.2, 0.2))