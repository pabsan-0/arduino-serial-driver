#define CHECK_ARG_COUNT(nargs, expected)                            \
    if ((nargs) != (expected))                                      \
    {                                                               \
        res.messages += "Expected " + String(expected);             \
        res.messages += " argument(s), got " + String(nargs) + "."; \
        goto exit;                                                  \
    }

#define CHECK_VALID_DIGITAL_PIN(pin)                    \
    if (!isValidDigitalPin(pin))                        \
    {                                                   \
        res.messages += "Bad pin " + String(pin) + "."; \
        goto exit;                                      \
    }

struct Request
{
    String header;
    String cmd;
    String arg0;
    String arg1;
    String arg2;

    String checksum;
};

struct Response
{
    String header;
    String retval;
    String out0;
    String messages;

    String checksum;
};

void setup()
{
    Serial.begin(9600);
}

void loop()
{

    if (Serial.available() > 0)
    {

        String   reqStr;
        Request  req;
        Response res;
        int      nargs;

        res.header   = "arduino->pc";
        res.retval   = "true";
        res.out0     = "";
        res.messages = "";
        res.checksum = "";

        reqStr = Serial.readStringUntil('\n');
        reqStr.trim();

        if (!parseData(requestStr, req, nargs))
        {
            goto exit;
        }

        if (request.header == "arduino->pc")
        {
            goto exit;
        }

        /*
        if (!validateChecksum(request))
        {
            response.message += "Checksum failed.";
            goto exit;
        }
        */

        int  pin  = 0;
        bool val  = false;
        int  mode = false;
        if (req.command == "digitalRead")
        {
            CHECK_ARG_COUNT(nargs, 1);
            CHECK_VALID_DIGITAL_PIN(req.arg0)

            pin      = int(req.arg0);
            res.out0 = digitalRead(pin) ? "true" : "false";
        }
        else if (req.command == "digitalWrite")
        {
            CHECK_ARG_COUNT(nargs, 2);
            CHECK_VALID_DIGITAL_PIN(req.arg0);

            pin = int(req.arg0);
            val = req.arg1 == "true" ? HIGH : LOW;
            digitalWrite(pin, val);
        }
        else if (req.command == "pinMode")
        {
            CHECK_ARG_COUNT(nargs, 2);
            CHECK_VALID_DIGITAL_PIN(req.arg0)

            pin  = int(req.arg0);
            mode = req.arg1 == "OUTPUT" ? OUTPUT : INPUT;
            pinMode(pin, mode);
        }
        else
        {
            res.messages += "Unrecognized command ";
            res.messages += req.cmd;
            goto exit;
        }

        res.retval = "true";

    exit:
        res.checksum = computeChecksum(response);

        Serial.println(serializeResponse(res).c_str());
    }
}

bool isValidDigitalPin(int pin)
{
    return pin >= 0 && pin <= 13; // Example validation for Arduino Uno
}

bool isHighOrLow(String value)
{
    return value == "HIGH" || value == "LOW";
}

bool requestParse(const String& input, Request& request)
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
    int headerEnd  = input.indexOf(';');
    request.header = input.substring(0, headerEnd);
    String rest    = input.substring(headerEnd + 1);

    int messageEnd   = remainder.indexOf(';');
    request.message  = remainder.substring(0, messageEnd);
    request.checksum = remainder.substring(messageEnd + 1);

    return true;
}

bool requestValidateChecksum(Request& request)
{
    return requestComputeChecksum(request) == request.checksum.toInt();
}

int requestComputeChecksum(Request& response)
{
    int checksum = 0;

    for (int i = 0; i < response.header.length(); i++)
        checksum += response.header[i];

    for (int i = 0; i < response.cmd.length(); i++)
        checksum += response.cmd[i];

    for (int i = 0; i < response.arg0.length(); i++)
        checksum += response.arg0[i];

    for (int i = 0; i < response.arg1.length(); i++)
        checksum += response.arg1[i];

    for (int i = 0; i < response.arg2.length(); i++)
        checksum += response.arg2[i];

    return checksum % 256;
}

String responseSerialize(const Request& request)
{
    String result = "";

    result += request.header + ";";
    result += request.retval + ";";
    result += request.out0 + ";";
    result += request.messages + ";";
    result += request.checksum;

    return result;
}

int responseComputeChecksum(Data& response)
{
    int checksum = 0;

    for (int i = 0; i < response.header.length(); i++)
        checksum += response.header[i];

    for (int i = 0; i < response.retval.length(); i++)
        checksum += response.retval[i];

    for (int i = 0; i < response.out0.length(); i++)
        checksum += response.out0[i];

    for (int i = 0; i < response.messages.length(); i++)
        checksum += response.messages[i];

    return checksum % 256;
}
