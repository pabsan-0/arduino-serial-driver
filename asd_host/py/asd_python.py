import os
import ctypes
from ctypes import c_char_p, c_int

# Load the shared library from env, default to the assumption
# we're in the repository root.
lib_dirname  = os.environ.get("ASD_LIBRARY_PATH", "./asd_host/lib") 
lib_basename = "libasd.so"

try:
    lib = ctypes.CDLL(os.path.join(lib_dirname, lib_basename))
except OSError as ee:
    print(ee)
    print("\nFatal: Could not locate %s. Try setting the environment variable:" % lib_basename)
    print("$ export ASD_LIBRARY_PATH=lib/parent/dir")
    print("Aborted.")
    exit(1)

# Define enums as in the C code
HIGH = 0x1
LOW = 0x0
INPUT = 0x0
OUTPUT = 0x1
INPUT_PULLUP = 0x2

PINSTATE_HIGH = HIGH
PINSTATE_LOW = LOW

# PinMode equivalent
PINMODE_INPUT = INPUT
PINMODE_OUTPUT = OUTPUT
PINMODE_INPUT_PULLUP = INPUT_PULLUP

# Define the serial functions
lib.py_serialBegin.argtypes = [c_char_p]
lib.py_serialBegin.restype = c_int

lib.py_serialClose.restype = c_int

lib.py_digitalWrite.argtypes = [c_int, c_int]

lib.py_digitalRead.restype = c_int
lib.py_digitalRead.argtypes = [c_int]

lib.py_pinMode.argtypes = [c_int, c_int]

# Python functions that wrap the C functions
def serialBegin(port):
    return lib.py_serialBegin(port.encode('utf-8'))

def serialClose():
    return lib.py_serialClose()

def digitalWrite(pin, value):
    lib.py_digitalWrite(pin, value)

def digitalRead(pin):
    return lib.py_digitalRead(pin)

def pinMode(pin, mode):
    lib.py_pinMode(pin, mode)


if __name__ == "__main__":
    port = "/dev/ttyUSB0"  # Adjust for your system
    if serialBegin(port) == -1:
        print("Failed to open serial port")
    
    pin = 5
    
    print("Setting pinMode to OUTPUT...")
    pinMode(pin, PINMODE_OUTPUT)
    
    print("Writing HIGH to pin...")
    digitalWrite(pin, HIGH)
    
    print("Reading from pin...")
    state = digitalRead(pin)
    print(f"Pin state: {state}")
    
    serialClose()

