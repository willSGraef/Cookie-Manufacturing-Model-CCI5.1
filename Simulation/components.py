#Date: 5/30/2025
#Author: William Graef, wgraef@uark.edu
#RIOT LAB, UNIVERSITY OF ARKANSAS

#Class description: Class for containers used in the model, silos, hoppers, etc.
#There are sensors associated with each container which are also defined in this file

class Container:

    #Consturctor initializes weight of the container as well as the list of sensors attached
    def __init__(self, weight = 0):
        self.weight = weight

    #--weight and sensor mutators
    def set_weight(self, new_weight):
        self.weight = new_weight

    def get_weight(self):
        return self.weight
    
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