#Date: 5/30/2025
#Author: William Graef, wgraef@uark.edu
#RIOT LAB, UNIVERSITY OF ARKANSAS

#Class description: Class for the different signals: currently has data_type, value, and address

class Signal:
    def __init__(self, name, value, address):
        self.name = name
        self.value = value
        self.address = address
        self.latest_published_value = None 

    def set_value(self, new_value):
        self.value = new_value

    def get_value(self):
        return self.value
    
    def get_address(self):
        return self.address

    def get_name(self):
        return self.name

# Signal variables

rv_1 = Signal("rv_1", False, 700)
rv_2 = Signal("rv_2", False, 701)
rv_3 = Signal("rv_3", False, 702)
dv = Signal("dv", False, 703)
vacuum = Signal("vacuum", False, 704)
mixer = Signal("mixer", False, 705)
wirecutter = Signal("wirecutter", False, 711)
vacuum_manual = Signal("vacuum_manual", False, 710)
trough_transfer = Signal("trough_transfer", False, 713)
gv_1 = Signal("gv_1", False, 718)
fan_1 = Signal("fan_1", False, 719)
fan_2 = Signal("fan_2", False, 720)
fan_3 = Signal("fan_3", False, 721)
exhaust_fan = Signal("exhaust_fan", False, 722)
conveyor = Signal("conveyor", False, 716)
paper_cutter = Signal("paper_cutter", False, 717)
box_maker = Signal("box_maker", False, 723)
taper = Signal("taper", False, 724)
palletizer = Signal("palletizer", False, 726)
palletizer_grabbing = Signal("palletizer_grabbing", False, 727)
wrapper = Signal("wrapper", False, 728)
wrapper_conveying = Signal("wrapper_conveying", False, 729)
conveyor_1 = Signal("conveyor_1", False, 730)
conveyor_2 = Signal("conveyor_2", False, 731)
conveyor_3 = Signal("conveyor_3", False, 732)
conveyor_4 = Signal("conveyor_4", False, 733)
conveyor_5 = Signal("conveyor_5", False, 734)
ps_1 = Signal("ps_1", False, 735)
ps_2 = Signal("ps_2", False, 736)
ps_3 = Signal("ps_3", False, 737)
ps_4 = Signal("ps_4", False, 738)
ps_5 = Signal("ps_5", False, 739)
ps_6 = Signal("ps_6", False, 740)
bagger = Signal("bagger", False, 741)
wrapper_wrapping = Signal("wrapper_wrapping", False, 742)
boxing = Signal("boxing", False, 743)
taping = Signal("taping", False, 744)
conveying_1 = Signal("conveying_1", False, 745)
conveying_2 = Signal("conveying_2", False, 746)
conveying_3 = Signal("conveying_3", False, 747)
conveying_4 = Signal("conveying_4", False, 748)
conveying_5 = Signal("conveying_5", False, 749)
palletizer_moving = Signal("palletizer_moving", False, 750)
bagging = Signal("bagging", False, 751)
roe_1 = Signal("roe_1", False, 752)
roe_2 = Signal("roe_2", False, 753)
roe_3 = Signal("roe_3", False, 754)
reset = Signal("reset", False, 1000)

vacuum_rpm = Signal("vacuum_rpm", 0, 1001)
mixer_rpm = Signal("mixer_rpm", 0, 1003) 
wirecut_cpm = Signal("wirecut_cpm", 0, 1006)
conveyor_fpm = Signal("conveyor_fpm", 0, 1007)
palletizer_target_rotation = Signal("palletizer_target_rotation", 0, 1012)

lcs_1 = Signal("lcs_1", 0.000, 100)
lcs_2 = Signal("lcs_2", 0.000, 102)
lcs_3 = Signal("lcs_3", 0.000, 104)
lcs_4 = Signal("lcs_4", 0.000, 106)
lcf_1 = Signal("lcf_1", 0.000, 108)
lcf_2 = Signal("lcf_2", 0.000, 110)
lcf_3 = Signal("lcf_3", 0.000, 112)
lcf_4 = Signal("lcf_4", 0.000, 114)
lch = Signal("lch", 0.000, 98)
lcm = Signal("lcm", 0.000, 96)
trough_weight = Signal("trough_weight", 0.000, 94)
flour_weight = Signal("flour_weight", 0.000, 116)
sugar_weight = Signal("sugar_weight", 0.000, 218)
tunnel_temp = Signal("tunnel_temp", 0.000, 302)
nitrogen_volume = Signal("nitrogen_volume", 0.000, 300)

# Signal list

SIGNALS = [
    rv_1,
    rv_2,
    rv_3,
    dv,
    vacuum,
    mixer,
    wirecutter,
    vacuum_manual,
    trough_transfer,
    gv_1,
    fan_1,
    fan_2,
    fan_3,
    exhaust_fan,
    conveyor,
    paper_cutter,
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
    conveyor_fpm,
    palletizer_target_rotation,
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
    nitrogen_volume,
]
