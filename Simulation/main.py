#Date: 7/11/2025
#Author: William Graef, wgraef@uark.edu
#RIOT LAB, UNIVERSITY OF ARKANSAS

from time import sleep
import signal
from core.engine import SimulationEngine

running = True

def handle_sigterm(signum, frame):
    global running
    print("SIGTERM received, stopping simulation...")
    running = False

signal.signal(signal.SIGTERM, handle_sigterm)

if __name__ == "__main__":
    engine = SimulationEngine()
    print("Simulation started.")
    try:
        while running:
            engine.step()
            sleep(1)
    except Exception as e:
        print(f"Error: {e}")
    finally:
        print("Simulation stopped.")
