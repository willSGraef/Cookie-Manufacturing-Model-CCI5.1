#Date: 5/30/2025
#Author: William Graef, wgraef@uark.edu
#RIOT LAB, UNIVERSITY OF ARKANSAS

#Class description: The main class includes the handling of all incoming data and sends all calculated data to the PLC
#The main class also binds together each sensor object to match the visualized model, as well as the virtualized Containers

#Import dependencies
import logging
import struct
import random
from pyModbusTCP.client import ModbusClient
from pyModbusTCP.utils import (decode_ieee, word_list_to_long)
from components import Container, Signal

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
GRAVITY_CFM_ESTIMATE = 814
#The 814 is an estimate CFM according to the force of gravity, 
#the material having to fall 1 foot before reaching the mixer, 
#and the area of the hopper outlet being 244 square inches
SECONDS_PER_MIN = 60
SILO_CAPACITY = 1000.0
TANK_CAPACITY = 500.0
HOPPER_CAPACITY = 40.0
MIXER_CAPACITY = 60.0
TROUGH_CAPACITY = 60.0
AMBIENT_TUNNEL_TEMP = 20.0 #Celcius
FREEZING_COOKIE_TEMP = -10.0 #Celcius
FREEZING_RATE = 4.5 #C/Second
WARMING_RATE = 0.1


#Define CFM function:

def CFM(rpm):
    #Use best fit curve function
    return ((-1.275*(pow(10, -7))) * pow(rpm, 2)) + ((0.7896)*rpm) - 365.43

#Initialize all discrete signals to their default values [STATE, ADDRESS]:

SIGNALS = {
    #"start_recipe" : Signal(False, 900), -- Start recipe should go from HMI to PLC
    "rv_1" : Signal(False, 700), #BOOL ...
    "rv_2" : Signal(False, 701),
    "rv_3" : Signal(False, 702),
    "dv" : Signal(False, 703),
    "vacuum" : Signal(False, 704),
    "mixer" : Signal(False, 705),
    "wirecutter" : Signal(False, 711),
    "trough_transfer" : Signal(False, 713),
    "gv_1" : Signal(False, 718),
    "exhaust_fan" : Signal(False, 722),
    "conveyor" : Signal(False, 716),
    "paper_cutter" : Signal(False, 717),

    #Initialize all analog signals to their default values
    "vacuum_rpm" : Signal(0, 1001),
    "mixer_rpm" : Signal(0, 1003), 
    "wirecut_cpm" : Signal(0, 1006),
    "conveyor_fpm" : Signal(0, 1007),

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

    #Load cell for mixer
    "lcm" : Signal(0.000, 96),

    #Load cell for wirecutter trough (This is a physical trough, something that an operator would physically put dough in and then transfer to the wirecutter)
    "trough_weight" : Signal(0.000, 94),

    "flour_weight" : Signal(0.000, 116),
    "sugar_weight" : Signal(0.000, 218),

    #Nitrogen tunnel temperature
    "tunnel_temp" : Signal(0.000, 302),

    #Nitrogen volume

    "nitrogen_volume" : Signal(0.000, 300)
}

#-----------------------------
# RESOURCE HANDLING
#-----------------------------

#Container and LI definitions, all Container weights are in pounds
#LC - Load Cell

flourSilo = Container(SILO_CAPACITY)
sugarSilo = Container(SILO_CAPACITY)
hopper = Container(0)
mixer = Container(0)
trough = Container(0)
nitrogen_tank = Container(TANK_CAPACITY)
temperature = Container(AMBIENT_TUNNEL_TEMP)

def runnable():
    #Read all signals from PLC into the signals global dictionary
    for Signal in SIGNALS.values():
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
    print(SIGNALS['vacuum'].get_value())
    print(SIGNALS['rv_1'].get_value())
    print(SIGNALS['dv'].get_value())
    #Pull vacuum RPM and mixer RPM from the PLC
    vacuum_rpm = SIGNALS['vacuum_rpm'].get_value()
    wirecut_cpm = SIGNALS['wirecut_cpm'].get_value()
    if SIGNALS['vacuum'].get_value():
        #Solve for vacuum CFM using best fit curve function
        vacuum_CFM = CFM(vacuum_rpm)
        rate = (vacuum_CFM * MATERIAL_AIR_RATIO * AIR_DENSITY)/SECONDS_PER_MIN
        if SIGNALS['rv_1'].get_value() and SIGNALS['dv'].get_value() and flourSilo.get_weight() > 0:
            print("Siphoning flour")
            if flourSilo.get_weight() < rate:
                hopper.set_weight(hopper.get_weight() + flourSilo.get_weight())
                flourSilo.set_weight(0)
            else:
                flourSilo.set_weight(flourSilo.get_weight()-rate)
                hopper.set_weight(hopper.get_weight() + rate)
            if (hopper.get_weight() >= HOPPER_CAPACITY):
                remainder = hopper.get_weight() - HOPPER_CAPACITY
                hopper.set_weight(hopper.get_weight() - remainder)
                flourSilo.set_weight(flourSilo.get_weight() + remainder)
                print("Hopper at max capacity")
            if flourSilo.get_weight() < 0:
                flourSilo.set_weight(0)
        elif SIGNALS['rv_2'].get_value() and not SIGNALS['dv'].get_value() and sugarSilo.get_weight() > 0:
            print("Siphoning sugar")
            if sugarSilo.get_weight() < rate:
                hopper.set_weight(hopper.get_weight() + sugarSilo.get_weight())
                sugarSilo.set_weight(0)
            else:
                sugarSilo.set_weight(sugarSilo.get_weight()-rate)
                hopper.set_weight(hopper.get_weight() + rate)
            if (hopper.get_weight() >= HOPPER_CAPACITY):
                remainder = hopper.get_weight() - HOPPER_CAPACITY
                hopper.set_weight(hopper.get_weight() - remainder)
                sugarSilo.set_weight(sugarSilo.get_weight() + remainder)
                print("Hopper at max capacity")
            if sugarSilo.get_weight() < 0:
                sugarSilo.set_weight(0)
    if SIGNALS['rv_3'].get_value() and hopper.get_weight() > 0:
        rate = (GRAVITY_CFM_ESTIMATE * MATERIAL_AIR_RATIO * AIR_DENSITY)/SECONDS_PER_MIN 
        if hopper.get_weight() < rate:
            mixer.set_weight(mixer.get_weight() + hopper.get_weight())
            hopper.set_weight(0)
        else:
            hopper.set_weight(hopper.get_weight()-rate)
            mixer.set_weight(mixer.get_weight()+rate)
        if (mixer.get_weight() >= MIXER_CAPACITY):
                remainder = mixer.get_weight() - MIXER_CAPACITY
                mixer.set_weight(mixer.get_weight() - remainder)
                hopper.set_weight(hopper.get_weight() + remainder)
                print("Mixer at max capacity")
        if hopper.get_weight() < 0:
            hopper.set_weight(0)

    if SIGNALS['trough_transfer'].get_value():
        #Transfer dough from mixer to "trough"
        trough.set_weight(mixer.get_weight())
        mixer.set_weight(0)
        client.write_single_coil(SIGNALS['trough_transfer'].get_address(), False)

    if SIGNALS['trough_weight'].get_value() > 0 and SIGNALS['wirecutter'].get_value():
        #Wirecutter cuts however many rows a minute, each row has 8 cookies, each cookie is 0.3 oz, and there are 16 oz in a pound. This should give us about 0.5 pound of dough per second
        cookie_count = 8.0
        cookie_weight = 0.3
        rate = ((wirecut_cpm * cookie_count * cookie_weight)/16.0)/SECONDS_PER_MIN
        trough.set_weight(trough.get_weight() - rate)

    if SIGNALS['gv_1'].get_value():
        temp_noise = random.uniform(0.0, 1.0)
        if SIGNALS['exhaust_fan'].get_value():
            if temperature.get_weight() > -10.0:
                temperature.set_weight(temperature.get_weight() - (FREEZING_RATE + temp_noise )) 
            else:
                temperature.set_weight(-10.0 - temp_noise)
        elif not SIGNALS['exhaust_fan'].get_value():
            if temperature.get_weight() > -15.0:
                temperature.set_weight(temperature.get_weight() - (FREEZING_RATE + temp_noise )) 
            else:
                temperature.set_weight(-15.0 - temp_noise)
        rate = 1365.0/3600.0
        if nitrogen_tank.get_weight() < rate or nitrogen_tank.get_weight() < 0:
            nitrogen_tank.set_weight(0)
        else:
            nitrogen_tank.set_weight(nitrogen_tank.get_weight()-rate)
    elif not SIGNALS['gv_1'].get_value():
        temp_noise = random.uniform(0.0, 0.1)
        if temperature.get_weight() < 20.0:
            temperature.set_weight(temperature.get_weight() + (WARMING_RATE + temp_noise )) 
        else:
            temperature.set_weight(20.0 + temp_noise)
            

    # Write all relevant signals to master device
    # Flour/Sugar load cells
    lcf_weight = flourSilo.get_weight()/4.0
    lcs_weight = sugarSilo.get_weight()/4.0
    for i in range(1, 5):
        client.write_float(SIGNALS[f'lcf_{i}'].get_address(), lcf_weight)
        client.write_float(SIGNALS[f'lcs_{i}'].get_address(), lcs_weight)
    # Hopper load cell
    lch_weight = hopper.get_weight()/1.0
    client.write_float(SIGNALS['lch'].get_address(), lch_weight)

    # Mixer load cell
    lcm_weight = mixer.get_weight()/1.0
    client.write_float(SIGNALS['lcm'].get_address(), lcm_weight)

    # Trough weight
    trough_weight = trough.get_weight()/1.0
    client.write_float(SIGNALS['trough_weight'].get_address(), trough_weight)

    # Nitrogen volume
    nitrogen = nitrogen_tank.get_weight()/1.0
    client.write_float(SIGNALS['nitrogen_volume'].get_address(), nitrogen)
    
    # Temperature
    temp = temperature.get_weight()/1.0
    client.write_float(SIGNALS['tunnel_temp'].get_address(), temp)
    client.close()

#BEGIN SIMULATION
running = True
while(running):
    runnable()