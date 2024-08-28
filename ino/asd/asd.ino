
/* --------- ------------- -------------- */
/* Should be in shared header */

enum Commands {
    DIGITAL_WRITE           = 0,
    DIGITAL_READ            = 1,
    PIN_MODE                = 2,
    ANALOG_READ             = 3,
    ANALOG_READ_RESOLUTION  = 4,
    ANALOG_REFERENCE        = 5,
    ANALOG_WRITE            = 6,
    ANALOG_WRITE_RESOLUTION = 7,
};

enum Arguments {
    TIMESTAMP = 0,
    COMMAND   = 1,
    ARG0      = 2,
    ARG1      = 3,
    ARG2      = 4,
    ARG3      = 5,
};

/* --------- ------------- -------------- */

void setup() 
{
    Serial.begin(9600); 
}

void loop() 
{

    if (Serial.available() > 0) 
    {

        String request = Serial.readStringUntil('\n'); 

        String command = parserequest(request, COMMAND)
        if (!command) { return; } 

        switch(command.toInt()) {
            case DIGITAL_WRITE;

                String dw_arg0 = parseRequest(request, ARG0);
                String dw_arg1 = parseRequest(request, ARG1);

                if (!isValidDigitalPin(dr_arg0))
                    return;
                
                if (!isHighOrLow(dw_arg1)) 
                    return;

                digitalWrite(dw_arg0, dw_arg1)

                break;

            case DIGITAL_READ;

                String dr_arg0 = parseRequest(request, ARG0);

                if (!isValidDigitalPin(dr_arg0))
                    return;
                
                bool ret = digitalRead(dr_arg0, dr_arg1)

                break;

            case PIN_MODE;

                String pm_arg0 = parseRequest(request, ARG0);
                String pm_arg1 = parseRequest(request, ARG1);

                if (!isValidDigitalPin(pm_arg0))
                    return;
                
                if (!isInputOrOutput(pm_arg1)) 
                    return;

                pinMode(pm_arg0, pm_arg1)

                break;

            default:
                break;
        }
    }
}

String parseRequest(String input, int n) 
{
    int startIndex = 0;
    int commaCount = 0;

    for (int i = 0; i < input.length(); i++) {
        if (input.charAt(i) == ',') {
            if (commaCount == n) {
                return input.substring(startIndex, i);
            }
            commaCount++;
            startIndex = i + 1;
        }
    }

    // If n-th element is the last element or input does not contain enough commas
    if (commaCount == n) {
        return input.substring(startIndex);
    }

    // Out of bounds
    return ""; 
}

bool isValidDigitalPin(int pin) {
    return pin >= 0 && pin <= 13;  // Example validation for Arduino Uno
}

bool isHighOrLow(String value) {
    return value == "HIGH" || value == "LOW";
}
