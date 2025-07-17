#Date: 5/30/2025
#Author: William Graef, wgraef@uark.edu
#RIOT LAB, UNIVERSITY OF ARKANSAS

#Class description: Class for the different signals: currently has data_type, value, and address

class Signal:
    #Constructor initializes data_type, value, and address based on paramters passed during construction
    def __init__(self, value, address):
        self.value = value
        self.address = address

    #Value mutator:
    def set_value(self, new_value):
        self.value = new_value

    #Getters:
    def get_value(self):
        return self.value
    
    def get_address(self):
        return self.address