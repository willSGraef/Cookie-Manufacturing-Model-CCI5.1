#Date: 11/12/2024
#Author: William Graef, wgraef@uark.edu
#RIOT LAB, UNIVERSITY OF ARKANSAS

#Class description: The main class includes the handling of all incoming data and sends all calculated data to the PLC
#The main class also binds together each motor and sensor object to match the visualized model, as well as the virtualized containers

#Import dependencies
import logging

from pyModbusTCP.client import ModbusClient
#-----------------------------
#Initialize modbus client and logging
#-----------------------------


#Set debug level for pyModbusTCP client to see frame changes, basically shows us all connection logs
logging.basicConfig()
logging.getLogger('pyModbusTCP.client').setLevel(logging.DEBUG)

#Init client using IP for the PLC acting as the 'hostname' and the port being 502 (Basic modbus communication port)
#Enable auto open and auto close for our port so that it opens and closes for each invidiual request

client = ModbusClient(host = "localhost", port = 502, auto_open= True, auto_close= True)

#-----------------------------
#Begin function definitions
#-----------------------------

#TO DO: Add relevant motors and containers for each section of the model; resource handling, production, shipping handling