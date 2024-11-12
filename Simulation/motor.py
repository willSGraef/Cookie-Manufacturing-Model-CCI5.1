#Date: 11/12/2024
#Author: William Graef, wgraef@uark.edu
#RIOT LAB, UNIVERSITY OF ARKANSAS

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

