#Date: 9/2/2025
#Author: William Graef, wgraef@uark.edu
#RIOT LAB, UNIVERSITY OF ARKANSAS

#Contains all the models signals, their values, and addresses

class Signal:
    def __init__(self, name, address, value = None):
        self.name = name
        self.address = address
        self.value = value
        self.latest_published_value = None 
        self.reset_value = value

    def set_value(self, new_value):
        self.value = new_value

    def get_value(self):
        return self.value
    
    def get_address(self):
        return self.address

    def get_name(self):
        return self.name

# Signal variables

# Boolean signals
rv_1 = Signal("rv_1", 700, False)
rv_2 = Signal("rv_2", 701, False)
rv_3 = Signal("rv_3", 702, False)
dv = Signal("dv", 703, False)
vacuum = Signal("vacuum", 704, False)
mixer = Signal("mixer", 705, False)
flour_alarm = Signal("flour_alarm", 706, False)
sugar_alarm = Signal("sugar_alarm", 707, False)
hopper_alarm = Signal("hopper_alarm", 708, False)
vacuum_alarm = Signal("vacuum_alarm", 709, False)
wirecutter = Signal("wirecutter", 711, False)
vacuum_manual = Signal("vacuum_manual", 710, False)
trough_transfer = Signal("trough_transfer", 713, False)
conveyor = Signal("conveyor", 716, False)
paper_cutter = Signal("paper_cutter", 717, False)
gv_1 = Signal("gv_1", 718, False)
fan_1 = Signal("fan_1", 719, False)
fan_2 = Signal("fan_2", 720, False)
fan_3 = Signal("fan_3", 721, False)
exhaust_fan = Signal("exhaust_fan", 722, False)
box_maker = Signal("box_maker", 723, False)
taper = Signal("taper", 724, False)
palletizer = Signal("palletizer", 726, False)
palletizer_grabbing = Signal("palletizer_grabbing", 727, False)
wrapper = Signal("wrapper", 728, False)
wrapper_conveying = Signal("wrapper_conveying", 729, False)
conveyor_1 = Signal("conveyor_1", 730, True)
conveyor_2 = Signal("conveyor_2", 731, True)
conveyor_3 = Signal("conveyor_3", 732, True)
conveyor_4 = Signal("conveyor_4", 733, True)
conveyor_5 = Signal("conveyor_5", 734, True)
ps_1 = Signal("ps_1", 735, False)
ps_2 = Signal("ps_2", 736, False)
ps_3 = Signal("ps_3", 737, False)
ps_4 = Signal("ps_4", 738, False)
ps_5 = Signal("ps_5", 739, False)
ps_6 = Signal("ps_6", 740, False)
bagger = Signal("bagger", 741, False)
wrapper_wrapping = Signal("wrapper_wrapping", 742, False)
boxing = Signal("boxing", 743, False)
taping = Signal("taping", 744, False)
conveying_1 = Signal("conveying_1", 745, False)
conveying_2 = Signal("conveying_2", 746, False)
conveying_3 = Signal("conveying_3", 747, False)
conveying_4 = Signal("conveying_4", 748, False)
conveying_5 = Signal("conveying_5", 749, False)
palletizer_moving = Signal("palletizer_moving", 750, False)
bagging = Signal("bagging", 751, False)
roe_1 = Signal("roe_1", 752, False)
roe_2 = Signal("roe_2", 753, False)
roe_3 = Signal("roe_3", 754, False)
add_ingredients = Signal("add_ingredients", 755, False)
worker_present = Signal("worker_present", 756, False)
reset = Signal("reset", 1000, False)

# Integer signals
vacuum_rpm = Signal("vacuum_rpm", 1001, 0)
mixer_rpm = Signal("mixer_rpm", 1003, 0) 
wirecut_cpm = Signal("wirecut_cpm", 1006, 0)
papercut_cpm = Signal("papercut_cpm", 1010, 0)
conveyor_fpm = Signal("conveyor_fpm", 1007, 0)
palletizer_target_rotation = Signal("palletizer_target_rotation", 1012, 180)
box_count = Signal("box_count", 1013, 0)

# Float signals
lcs_1 = Signal("lcs_1", 100, 0.0)
lcs_2 = Signal("lcs_2", 102, 0.0)
lcs_3 = Signal("lcs_3", 104, 0.0)
lcs_4 = Signal("lcs_4", 106, 0.0)
lcf_1 = Signal("lcf_1", 108, 0.0)
lcf_2 = Signal("lcf_2", 110, 0.0)
lcf_3 = Signal("lcf_3", 112, 0.0)
lcf_4 = Signal("lcf_4", 114, 0.0)
lch = Signal("lch", 98, 0.0)
lcm = Signal("lcm", 96, 0.0)
trough_weight = Signal("trough_weight", 94, 0.0)
flour_weight = Signal("flour_weight", 116, 0.0)
sugar_weight = Signal("sugar_weight", 218, 0.0)
tunnel_temp = Signal("tunnel_temp", 302, 0.0)
nitrogen_volume = Signal("nitrogen_volume", 300, 0.0)

# Signal list

SIGNALS = [
    rv_1,
    rv_2,
    rv_3,
    dv,
    vacuum,
    mixer,
    flour_alarm,
    sugar_alarm,
    hopper_alarm,
    vacuum_alarm,
    wirecutter,
    vacuum_manual,
    trough_transfer,
    conveyor,
    paper_cutter,
    gv_1,
    fan_1,
    fan_2,
    fan_3,
    exhaust_fan,
    box_maker,
    taper,
    palletizer,
    palletizer_grabbing,
    wrapper,
    wrapper_conveying,
    conveyor_1,
    conveyor_2,
    conveyor_3,
    conveyor_4,
    conveyor_5,
    ps_1,
    ps_2,
    ps_3,
    ps_4,
    ps_5,
    ps_6,
    bagger,
    wrapper_wrapping,
    boxing,
    taping,
    conveying_1,
    conveying_2,
    conveying_3,
    conveying_4,
    conveying_5,
    palletizer_moving,
    bagging,
    roe_1,
    roe_2,
    roe_3,
    add_ingredients,
    worker_present,
    reset,
    vacuum_rpm,
    mixer_rpm,
    wirecut_cpm,
    papercut_cpm,
    conveyor_fpm,
    palletizer_target_rotation,
    box_count,
    lcs_1,
    lcs_2,
    lcs_3,
    lcs_4,
    lcf_1,
    lcf_2,
    lcf_3,
    lcf_4,
    lch,
    lcm,
    trough_weight,
    flour_weight,
    sugar_weight,
    tunnel_temp,
]