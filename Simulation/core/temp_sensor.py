from core.constants import *
from utils.modbus_utils import FloatModbusClient
from utils.redis_client import SignalClient
import random
import time
import signal
import sys
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

# Check shutdown signal from redis server every loop iteration. If shutdown is true, break the loop and end the program.
shutdown = redis_client.get_value("shutdown")

while not shutdown:
    # Update shutdown value
    shutdown = redis_client.get_value("shutdown")
    
    # Pull all relevant signals from redis server
    gv_1 = redis_client.get_value("gv_1")
    temperature_signal = redis_client.get("tunnel_temp")
    temperature_value = redis_client.get_value("tunnel_temp")
    exhaust_fan = redis_client.get_value("exhaust_fan")
    fan_1 = redis_client.get_value("fan_1")
    fan_2 = redis_client.get_value("fan_2")
    fan_3 = redis_client.get_value("fan_3")

    # Transfer material from silo to hopper
    if gv_1():
        temp_noise = random.uniform(0.0, 1.0)
        if exhaust_fan:
            if temperature_value > -10.0:
                temperature_value -= FREEZING_RATE + temp_noise
            else:
                temperature = (-10.0 - temp_noise)
        elif not exhaust_fan:
            if temperature_value > -140.0:
                temperature_value -= FREEZING_RATE + temp_noise
            else:
                temperature = (-140.0 - temp_noise)
    elif not gv_1():
        temp_noise = random.uniform(0.0, 0.1)
        if temperature_value < 20.0:
            rate = WARMING_RATE
            #Increase the warming rate based on how many circulating fans are on
            if fan_1():
                rate += WARMING_RATE
            if fan_2():
                rate += WARMING_RATE
            if fan_3():
                rate += WARMING_RATE
            temperature_value += (rate + temp_noise)
        else:
            temperature = (20.0 + temp_noise)

    # Push sugar weight to OpenPLC
    temperature_signal.set_value(temperature_value)

    modbus_client.write_signal(temperature_signal)

    # Update redis server with new silo and hopper values
    redis_client.set_value("tunnel_temp", temperature_signal)

    time.sleep(1 + random.uniform(-0.2, 0.2))