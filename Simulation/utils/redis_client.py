import redis
from core.signals import Signal
from core.constants import *

class SignalClient:

    def __init__(self, host=REDIS_HOST, port=REDIS_PORT):

        self.r = redis.Redis(host=host, port=port, decode_responses=True)

    def get_value(self, name):
        value = self.r.hget(f"signal:{name}", "value")
        if value in ["True", "False"]:
            value = True if value == "True" else False
        try:
            if "." in value:
                value = float(value)
            value = int(value)
        except:
            value = value
        return value

    def get(self, name):

        address = self.r.hget(f"signal:{name}", "address")
        value = self.get_value(name)
        reset_value = self.r.hget(f"signal:{name}", "reset_value")

        return Signal(name, address, value, reset_value)
 
    def set_value(self, name, value):

        self.r.hset(f"signal:{name}", "value", value)

    def get_by_address(self, address):

        name = self.r.get(f"address:{address}")
        return self.get(name)

    def set_by_address(self, address, value):

        name = self.r.get(f"address:{address}")
        self.set(name, value)

    def reset(self, name):

        reset_value = self.r.hget(f"signal:{name}", "reset_value")
        self.set(name, reset_value)