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
    ps_4 = redis_client.get_value("ps_4")
    taping_signal = redis_client.get("taping")
    taping_value = redis_client.get_value("taping")
    taper = redis_client.get("taper")
    # Compare redis value to OpenPLC value and update if necessary
    if taper.get_value() != modbus_client.read_signal(taper):
        taper.set_value(modbus_client.read_signal(taper))
        redis_client.set_value("taper", taper.get_value())
    
    # If photoelectric sensor 4 is on, set taping to true
    if ps_4:
        taping_value = True

    # If taping is on and running, increment the taping counter
    # If the taping counter has reached 1, reset it and set taping to false
    if taping_value:
        if counter >= 1 * COUNTER_TIME_SCALE:
            counter = 0
            taping_value = False
        else:
            counter += 1

    # Push taping signal and taper signal to OpenPLC
    taping_signal.set_value(taping_value)
    modbus_client.write_signal(taping_signal)
    modbus_client.write_signal(taper)

    # Update redis server with new taping value
    redis_client.set_value("taping", taping_value)

    time.sleep(1 + random.uniform(-0.2, 0.2))