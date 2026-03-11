from core.constants import *
from utils.modbus_utils import FloatModbusClient
from utils.engine_utils import transfer_material
from utils.redis_client import SignalClient
from core.signals import SIGNALS
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
    trough = redis_client.get_value("trough_weight")
    trough_transfer = redis_client.get("trough_transfer")
    add_ingredients = redis_client.get("add_ingredients")
    reset = redis_client.get("reset")

    # Compare redis value to OpenPLC value and update if necessary
    if trough_transfer.get_value() != modbus_client.read_signal(trough_transfer):
        trough_transfer.set_value(modbus_client.read_signal(trough_transfer))
        redis_client.set_value("trough_transfer", trough_transfer.get_value())

    if add_ingredients.get_value() != modbus_client.read_signal(add_ingredients):
        add_ingredients.set_value(modbus_client.read_signal(add_ingredients))
        redis_client.set_value("add_ingredients", add_ingredients.get_value())

    if reset.get_value() != modbus_client.read_signal(reset):
        reset.set_value(modbus_client.read_signal(reset))
        redis_client.set_value("reset", reset.get_value())

    # Transfer material from silo to hopper
    if trough_transfer.get_value():
        if (trough + mixer <= 60):
            trough += mixer
            mixer = 0
        trough_transfer.set_value(False)

    # If the mixer isn't going to be overfilled by adding the non bulk ingredients, add them
    if add_ingredients.get_value():
        if (mixer + NON_BULK_INGREDIENT_WEIGHT) < 60:
            mixer = mixer + NON_BULK_INGREDIENT_WEIGHT
        add_ingredients.set_value(False)

    # If reset is true, reset all signals to their default values
    if reset.get_value():
        for signal in SIGNALS:
            redis_client.set_value(signal.name, signal.reset_value())
        reset.set_value(False)

    # Push simulated values to OpenPLC
    modbus_client.write_signal(trough_transfer)
    modbus_client.write_signal(add_ingredients)
    modbus_client.write_signal(reset)

    # Update redis server with new mixer, trough, and reset values
    redis_client.set_value("lcm", mixer)
    redis_client.set_value("trough_weight", trough)
    redis_client.set_value("reset", reset.get_value())

    time.sleep(1)