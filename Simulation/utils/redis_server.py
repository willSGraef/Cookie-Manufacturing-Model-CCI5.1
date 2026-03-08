import redis
from core.signals import SIGNALS

REDIS_HOST = "redis"
REDIS_PORT = 6379

r = redis.Redis(host=REDIS_HOST, port=REDIS_PORT, decode_responses=True)


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


if __name__ == "__main__":
    initialize_signals()