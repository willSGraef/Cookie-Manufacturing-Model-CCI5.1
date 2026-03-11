from core.constants import *
from utils.modbus_utils import FloatModbusClient
from utils.engine_utils import transfer_material
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
    mixer = redis_client.get_value("lcm")
    rv_3 = redis_client.get_value("rv_3")
    hopper_signal = redis_client.get("lch")
    hopper_value = hopper_signal.get_value()

    # Transfer material from silo to hopper
    if rv_3 and hopper_value > 0:
        rate = (GRAVITY_CFM_ESTIMATE * MATERIAL_AIR_RATIO * AIR_DENSITY)/SECONDS_PER_MIN 
        hopper_value, mixer = transfer_material(hopper_value, mixer, rate, MIXER_CAPACITY)

    # Push hopper weight to OpenPLC
    hopper_signal.set_value(hopper_value)

    modbus_client.write_signal(hopper_signal)

    # Update redis server with new mixer and hopper values
    redis_client.set_value("lch", hopper_value)
    redis_client.set_value("lcm", mixer)

    time.sleep(1)