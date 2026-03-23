import struct
from pyModbusTCP.client import ModbusClient
from pyModbusTCP.utils import decode_ieee, word_list_to_long

def encode_ieee(value):
    # Ensure big-endian float to two 16-bit registers
    return struct.unpack('>HH', struct.pack('>f', value))

#Modbus client with float reading and writing capabilities
class FloatModbusClient(ModbusClient):
    def read_float(self, address, number=1):
        #Read float(s) with read holding registers.
        reg_l = self.read_holding_registers(address, number * 2)
        if reg_l:
            return [decode_ieee(f) for f in word_list_to_long(reg_l)]
        else:
            return None

    def write_float(self, address, floats_list):
        if not isinstance(floats_list, list):
            floats_list = [floats_list]
        # Convert each float to a pair of 16-bit words
        b16_l = []
        for f in floats_list:
            b16_l.extend(encode_ieee(f))
        # Write the registers to the specified address
        return self.write_multiple_registers(address, b16_l)
    
    # Write signal value to modbus register based on signal type
    def write_signal(self, signal):
        value = signal.get_value()
        result = None
        if type(value) == bool:
            self.write_single_coil(signal.get_address(), value)
        elif isinstance(value, int):
            self.write_single_register(signal.get_address(), value)
        elif isinstance(value, float):
            self.write_float(signal.get_address(), value)
            print(f"write_signal: {signal.get_name()} = {value} @ address {signal.get_address()} | success: {result}")

    # Read signal value from modbus register based on signal type and set signal value
    def read_signal(self, signal):
        value = signal.get_value()
        address = signal.get_address()
        if isinstance(value, bool):
            result = self.read_coils(address)
            # print(signal.get_name() + " : " + (str)(result[0]))
            if result is not None:
                return result[0]    # Extract first value
        elif isinstance(value, int):
            result = self.read_holding_registers(address)
            # print(signal.get_name() + " : " + (str)(result[0]))
            if result and len(result) > 0:
                return result[0]
        elif isinstance(value, float):
            result = self.read_float(address)
            # print(signal.get_name() + " : " + (str)(result[0]))
            if result is not None:
                return result[0]