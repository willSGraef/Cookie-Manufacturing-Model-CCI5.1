#Date: 7/11/2025
#Author: William Graef, wgraef@uark.edu
#RIOT LAB, UNIVERSITY OF ARKANSAS

#Class description: The main class includes the handling of all incoming data and sends all calculated data to the PLC
#The main class also binds together each sensor object to match the visualized model, as well as the virtualized Containers

#Import dependencies
import signal
import struct
import time
import random
from pyModbusTCP.client import ModbusClient
from pyModbusTCP.utils import (decode_ieee, word_list_to_long)
from components import Signal

#-----------------------------
#Handle service termination
#-----------------------------

running = True

def handle_sigterm(signum, frame):
    global running
    print("SIGTERM received, stopping simulation...")
    running = False

signal.signal(signal.SIGTERM, handle_sigterm)

#-----------------------------
#Initialize modbus client
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
        # Write the registers to the specified address
        return self.write_multiple_registers(address, b16_l)

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
    "fan_1" : Signal(False, 719),
    "fan_2" : Signal(False, 720),
    "fan_3" : Signal(False, 721),
    "exhaust_fan" : Signal(False, 722),
    "conveyor" : Signal(False, 716),
    "paper_cutter" : Signal(False, 717),
    "box_maker" : Signal(False, 723),
    "taper" : Signal(False, 724),
    "palletizer" : Signal(False, 726),
    "palletizer_grabbing" : Signal(False, 727),
    "wrapper_conveyor" : Signal(False, 729),
    "conveyor_1" : Signal(False, 730),
    "conveyor_2" : Signal(False, 731),
    "conveyor_3" : Signal(False, 732),
    "conveyor_4" : Signal(False, 733),
    "conveyor_5" : Signal(False, 734),
    "ps_1" : Signal(False, 735),
    "ps_2" : Signal(False, 736),
    "ps_3" : Signal(False, 737),
    "ps_4" : Signal(False, 738),
    "ps_5" : Signal(False, 739),
    "ps_6" : Signal(False, 740),
    "bagger" : Signal(False, 741),
    "wrapper_wrapping" : Signal(False, 742),
    "boxing" : Signal(False, 743),
    "reset" : Signal(False, 1000),

    #Initialize all analog signals to their default values
    "vacuum_rpm" : Signal(0, 1001),
    "mixer_rpm" : Signal(0, 1003), 
    "wirecut_cpm" : Signal(0, 1006),
    "conveyor_fpm" : Signal(0, 1007),
    "palletizer_position" : Signal(0, 1012),

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

flourSilo = SILO_CAPACITY
sugarSilo = SILO_CAPACITY
hopper = 0
mixer = 0
trough = 0
nitrogen_tank = TANK_CAPACITY
temperature = AMBIENT_TUNNEL_TEMP

#Counters
box_counter = 0
c1_counter = 0
c2_counter = 0
c3_counter = 0
c4_counter = 0
ps1_counter = 0
ps2_counter = 0
ps3_coutner = 0

palletizer_counter = 0
wrapper_counter = 0

#State variable
current_state = 0

#Reset system function

def reset_system():
    global flourSilo, sugarSilo, hopper, mixer, trough, nitrogen_tank, temperature
    # Reset container weights to their initial values
    flourSilo = SILO_CAPACITY
    sugarSilo = SILO_CAPACITY
    hopper = 0
    mixer = 0
    trough = 0
    nitrogen_tank = TANK_CAPACITY
    # Reset temperature to ambient temp
    temperature = AMBIENT_TUNNEL_TEMP

    # Reset boolean signals
    for signal in SIGNALS.values():
        if isinstance(signal.get_value(), bool):
            client.write_single_coil(signal.get_address(), False)

def transfer_material(source, dest, rate, capacity):
    if source < rate:
        dest += source
        source = 0
    else:
        source -= rate
        dest += rate
    if (dest >= capacity):
        remainder = dest - capacity
        dest -= remainder
        source += remainder
    if source < 0:
        source = 0
    
    return source, dest

def runnable():
    global flourSilo, sugarSilo, hopper, mixer, trough, nitrogen_tank, temperature, box_counter, c1_counter, c2_counter, c3_counter, c4_counter, ps1_counter, ps2_counter, ps3_coutner, ps4_counter, palletizer_counter, wrapper_counter
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
    #Check to see if simulation needs to be reset
    if SIGNALS['reset'].get_value():
        reset_system()
        client.write_single_coil(SIGNALS["reset"].get_address(), False)
    #Check to see if vacuum is on, if it is start pulling
    #Pull vacuum RPM and mixer RPM from the PLC
    vacuum_rpm = SIGNALS['vacuum_rpm'].get_value()
    wirecut_cpm = SIGNALS['wirecut_cpm'].get_value()
    if SIGNALS['vacuum'].get_value():
        #Solve for vacuum CFM using best fit curve function
        vacuum_CFM = CFM(vacuum_rpm)
        rate = (vacuum_CFM * MATERIAL_AIR_RATIO * AIR_DENSITY)/SECONDS_PER_MIN
        if SIGNALS['rv_1'].get_value() and SIGNALS['dv'].get_value() and flourSilo > 0:
            flourSilo, hopper = transfer_material(flourSilo, hopper, rate, HOPPER_CAPACITY)
        elif SIGNALS['rv_2'].get_value() and not SIGNALS['dv'].get_value() and sugarSilo > 0:
            sugarSilo, hopper = transfer_material(sugarSilo, hopper, rate, HOPPER_CAPACITY)
    if SIGNALS['rv_3'].get_value() and hopper > 0:
        rate = (GRAVITY_CFM_ESTIMATE * MATERIAL_AIR_RATIO * AIR_DENSITY)/SECONDS_PER_MIN 
        hopper, mixer = transfer_material(hopper, mixer, rate, MIXER_CAPACITY)

    if SIGNALS['trough_transfer'].get_value():
        #Transfer dough from mixer to "trough"
        trough = mixer
        mixer = 0
        client.write_single_coil(SIGNALS['trough_transfer'].get_address(), False)

    if SIGNALS['trough_weight'].get_value() > 0 and SIGNALS['wirecutter'].get_value():
        #Wirecutter cuts however many rows a minute, each row has 8 cookies, each cookie is 0.3 oz, and there are 16 oz in a pound. This should give us about 0.5 pound of dough per second
        cookie_count = 8.0
        cookie_weight = 0.3
        rate = ((wirecut_cpm * cookie_count * cookie_weight)/16.0)/SECONDS_PER_MIN
        trough -= rate

    if SIGNALS['gv_1'].get_value():
        temp_noise = random.uniform(0.0, 1.0)
        if SIGNALS['exhaust_fan'].get_value():
            if temperature > -10.0:
                temperature -= FREEZING_RATE + temp_noise
            else:
                temperature = (-10.0 - temp_noise)
        elif not SIGNALS['exhaust_fan'].get_value():
            if temperature > -140.0:
                temperature -= FREEZING_RATE + temp_noise
            else:
                temperature = (-140.0 - temp_noise)
        rate = 1365.0/3600.0
        if nitrogen_tank < rate or nitrogen_tank < 0:
            nitrogen_tank = 0
        else:
            nitrogen_tank -= rate
    elif not SIGNALS['gv_1'].get_value():
        temp_noise = random.uniform(0.0, 0.1)
        if temperature < 20.0:
            rate = WARMING_RATE
            #Increase the warming rate based on how many circulating fans are on
            for i in range(1, 4):
                if SIGNALS[f'fan_{i}'].get_value():
                    rate += WARMING_RATE
            temperature += (rate + temp_noise)
        else:
            temperature = (20.0 + temp_noise)
    
    #Move to openPLC
    if SIGNALS['boxing'].get_value() and SIGNALS['box_maker'].get_value():
        if box_counter >= 2:
            box_counter = 0
            client.write_single_coil(SIGNALS['boxing'].get_address(), False)
        box_counter += 1

    if SIGNALS['conveyor_1'].get_value():
        if c1_counter >= 3:
            c1_counter = 0
            client.write_single_coil(SIGNALS['conveyor_1'].get_address(), False)
            client.write_single_coil(SIGNALS['ps_1'].get_address(), True)
        c1_counter += 1
    
    #Move to openPLC
    if SIGNALS['ps_1'].get_value() and SIGNALS['bagger'].get_value():
        if ps1_counter >= 1:
            ps1_counter = 0
            client.write_single_coil(SIGNALS['ps_1'].get_address(), False)
        ps1_counter += 1
    
    if SIGNALS['conveyor_2'].get_value():
        if c2_counter >= 3:
            c1_counter = 0
            client.write_single_coil(SIGNALS['conveyor_2'].get_address(), False)
            client.write_single_coil(SIGNALS['ps_2'].get_address(), True)
        c2_counter += 1

    if SIGNALS['ps_2'].get_value():
        if ps2_counter >= 4:
            ps2_counter = 0
            client.write_single_coil(SIGNALS['ps_2'].get_address(), False)
            client.write_single_coil(SIGNALS['ps_3'].get_address(), True)
        ps2_counter += 1
    
    if SIGNALS['conveyor_3'].get_value():
        if c3_counter >= 3:
            c3_counter = 0
            client.write_single_coil(SIGNALS['conveyor_3'].get_address(), False)
            client.write_single_coil(SIGNALS['ps_4'].get_address(), True)
        c3_counter += 1
    
    if SIGNALS['ps_4'].get_value() and SIGNALS['taper'].get_value():
        if ps4_counter >= 2:
            ps4_counter = 0
            client.write_single_coil(SIGNALS['ps_4'].get_address(), False)
        ps4_counter += 1

    if SIGNALS['conveyor_4'].get_value():
        if c4_counter >= 3:
            c4_counter = 0
            client.write_single_coil(SIGNALS['conveyor_4'].get_address(), False)
            client.write_single_coil(SIGNALS['ps_5'].get_address(), True)
        c4_counter += 1
    
    #Move to openPLC
    if SIGNALS['palletizer'].get_value():
        if SIGNALS['ps_5'].get_value():
            if SIGNALS['palletizer_position'].get_value() == 0:
                if palletizer_counter >= 1:
                    palletizer_counter = 0
                    client.write_single_coil(SIGNALS['palletizer_position'].get_address(), 1)
                    client.write_single_coil(SIGNALS['palletizer_grabbing'].get_address(), True)
                    client.write_single_coil(SIGNALS['ps_5'].get_address(), False)
                palletizer_counter += 1
            if SIGNALS['palletizer_position'].get_value() == 2:
                if palletizer_counter >= 2:
                    palletizer_counter = 0
                    client.write_single_coil(SIGNALS['palletizer_position'].get_address(), 1)
                    client.write_single_coil(SIGNALS['palletizer_grabbing'].get_address(), True)
                    client.write_single_coil(SIGNALS['ps_5'].get_address(), False)
                palletizer_counter += 1
        else:
            if SIGNALS['palletizer_grabbing'].get_value():
                if SIGNALS['palletizer_position'].get_value() == 2:
                    if palletizer_counter >= 1:
                        palletizer_counter = 0
                        client.write_single_coil(SIGNALS['palletizer_grabbing'].get_address(), False)
                        stack_counter += 1
                    palletizer_counter += 1
                if SIGNALS['palletizer_position'].get_value() == 1:
                    if palletizer_counter >= 1:
                        palletizer_counter = 0
                        client.write_single_coil(SIGNALS['palletizer_position'].get_address(), 0)
                    palletizer_counter += 1
                if SIGNALS['palletizer_position'].get_value() == 0 and SIGNALS['wrapper_wrapping'].get_value() == False and SIGNALS['wrapper_conveyor'].get_value() == False:
                    if palletizer_counter >= 1:
                        palletizer_counter = 0
                        client.write_single_coil(SIGNALS['palletizer_position'].get_address(), 2)
                    palletizer_counter += 1
            else:
                if SIGNALS['palletizer_position'].get_value() == 1:
                    if palletizer_counter >= 1:
                        palletizer_counter = 0
                        client.write_single_coil(SIGNALS['palletizer_position'].get_address(), 0)
                    palletizer_counter += 1
                if SIGNALS['palletizer_position'].get_value() == 2:
                    if palletizer_counter >= 1:
                        palletizer_counter = 0
                        client.write_single_coil(SIGNALS['palletizer_position'].get_address(), 0)
                    palletizer_counter += 1
    
    #Move to openPLC
    if SIGNALS['wrapper_wrapping'].get_value():
        if wrapper_counter >= 3:
            wrapper_counter = 0
            client.write_single_coil(SIGNALS['wrapper_wrapping'].get_address(), False)
            client.write_single_coil(SIGNALS['wrapper_conveyor'].get_address(), True)
        wrapper_counter += 1
    
    if SIGNALS['wrapper_conveyor'].get_value():
        if wrapper_counter >= 1:
            wrapper_counter = 0
            client.write_single_coil(SIGNALS['wrapper_conveyor'].get_address(), False)
            client.write_single_coil(SIGNALS['ps_6'].get_address(), True)
        wrapper_counter += 1

    # Write all relevant signals to master device
    # Flour/Sugar load cells
    lcf_weight = flourSilo/4.0
    lcs_weight = sugarSilo/4.0
    for i in range(1, 5):
        client.write_float(SIGNALS[f'lcf_{i}'].get_address(), lcf_weight)
        client.write_float(SIGNALS[f'lcs_{i}'].get_address(), lcs_weight)
    # Hopper load cell
    lch_weight = hopper/1.0
    client.write_float(SIGNALS['lch'].get_address(), lch_weight)

    # Mixer load cell
    lcm_weight = mixer/1.0
    client.write_float(SIGNALS['lcm'].get_address(), lcm_weight)

    # Trough weight
    trough_weight = trough/1.0
    client.write_float(SIGNALS['trough_weight'].get_address(), trough_weight)

    # Nitrogen volume
    nitrogen = nitrogen_tank/1.0
    client.write_float(SIGNALS['nitrogen_volume'].get_address(), nitrogen)
    
    # Temperature
    temp = temperature/1.0
    client.write_float(SIGNALS['tunnel_temp'].get_address(), temp)
    client.close()
    time.sleep(1)

#BEGIN SIMULATION
if __name__ == "__main__":
    print("Simulation started.")
    try:
        while running:
            runnable()
    except Exception as e:
        print(f"Error: {e}")
    finally:
        print("Simulation stopped.")
