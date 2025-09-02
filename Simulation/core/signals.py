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
        self.reset_value = None

    def set_value(self, new_value):
        if self.value == None:
            self.reset_value = new_value
        self.value = new_value

    def get_value(self):
        return self.value
    
    def get_address(self):
        return self.address

    def get_name(self):
        return self.name

# Signal variables

# Boolean signals
rv_1 = Signal("rv_1", 700)
rv_2 = Signal("rv_2", 701)
rv_3 = Signal("rv_3", 702)
dv = Signal("dv", 703)
vacuum = Signal("vacuum", 704)
mixer = Signal("mixer", 705)
flour_alarm = Signal("flour_alarm", 706)
sugar_alarm = Signal("sugar_alarm", 707)
hopper_alarm = Signal("hopper_alarm", 708)
vacuum_alarm = Signal("vacuum_alarm", 709)
wirecutter = Signal("wirecutter", 711)
vacuum_manual = Signal("vacuum_manual", 710)
trough_transfer = Signal("trough_transfer", 713)
conveyor = Signal("conveyor", 716)
paper_cutter = Signal("paper_cutter", 717)
gv_1 = Signal("gv_1", 718)
fan_1 = Signal("fan_1", 719)
fan_2 = Signal("fan_2", 720)
fan_3 = Signal("fan_3", 721)
exhaust_fan = Signal("exhaust_fan", 722)
box_maker = Signal("box_maker", 723)
taper = Signal("taper", 724)
palletizer = Signal("palletizer", 726)
palletizer_grabbing = Signal("palletizer_grabbing", 727)
wrapper = Signal("wrapper", 728)
wrapper_conveying = Signal("wrapper_conveying", 729)
conveyor_1 = Signal("conveyor_1", 730)
conveyor_2 = Signal("conveyor_2", 731)
conveyor_3 = Signal("conveyor_3", 732)
conveyor_4 = Signal("conveyor_4", 733)
conveyor_5 = Signal("conveyor_5", 734)
ps_1 = Signal("ps_1", 735)
ps_2 = Signal("ps_2", 736)
ps_3 = Signal("ps_3", 737)
ps_4 = Signal("ps_4", 738)
ps_5 = Signal("ps_5", 739)
ps_6 = Signal("ps_6", 740)
bagger = Signal("bagger", 741)
wrapper_wrapping = Signal("wrapper_wrapping", 742)
boxing = Signal("boxing", 743)
taping = Signal("taping", 744)
conveying_1 = Signal("conveying_1", 745)
conveying_2 = Signal("conveying_2", 746)
conveying_3 = Signal("conveying_3", 747)
conveying_4 = Signal("conveying_4", 748)
conveying_5 = Signal("conveying_5", 749)
palletizer_moving = Signal("palletizer_moving", 750)
bagging = Signal("bagging", 751)
roe_1 = Signal("roe_1", 752)
roe_2 = Signal("roe_2", 753)
roe_3 = Signal("roe_3", 754)
reset = Signal("reset", 1000)

# Integer signals
vacuum_rpm = Signal("vacuum_rpm", 1001)
mixer_rpm = Signal("mixer_rpm", 1003) 
wirecut_cpm = Signal("wirecut_cpm", 1006)
papercut_cpm = Signal("papercut_cpm", 1010)
conveyor_fpm = Signal("conveyor_fpm", 1007)
palletizer_target_rotation = Signal("palletizer_target_rotation", 1012)
box_count = Signal("box_count", 1013)

# Float signals
lcs_1 = Signal("lcs_1", 100)
lcs_2 = Signal("lcs_2", 102)
lcs_3 = Signal("lcs_3", 104)
lcs_4 = Signal("lcs_4", 106)
lcf_1 = Signal("lcf_1", 108)
lcf_2 = Signal("lcf_2", 110)
lcf_3 = Signal("lcf_3", 112)
lcf_4 = Signal("lcf_4", 114)
lch = Signal("lch", 98)
lcm = Signal("lcm", 96)
trough_weight = Signal("trough_weight", 94)
flour_weight = Signal("flour_weight", 116)
sugar_weight = Signal("sugar_weight", 218)
tunnel_temp = Signal("tunnel_temp", 302)
nitrogen_volume = Signal("nitrogen_volume", 300)

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