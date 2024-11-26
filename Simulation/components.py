#Date: 11/12/2024
#Author: William Graef, wgraef@uark.edu
#RIOT LAB, UNIVERSITY OF ARKANSAS

#Class description: Main class file for containers used in the model, silos, hoppers, etc.
#There are sensors associated with each container which are also defined in this file

class container():

    #Consturctor initializes size of the container as well as the list of sensors attached
    def __init__(self, size, sensors):
        self.size = size
        self.sensors = sensors

    #--Size and sensor mutators
    def set_size(self, new_size):
        self.size = new_size
    
    #Add sensor to sensor list
    def add_sensor(self, new_sensor):
        self.sensors.append(new_sensor)

    def get_size(self):
        return self.size
    
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
    
    def get_value(self):
        return self.value
    
#Class description: Class file for the motor class, currently just has speed as a parameter for simplicity sake

class motor:

    #Constructor initializes motor velocity at 0, implementation for further parameters will go here
    def __init__(self):
        self.velocity = 0

    #Velocity mutators:
    def set_velocity(self, new_velocity):
        self.velocity = new_velocity

    def get_velocity(self):
        return self.velocity