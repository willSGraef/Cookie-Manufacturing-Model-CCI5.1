from core.constants import *
from utils.modbus_utils import FloatModbusClient
from utils.engine_utils import transfer_material, CFM
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

# Check shutdown signal from redis server every loop iteration. If shutdown is true, break the loop and end the program.
shutdown = redis_client.get_value("shutdown")

while not shutdown:
    # Update shutdown value
    shutdown = redis_client.get_value("shutdown")
    
    # Pull all relevant signals from redis server
    vacuum = redis_client.get_value("vacuum")
    vacuum_rpm = redis_client.get_value("vacuum_rpm")
    rv_2 = redis_client.get_value("rv_2")
    dv = redis_client.get_value("dv")
    lcs_1 = redis_client.get("lcs_1")
    lcs_2 = redis_client.get("lcs_2")
    lcs_3 = redis_client.get("lcs_3")
    lcs_4 = redis_client.get("lcs_4")
    sugar_silo = lcs_1.get_value() + lcs_2.get_value() + lcs_3.get_value() + lcs_4.get_value()
    hopper = redis_client.get_value("lch")

    # Transfer material from silo to hopper
    if vacuum:
        vacuum_cfm = CFM(vacuum_rpm)
        rate = (vacuum_cfm * MATERIAL_AIR_RATIO * AIR_DENSITY)/SECONDS_PER_MIN
        if rv_2 and not dv and sugar_silo > 0 and hopper + rate <= HOPPER_CAPACITY:
            sugar_silo, hopper = transfer_material(sugar_silo, hopper, rate, HOPPER_CAPACITY)

    # Push sugar weight to OpenPLC
    lcs_1.set_value(sugar_silo/4.0)
    lcs_2.set_value(sugar_silo/4.0)
    lcs_3.set_value(sugar_silo/4.0)
    lcs_4.set_value(sugar_silo/4.0)

    modbus_client.write_signal(lcs_1)
    modbus_client.write_signal(lcs_2) 
    modbus_client.write_signal(lcs_3)
    modbus_client.write_signal(lcs_4)

    # Update redis server with new silo and hopper values
    redis_client.set_value("lcs_1", lcs_1.get_value())
    redis_client.set_value("lcs_2", lcs_2.get_value())
    redis_client.set_value("lcs_3", lcs_3.get_value())
    redis_client.set_value("lcs_4", lcs_4.get_value())

    redis_client.set_value("lch", hopper)

    time.sleep(1 + random.uniform(-0.2, 0.2))