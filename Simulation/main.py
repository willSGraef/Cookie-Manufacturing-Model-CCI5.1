#Date: 5/30/2025
#Author: William Graef, wgraef@uark.edu
#RIOT LAB, UNIVERSITY OF ARKANSAS

#Class description: The main class includes the handling of all incoming data and sends all calculated data to the PLC
#The main class also binds together each sensor object to match the visualized model, as well as the virtualized Containers

#Import dependencies
import logging
import struct
from pyModbusTCP.client import ModbusClient
from pyModbusTCP.utils import (decode_ieee, word_list_to_long)
from components import Container, Signal
from pathlib import Path

#-----------------------------
#Initialize modbus client and logging
#-----------------------------

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
            print(encode_ieee(f))
        # Write the registers to the specified address
        return self.write_multiple_registers(address, b16_l)

#Set debug level for pyModbusTCP client to see frame changes, basically shows us all connection logs
logging.basicConfig()
logging.getLogger('pyModbusTCP.client').setLevel(logging.DEBUG)

#Init client using IP for the PLC acting as the 'hostname' and the port being 502 (Basic modbus communication port)
#Enable auto open and auto close for our port so that it opens and closes for each invidiual request

client = FloatModbusClient(host = "localhost", port = 502, auto_open= True, auto_close= False)

#CONVEYING CONSTANTS
AIR_DENSITY = 0.075
MATERIAL_AIR_RATIO = 2.0
HOPPER_MAX_WEIGHT = 400.0
GRAVITY_CFM_ESTIMATE = 814
#The 814 is an estimate CFM according to the force of gravity, 
#the material having to fall 1 foot before reaching the mixer, 
#and the area of the hopper outlet being 244 square inches
SECONDS_PER_MIN = 60
SILO_WEIGHT = 10000.0

#Define RPM function, simulates the VFD at work
def RPM(frequency):
    return ((120*frequency)/2) #RPM from frequency, our motor has 2 poles so we have 2 in the denominator

#Define CFM function:

def CFM(frequency):
    rpm = RPM(frequency)
    #Use best fit curve function
    return ((-1.275*(pow(10, -7))) * pow(rpm, 2)) + ((0.7896)*rpm) - 365.43

#Initialize all discrete signals to their default values [STATE, ADDRESS]:

signals = {
    #"start_recipe" : Signal(False, 900), -- Start recipe should go from HMI to PLC
    "rv_1" : Signal(False, 700), #BOOL ...
    "rv_2" : Signal(False, 701),
    "rv_3" : Signal(False, 702),
    "dv" : Signal(False, 703),
    "vacuum" : Signal(False, 704),
    "mixer" : Signal(False, 705), 

    #Initialize all analog signals to their default values
    "vacuum_freq" : Signal(0, 1000), #INT ...
    "vacuum_rpm" : Signal(0, 1001),
    "mixer_freq" : Signal(0, 1002),
    "mixer_rpm" : Signal(0, 1003), 

    #Load cells for sugar, for REAL values make sure there
    #is enough room for both 16 bit addresses
    "lcs_1" : Signal(0.000, 100), #REAL ...
    "lcs_2" : Signal(0.000, 102),
    "lcs_3" : Signal(0.000, 104),
    "lcs_4" : Signal(0.000, 106),

    #Load cells for flour
    "lcf_1" : Signal(0.000, 108),
    "lcf_2" : Signal(0.000, 110),
    "lcf_3" : Signal(0.000, 112),
    "lcf_4" : Signal(0.000, 114),

    #Load cell for hopper
    "lch" : Signal(0.000, 98),

    "flour_weight" : Signal(0.000, 116),
    "sugar_weight" : Signal(0.000, 218)
}

#-----------------------------
# RESOURCE HANDLING
#-----------------------------

#Container and LI definitions, all Container weights are in pounds
#LC - Load Cell

flourSilo = Container(SILO_WEIGHT)

sugarSilo = Container(SILO_WEIGHT)
hopper = Container()

def runnable():
    #Read all signals from PLC into the signals global dictionary
    for Signal in signals.values():
        # Set booleans
        if isinstance(Signal.get_value(), bool):
            result = client.read_coils(Signal.get_address())
            if result is not None:
                Signal.set_value(result[0])
        # Set integers
        elif isinstance(Signal.get_value(), int):
            result = client.read_holding_registers(Signal.get_address())
            if result and len(result) > 0:
                Signal.set_value(result[0])  # Extract first value
        # Set floats
        elif isinstance(Signal.get_value(), float):
            result = client.read_float(Signal.get_address())
            if result is not None:
                Signal.set_value(result[0])
    #Check to see if vacuum is on, if it is start pulling
    print(signals['vacuum'].get_value())
    print(signals['rv_1'].get_value())
    print(signals['dv'].get_value())
    if signals['vacuum'].get_value():
        #Get vacuum voltage sent by PLC
        vacuum_freq = signals['vacuum_freq'].get_value()
        #Solve for vacuum CFM using best fit curve function
        vacuum_CFM = CFM(vacuum_freq)
        rate = (vacuum_CFM * MATERIAL_AIR_RATIO * AIR_DENSITY)/60
        if signals['rv_1'].get_value() and signals['dv'].get_value():
            print("Siphoning flour")
            if (hopper.get_weight() >= HOPPER_MAX_WEIGHT):
                print("Hopper at max capacity")
            else:
                flourSilo.set_weight(flourSilo.get_weight()-rate)
                hopper.set_weight(hopper.get_weight() + rate)
            if flourSilo.get_weight() < 0:
                flourSilo.set_weight(0)
        elif signals['rv_2'].get_value() and not signals['dv'].get_value():
            print("Siphoning sugar")
            if (hopper.get_weight() >= HOPPER_MAX_WEIGHT):
                print("Hopper at max capacity")
            else:
                sugarSilo.set_weight(sugarSilo.get_weight()-rate)
                hopper.set_weight(hopper.get_weight() + rate)
            if sugarSilo.get_weight() < 0:
                sugarSilo.set_weight(0)
    if signals["rv_3"].get_value():
        rate = (GRAVITY_CFM_ESTIMATE * MATERIAL_AIR_RATIO * AIR_DENSITY)/SECONDS_PER_MIN 
        hopper.set_weight(hopper.get_weight()-rate)
        if hopper.get_weight() < 0:
            hopper.set_weight(0)

    # Write all relevant signals to master device
    # Flour/Sugar load cells
    lcf_weight = flourSilo.get_weight()/4.0
    lcs_weight = sugarSilo.get_weight()/4.0
    for i in range(1, 5):
        client.write_float(signals[f'lcf_{i}'].get_address(), lcf_weight)
        client.write_float(signals[f'lcs_{i}'].get_address(), lcs_weight)
    
    # Hopper load cell
    lch_weight = hopper.get_weight()/1.0
    client.write_float(signals['lch'].get_address(), lch_weight)

    #Sugar and flour weight
    client.write_float(signals['flour_weight'].get_address(), flourSilo.get_weight())
    print(sugarSilo.get_weight())
    client.write_float(signals['sugar_weight'].get_address(), sugarSilo.get_weight())
    
    # Vacuum RPM
    rpm = RPM(signals['vacuum_freq'].get_value())
    client.write_single_register(signals['vacuum_rpm'].get_address(), int(rpm))

    # Mixer RPM
    #client.write_single_register(signals['mixer_rpm'].get_address(), 0)
    client.close()

#BEGIN SIMULATION
running = True
while(running):
    runnable()