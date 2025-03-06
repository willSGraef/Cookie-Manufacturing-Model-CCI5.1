#Date: 11/12/2024
#Author: William Graef, wgraef@uark.edu
#RIOT LAB, UNIVERSITY OF ARKANSAS

#Class description: The main class includes the handling of all incoming data and sends all calculated data to the PLC
#The main class also binds together each motor and sensor object to match the visualized model, as well as the virtualized containers

#Import dependencies
import logging

import json

from pyModbusTCP.client import ModbusClient

from pyModbusTCP.utils import (decode_ieee, encode_ieee, long_list_to_word, word_list_to_long)

from components import container, motor, sensor, signal
#-----------------------------
#Initialize modbus client and logging
#-----------------------------

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
        #Write float(s) with write multiple registers.
        b32_l = [encode_ieee(f) for f in floats_list]
        b16_l = long_list_to_word(b32_l)
        return self.write_multiple_registers(address, b16_l)

#Set debug level for pyModbusTCP client to see frame changes, basically shows us all connection logs
logging.basicConfig()
logging.getLogger('pyModbusTCP.client').setLevel(logging.DEBUG)

#Init client using IP for the PLC acting as the 'hostname' and the port being 502 (Basic modbus communication port)
#Enable auto open and auto close for our port so that it opens and closes for each invidiual request

client = FloatModbusClient(host = "localhost", port = 502, auto_open= True, auto_close= True)

#Initialize all discrete signals to their default values [STATE, ADDRESS]:

signals = {
    "start_recipe" : signal(False, 900), #BOOL ...
    "rv_1" : signal(False, 700),
    "rv_2" : signal(False, 701),
    "rv_3" : signal(False, 702),
    "dv" : signal(False, 703),
    "vacuum" : signal(False, 704),
    "mixer" : signal(False, 705), 
    "hopper_alarm" : signal(False, 600), 
    "flour_alarm" : signal(False, 601), 
    "sugar_alarm" : signal(False, 602), 

    #Initialize all analog signals to their default values
    "vacuum_config" : signal(0, 1000), #INT ...
    "mixer_config" : signal(0, 1001), 
    "rv_1_config" : signal(0, 1002),
    "rv_2_config" : signal(0, 1002),
    "rv_3_config" : signal(0, 1002),

    #Load cells for sugar, for REAL values make sure there
    #is enough room for both 16 bit addresses
    "lcs_1" : signal(0.000, 100), #REAL ...
    "lcs_2" : signal(0.000, 102),
    "lcs_3" : signal(0.000, 104),
    "lcs_4" : signal(0.000, 106),

    #Load cells for flour
    "lcf_1" : signal(0.000, 108),
    "lcf_2" : signal(0.000, 110),
    "lcf_3" : signal(0.000, 112),
    "lcf_4" : signal(0.000, 114),

    #Load cell for hopper
    "lch" : signal(0.000, 116)
}

#-----------------------------
# RESOURCE HANDLING
#-----------------------------

#Container and LI definitions, all container weights are in pounds
#LC - Load Cell

silo_weight = 10000.0

#Flour Silo
flourSiloLCs = {
    "lcf_1" : sensor(250),
    "lcf_2" : sensor(250),
    "lcf_3" : sensor(250),
    "lcf_4" : sensor(250)
}

flourSilo = container(silo_weight, flourSiloLCs)

#Sugar Silo

sugarSiloLCs = {
    "lcs_1" : sensor(250),
    "lcs_2" : sensor(250),
    "lcs_3" : sensor(250),
    "lcs_4" : sensor(250)
}

sugarSilo = container(silo_weight, sugarSiloLCs)

hopper = container(400, {sensor(40)})

#Motor and RV definitions
#RV: Rotary Valve

flourRV = motor()
sugarRV = motor()
hopperRV = motor()

diverterValve = 0
topValve = motor()

fan = motor()
mixer = motor()

#BEGIN SIMULATION
running = True
while (running):
    #Check to see if there is old sim data
    with open("sim_data.json", 'r') as read_obj:
        first_char = read_obj.read(1)
    if not first_char:
        #Data file is empty
        print("Starting new simulation...")
        with open("sim_data.json", "w") as file:
            for s in signals.values:
                json.dump(s.__dict__, file)
    else:
        #Data file is not empty
        new_sim = input("Would you like to start a new simulation?\n")
        if new_sim:
            print("Starting new simulation...")
            #Write current data to the data file and clear old data
            with open("sim_data.json", "w") as file:
                json.dump({}, file)
                for s in signals.values:
                    json.dump(s.__dict__, file)
        else:
            #Load existing data from the data file and clear old data
            with open("sim_data.json", "r") as file:
                json.dump({}, file)
                for s in signals.values:
                    signals = json.load(file)    