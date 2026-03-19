import redis
from core.signals import SIGNALS
from core.constants import *
import subprocess
import time

# Start the actual Redis server process
subprocess.Popen(['redis-server'])

# Wait for Redis to be ready
r = redis.Redis(host=REDIS_HOST, port=REDIS_PORT, decode_responses=True)
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

        r.hset(
            key,
            mapping={
                "name": signal.name,
                "address": signal.address,
                "value": signal.value,
                "reset_value": signal.reset_value,
            },
        )

        # reverse lookup for modbus mapping
        r.set(f"address:{signal.address}", signal.name)

    print(f"Loaded {len(SIGNALS)} signals into Redis")

initialize_signals()