#! /usr/bin/env python3

import time
import sys
import os

# TODO fixme
sys.path.append(os.path.join(os.path.dirname(__file__), ".."))

from asd_host.py import *


if __name__ == "__main__":

    try:
        port = sys.argv[1] 
    except Exception:
        port = "/dev/ttyUSB0" 

    serialBegin(port)
    
    total = 3
    for curr in range(total):
        time.sleep(1)
        print(f"\rWaking arduino... {curr / total}/100%", end="")

    print()

    pin_number = 3

    print("1: Reading pin_value...");
    pin_value = digitalRead(pin_number);
    print("1: Read pin_value: %d", pin_value);

    print("2: Setting pin to HIGH...");
    digitalWrite(pin_number, HIGH);
    print("2: Set %d to HIGH", pin_number);

    print("3: Reading pin_value...");
    pin_value = digitalRead(pin_number);
    print("3: Read pin_value: %d", pin_value);

    serialClose();
