import redis
from core.signals import Signal
from core.signals import SIGNALS
from core.constants import *

class SignalClient:

    def __init__(self, host=REDIS_HOST, port=REDIS_PORT):
        self.r = redis.Redis(host=host, port=port, decode_responses=True)

    def _parse_value(self, value):
        if value is None:
            return None
        if value in ["True", "False"]:
            return True if value == "True" else False
        try:
            if "." in value:
                return float(value)
            return int(value)
        except:
            return value

    def get_value(self, name):
        value = self.r.hget(f"signal:{name}", "value")
        return self._parse_value(value)

    def get(self, name):
        address = int(self.r.hget(f"signal:{name}", "address"))
        value = self.get_value(name)
        reset_value = self._parse_value(self.r.hget(f"signal:{name}", "reset_value"))
        return Signal(name, address, value, reset_value)
 
    def set_value(self, name, value):
        self.r.hset(f"signal:{name}", "value", str(value))

    def get_by_address(self, address):
        name = self.r.get(f"address:{address}")
        if name is None:
            raise KeyError(f"No signal registered at address {address}")
        return self.get(name)

    def reset(self, name):
        reset_value = self.r.hget(f"signal:{name}", "reset_value")
        print(f"Resetting {name} to {reset_value}, type: {type(reset_value)}")
        self.set_value(name, reset_value)
    
    def reset_all(self):
        for signal in SIGNALS:
            self.reset(signal.name)
    
    def close(self):
        self.r.connection_pool.disconnect()