#Date: 11/12/2024
#Author: William Graef, wgraef@uark.edu
#RIOT LAB, UNIVERSITY OF ARKANSAS

#Class description: Class for containers used in the model, silos, hoppers, etc.
#There are sensors associated with each container which are also defined in this file

class container():

    #Consturctor initializes weight of the container as well as the list of sensors attached
    def __init__(self, weight, sensors):
        self.weight = weight
        self.sensors = sensors

    #--weight and sensor mutators
    def set_weight(self, new_weight):
        self.weight = new_weight
    
    #Add sensor to sensor list
    def add_sensor(self, new_sensor):
        self.sensors.append(new_sensor)

    def get_weight(self):
        return self.weight
    
    def get_sensors(self):
        return self.sensors
    
class sensor():
    
    #Constructor initializes the limit associated with the sensor
    def __init__(self, limit):
        self.active = False
        self.value = 0
        self.limit = limit
    
    #Check limit function checks to see if the current value matches the limit, if it does switch sensor on
    def check_limit(self):
        if self.limit == self.value:
            self.active = True
        else:
            self.active = False
    
    #--Value mutators
    def set_value(self, new_value):
        self.value = new_value

    def up_value(self, increment):
        self.value += increment
    
    def get_value(self):
        return self.value
    
#Class description: Class for the motor: currently has voltage, rpm, cfm, and on_off parameters

class motor:

    #Constructor initializes motor velocity at 0, implementation for further parameters will go here
    def __init__(self):
        self.active = False
        self.velocity = 0

    #Velocity mutator:
    def set_velocity(self, new_velocity):
        self.velocity = new_velocity

    #Velocity getter:
    def get_velocity(self):
        return self.velocity
    
#Class description: Class for the different signals: currently has data_type, value, and address

class signal:
    #Constructor initializes data_type, value, and address based on paramters passed during construction
    def __init__(self, value, address):
        self.data_type = type(value)
        self.value = value
        self.address = address

    #Value mutator:
    def set_velocity(self, new_velocity):
        self.velocity = new_velocity

    #Getters:
    def get_velocity(self):
        return self.velocity
    
    def get_value(self):
        return self.value
    
    def get_address(self):
        return self.address