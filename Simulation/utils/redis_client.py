import redis

class SignalClient:

    def __init__(self, host="redis", port=6379):

        self.r = redis.Redis(host=host, port=port, decode_responses=True)

    def get(self, name):

        value = self.r.hget(f"signal:{name}", "value")

        if value in ["True", "False"]:
            return value == "True"

        try:
            if "." in value:
                return float(value)
            return int(value)
        except:
            return value

    def set(self, name, value):

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