#define MAX_PINS 14

struct Data
{
    String header;
    String pinStr;
    char   pinModes[MAX_PINS];  // 'i' for INPUT, 'o' for OUTPUT
    char   pinStates[MAX_PINS]; // '0' for LOW, '1' for HIGH
    String message;
    String checksum;
};

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    Data   request;
    String requestStr;
    Data   response;
    String responseStr;

    if (Serial.available() > 0)
    {
        response.header   = "";
        response.pinStr   = "";
        response.message  = "";
        response.checksum = "";

        requestStr = Serial.readStringUntil('\n');
        requestStr.trim();

        if (!parseData(requestStr, request))
        {
            response.message = "Parsing failed. ";
            goto exit;
        }

        if (request.header == "arduino->pc")
        {
            response.message = "Checksum failed.";
            // goto exit;
        }

        /*if (!validateChecksum(request))
        {
            response.message += "Checksum failed.";
            goto exit;
        }*/

        // Per pin logic
        for (int pin = 0; pin < MAX_PINS; pin++)
        {
            if (request.pinModes[pin] != response.pinModes[pin])
            {
                pinMode(pin, request.pinModes[pin]);
            }

            if (request.pinModes[pin] == 'o')
            {
                digitalWrite(pin, request.pinStates[pin]);
            }

            response.pinStates[pin] = digitalRead(pin);
            response.pinModes[pin]  = request.pinModes[pin];
        }

        response.message = "OK";

    exit:

        response.header   = "arduino->pc";
        response.pinStr   = serializePinStr(response);
        response.checksum = computeChecksum(response);

        Serial.println(serializeData(response).c_str());
    }
}

bool parseData(const String& input, Data& data)
{
    // Check if the input contains exactly 3 semicolons
    int semicolonCount = 0;
    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == ';')
            semicolonCount++;
    }

    if (semicolonCount != 3)
    {
        Serial.print("Invalid input format. Expected 3 semicolons but found: ");
        Serial.println(semicolonCount);
        return false;
    }

    // Parsing main Str fields
    int headerEnd = input.indexOf(';');
    data.header   = input.substring(0, headerEnd);
    String rest   = input.substring(headerEnd + 1);

    int dataEnd      = rest.indexOf(';');
    data.pinStr      = rest.substring(0, dataEnd);
    String remainder = rest.substring(dataEnd + 1);

    int messageEnd = remainder.indexOf(';');
    data.message   = remainder.substring(0, messageEnd);
    data.checksum  = remainder.substring(messageEnd + 1);

    Serial.print("   ");
    Serial.print(data.header);
    Serial.println();
    Serial.print("   ");
    Serial.print(data.pinStr);
    Serial.println();
    Serial.print("   ");
    Serial.print(data.message);
    Serial.println();
    Serial.print("   ");
    Serial.print(data.checksum);
    Serial.println();

    int pin = 0;
    for (int cc = 0; data.pinStr[cc]; cc++)
    {
        switch (data.pinStr[cc])
        {
            case ',':
                // needs to be in its own line else will be called several times
                pin++;
                break;
            case 'o':
                data.pinModes[min(pin, MAX_PINS)] = OUTPUT;
                break;
            case 'i':
                data.pinModes[min(pin, MAX_PINS)] = INPUT;
                break;
            case 'p':
                data.pinModes[min(pin, MAX_PINS)] = INPUT_PULLUP;
                break;
            case '0':
                data.pinStates[min(pin, MAX_PINS)] = LOW;
                break;
            case '1':
                data.pinStates[min(pin, MAX_PINS)] = HIGH;
                break;
            case ';':
                break;
            default:
                Serial.print("Unexpected pin state string in request: ");
                Serial.println(data.pinStr[cc]);
                return false;
                break;
        }
    }

    if (pin + 1 != MAX_PINS)
    {
        Serial.print("Wrong number of pins in request. Found: ");
        Serial.print(pin + 1);
        Serial.print(" but expected ");
        Serial.println(MAX_PINS);
        return false;
    }

    return true;
}

String serializeData(const Data& data)
{
    String result = "";

    result += data.header + ";";
    result += data.pinStr + ";";
    result += data.message + ";";
    result += data.checksum;

    return result;
}

String serializePinStr(const Data& data)
{
    String pinStr = "";

    for (int pin = 0; pin < MAX_PINS; pin++)
    {
        switch (data.pinModes[pin])
        {
            case INPUT:
                pinStr += 'i';
                break;
            case OUTPUT:
                pinStr += 'o';
                break;
            case INPUT_PULLUP:
                pinStr += 'p';
                break;
            default:
                pinStr += '?'; // Unknown mode
                break;
        }

        switch (data.pinStates[pin])
        {
            case LOW:
                pinStr += '0';
                break;
            case HIGH:
                pinStr += '1';
                break;
            default:
                pinStr += '?'; // Unknown state
                break;
        }

        // Append a comma if it's not the last pin
        if (pin < MAX_PINS - 1)
        {
            pinStr += ',';
        }
    }

    return pinStr;
}

bool validateChecksum(Data& request)
{
    return computeChecksum(request) == request.checksum.toInt();
}

int computeChecksum(Data& response)
{
    int checksum = 0;

    for (int i = 0; i < response.header.length(); i++)
        checksum += response.header[i];

    for (int i = 0; i < response.pinStr.length(); i++)
        checksum += response.pinStr[i];

    for (int i = 0; i < response.message.length(); i++)
        checksum += response.message[i];

    return checksum % 256;
}
