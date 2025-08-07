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

def CFM(rpm):
    #Use best fit curve function
    return ((-1.275*(pow(10, -7))) * pow(rpm, 2)) + ((0.7896)*rpm) - 365.43

