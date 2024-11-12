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

    #Size and sensor mutators
    def set_size(self, new_size):
        self.size = new_size
    
    #Add sensor to sensor list
    def add_sensor(self, new_sensor):
        self.sensors.append(new_sensor)

    def get_size(self):
        return self.size
    
    def get_sensors(self):
        return self.sensors