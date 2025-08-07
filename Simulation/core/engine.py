from core.constants import *
import core.signals as signals
from utils.modbus_utils import FloatModbusClient
from utils.engine_utils import transfer_material, CFM
from utils.mqtt_utils import MqttClient
import random

class SimulationEngine:
    def __init__(self):
        print("Connecting to OpenPLC...")
        self.modbus_client = FloatModbusClient(host = "localhost", port = 502, auto_open= False, auto_close= True)
        self.modbus_client.open()
        print("Connected to OpenPLC successfully.")
        print("Connecting to MQTT Broker...")
        self.mqtt_client = MqttClient(signals.SIGNALS, self.modbus_client)
        self.mqtt_client.connect()
        print("Connected to MQTT Broker successfully.")
        self.flour_silo = SILO_CAPACITY
        self.sugar_silo = SILO_CAPACITY
        self.hopper = 0
        self.mixer = 0
        self.trough = 0
        self.nitrogen_tank = TANK_CAPACITY
        self.temperature = AMBIENT_TUNNEL_TEMP
        self.counters = [0,0,0]
        self.palletizer_rotation = 180
        self.packing = False
    
    def set_signal(self, signal, value):
        signal.set_value(value)
        self.mqtt_client.publish_signal(signal.get_name(), signal)
        if type(signal.get_value()) == bool:
            print(f"Setting BOOL signal {signal.get_name()} to {value} at address {signal.get_address()}")
            self.modbus_client.write_single_coil(signal.get_address(), value)
        elif isinstance(signal.get_value(), int):
            print(f"Setting INT signal {signal.get_name()} to {value} at address {signal.get_address()}")
            self.modbus_client.write_single_register(signal.get_address(), value)
        elif isinstance(signal.get_value(), float):
            print(f"Setting FLOAT signal {signal.get_name()} to {value} at address {signal.get_address()}")
            self.modbus_client.write_float(signal.get_address(), value)

    def reset(self):
        # Reset constants
        self.flour_silo = SILO_CAPACITY
        self.sugar_silo = SILO_CAPACITY
        self.hopper = 0
        self.mixer = 0
        self.trough = 0
        self.nitrogen_tank = TANK_CAPACITY
        self.temperature = AMBIENT_TUNNEL_TEMP
        self.counters = [0,0,0]
        self.palletizer_rotation = 180
        self.packing = False
        # Reset signals
        for signal in signals.SIGNALS:
            if isinstance(signal.get_value(), bool):
                self.set_signal(signal, False)

    def read_signals(self):
        for signal in signals.SIGNALS:
            # Set booleans
            if isinstance(signal.get_value(), bool):
                result = self.modbus_client.read_coils(signal.get_address())
                if result is not None:
                    signal.set_value(result[0])
            # Set integers
            elif isinstance(signal.get_value(), int):
                result = self.modbus_client.read_holding_registers(signal.get_address())
                if result and len(result) > 0:
                    signal.set_value(result[0])  # Extract first value
            # Set floats
            elif isinstance(signal.get_value(), float):
                result = self.modbus_client.read_float(signal.get_address())
                if result is not None:
                    signal.set_value(result[0])
    
    def write_signals(self):
        print("Writing signals to OpenPLC...")
        lcf_weight = self.flour_silo/4.0
        lcs_weight = self.sugar_silo/4.0

        self.set_signal(signals.lcf_1, lcf_weight)
        self.set_signal(signals.lcf_2, lcf_weight)
        self.set_signal(signals.lcf_3, lcf_weight)
        self.set_signal(signals.lcf_4, lcf_weight)

        self.set_signal(signals.lcs_1, lcs_weight)
        self.set_signal(signals.lcs_2, lcs_weight)
        self.set_signal(signals.lcs_3, lcs_weight)
        self.set_signal(signals.lcs_4, lcs_weight)

        # Hopper load cell
        self.set_signal(signals.lch, self.hopper)

        # Mixer load cell
        self.set_signal(signals.lcm, self.mixer)

        # Trough weight
        self.set_signal(signals.trough_weight, self.trough)

        # Nitrogen volume
        self.set_signal(signals.nitrogen_volume, self.nitrogen_tank)
        
        # Temperature
        self.set_signal(signals.tunnel_temp, self.temperature)

    def resource_machine(self):
        flour_silo = self.flour_silo
        sugar_silo = self.sugar_silo
        hopper = self.hopper
        mixer = self.mixer
        trough = self.trough
        
        if signals.vacuum.get_value():
            vacuum_cfm = CFM(signals.vacuum_rpm.get_value())
            rate = (vacuum_cfm * MATERIAL_AIR_RATIO * AIR_DENSITY)/SECONDS_PER_MIN
            if signals.rv_1.get_value() and signals.dv.get_value() and flour_silo > 0:
                flour_silo, hopper = transfer_material(flour_silo, hopper, rate, HOPPER_CAPACITY)
            elif signals.rv_2.get_value() and not signals.dv.get_value() and sugar_silo > 0:
                sugar_silo, hopper = transfer_material(sugar_silo, hopper, rate, HOPPER_CAPACITY)
        if signals.rv_3.get_value() and hopper > 0:
            rate = (GRAVITY_CFM_ESTIMATE * MATERIAL_AIR_RATIO * AIR_DENSITY)/SECONDS_PER_MIN 
            hopper, mixer = transfer_material(hopper, mixer, rate, MIXER_CAPACITY)

        if signals.trough_transfer.get_value():
            trough = mixer
            mixer = 0
            self.set_signal(signals.trough_transfer, False)
        
        print(flour_silo, sugar_silo, hopper, mixer, trough)
        
        self.flour_silo = flour_silo
        self.sugar_silo = sugar_silo
        self.hopper = hopper
        self.mixer = mixer
        self.trough = trough
    
    def production_machine(self):
        trough = self.trough
        temperature = self.temperature
        nitrogen_tank = self.nitrogen_tank

        if signals.trough_weight.get_value() > 0 and signals.wirecutter.get_value():
            rate = ((signals.wirecut_cpm.get_value() * COOKIE_COUNT * COOKIE_WEIGHT)/16.0)/SECONDS_PER_MIN
            trough -= rate

        if signals.gv_1.get_value():
            temp_noise = random.uniform(0.0, 1.0)
            if signals.exhaust_fan.get_value():
                if temperature > -10.0:
                    temperature -= FREEZING_RATE + temp_noise
                else:
                    temperature = (-10.0 - temp_noise)
            elif not signals.exhaust_fan.get_value():
                if temperature > -140.0:
                    temperature -= FREEZING_RATE + temp_noise
                else:
                    temperature = (-140.0 - temp_noise)
            rate = 1365.0/3600.0
            if nitrogen_tank < rate or nitrogen_tank < 0:
                nitrogen_tank = 0
            else:
                nitrogen_tank -= rate
        elif not signals.gv_1.get_value():
            temp_noise = random.uniform(0.0, 0.1)
            if temperature < 20.0:
                rate = WARMING_RATE
                #Increase the warming rate based on how many circulating fans are on
                if signals.fan_1.get_value():
                    rate += WARMING_RATE
                if signals.fan_2.get_value():
                    rate += WARMING_RATE
                if signals.fan_3.get_value():
                    rate += WARMING_RATE
                temperature += (rate + temp_noise)
            else:
                temperature = (20.0 + temp_noise)

        self.trough = trough
        self.temperature = temperature
        self.nitrogen_tank = nitrogen_tank

    def boxing_machine(self):
        counter = self.counters[0]
        if signals.conveying_2.get_value() and signals.ps_1.get_value():
                self.set_signal(signals.ps_1, False)
        if signals.conveying_2.get_value():
            if counter >= 2:
                counter = 0
                self.set_signal(signals.ps_2, True)
            else:
                counter += 1
        elif signals.bagging.get_value():
            if counter >= 1:
                counter = 0
                self.set_signal(signals.bagging, False)
            else:
                counter += 1
        elif signals.conveying_1.get_value():
            if counter >= 2:
                counter = 0
                self.set_signal(signals.ps_1, True)
            else:
                counter += 1
        elif signals.boxing.get_value():
            if counter >= 3:
                counter = 0
                self.set_signal(signals.boxing, False)
            else:
                counter += 1
        self.counters[0] = counter

    def taping_machine(self):
        counter = self.counters[1]
        if signals.ps_2.get_value() and signals.conveyor.get_value() and self.packing == False:
                self.packing = True
                self.set_signal(signals.ps_2, False)

        if signals.conveying_4.get_value() and signals.ps_4.get_value():
            self.set_signal(signals.ps_4, False)

        if signals.conveying_3.get_value() and signals.ps_3.get_value():
            self.set_signal(signals.ps_3, False)
        
        if signals.conveying_4.get_value():
            if counter >= 2:
                counter = 0
                self.set_signal(signals.ps_5, True)
            else:
                counter += 1
        elif signals.taping.get_value():
            if counter >= 1:
                counter = 0
                self.set_signal(signals.taping, False)
            else:
                counter += 1
        elif signals.conveying_3.get_value():
            if counter >= 2:
                counter = 0
                self.set_signal(signals.ps_4, True)
            else:
                counter += 1
        elif self.packing:
            if counter >= 8:
                counter = 0
                self.packing = False
                self.set_signal(signals.ps_3, True)
            else:
                counter += 1
        self.counters[1] = counter

    def palletizer_machine(self):
        palletizer_rotation = self.palletizer_rotation
        if signals.palletizer_moving.get_value():
            if signals.palletizer_target_rotation.get_value() == palletizer_rotation:
                if palletizer_rotation == 225:
                    self.set_signal(signals.roe_1, True)
                    self.set_signal(signals.roe_2, False)
                    self.set_signal(signals.roe_3, False)
                elif palletizer_rotation == 180:
                    self.set_signal(signals.roe_2, True)
                    self.set_signal(signals.roe_1, False)
                    self.set_signal(signals.roe_3, False)
                elif palletizer_rotation == 130:
                    self.set_signal(signals.roe_3, True)
                    self.set_signal(signals.roe_1, False)
                    self.set_signal(signals.roe_2, False)
                else:
                    self.set_signal(signals.roe_1, False)
                    self.set_signal(signals.roe_2, False)
                    self.set_signal(signals.roe_3, False)
            else:
                if palletizer_rotation + ARM_ROTATION_SPEED < signals.palletizer_target_rotation.get_value():
                    palletizer_rotation += ARM_ROTATION_SPEED
                    if palletizer_rotation > signals.palletizer_target_rotation.get_value():
                        palletizer_rotation = signals.palletizer_target_rotation.get_value()
                elif palletizer_rotation + ARM_ROTATION_SPEED > signals.palletizer_target_rotation.get_value():
                    palletizer_rotation -= ARM_ROTATION_SPEED
                    if palletizer_rotation < signals.palletizer_target_rotation.get_value():
                        palletizer_rotation = signals.palletizer_target_rotation.get_value()
                        
            if signals.ps_5.get_value() and palletizer_rotation == 225 and signals.palletizer_grabbing.get_value():
                    self.set_signal(signals.ps_5, False)
        self.palletizer_rotation = palletizer_rotation

    def wrapper_machine(self):
        counter = self.counters[2]
        if signals.ps_6.get_value() and random.randint(1,5) == 3: # Simulating time it takes for a forklift driver to pick up a finished pallet
                self.set_signal(signals.ps_6, False)

        if signals.conveying_5.get_value():
            if counter >= 8:
                counter = 0
                self.set_signal(signals.ps_6, True)
            else:
                counter += 1
        elif signals.wrapper_conveying.get_value():
            if counter >= 1:
                counter = 0
                self.set_signal(signals.wrapper_conveying, False)
            else:
                counter += 1
        elif signals.wrapper_wrapping.get_value():
            if counter >= 3:
                counter = 0
                self.set_signal(signals.wrapper_wrapping, False)
            else:
                counter += 1
        self.counters[2] = counter
    
    def step(self):
        self.read_signals()
        if signals.reset.get_value():
            self.reset()
            self.set_signal(signals.reset, False)    

        # Resource Allocation
        self.resource_machine()

        # Production
        self.production_machine()
        
        # Finished Good
        self.boxing_machine()
        self.taping_machine()
        self.palletizer_machine()
        self.wrapper_machine()
        
        self.write_signals()        