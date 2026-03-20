import redis
from core.signals import SIGNALS
from core.constants import *
import subprocess
import time

# Wait for Redis to be ready
r = redis.Redis(host='127.0.0.1', port=REDIS_PORT, decode_responses=True)
while True:
    try:
        r.ping()
        print("Redis server is ready.")
        break
    except redis.exceptions.ConnectionError:
        print("Waiting for Redis server to start...")
        time.sleep(0.5)

def initialize_signals():

    for signal in SIGNALS:

        key = f"signal:{signal.name}"
        reset_value = str(signal.reset_value)
        value = str(signal.value)

        r.hset(
            key,
            mapping={
                "name": signal.name,
                "address": signal.address,
                "value": value,
                "reset_value": reset_value,
            },
        )

        # reverse lookup for modbus mapping
        r.set(f"address:{signal.address}", signal.name)

    print(f"Loaded {len(SIGNALS)} signals into Redis")

initialize_signals()