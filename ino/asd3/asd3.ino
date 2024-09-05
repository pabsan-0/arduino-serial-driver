#define CHECK_ARG_COUNT(nargs, expected)                            \
    if ((nargs) != (expected))                                      \
    {                                                               \
        res.messages += "Expected " + String(expected);             \
        res.messages += " argument(s), got " + String(nargs) + "."; \
        goto exit;                                                  \
    }

#define CHECK_VALID_DIGITAL_PIN(pin)                    \
    if (!isValidDigitalPin(pin.toInt()))                \
    {                                                   \
        res.messages += "Bad pin " + String(pin) + "."; \
        goto exit;                                      \
    }

struct Request
{
    String header;
    String command;
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

        res.header   = "arduino->pc"; // constant
        res.retval   = "false";       // set to true at loop's end
        res.out0     = "";
        res.messages = "";
        res.checksum = "";

        reqStr = Serial.readStringUntil('\n');
        reqStr.trim();

        if (!requestParse(reqStr, req, nargs))
        {
            goto exit;
        }

        if (req.header == "arduino->pc")
        {
            goto exit;
        }

        /*
        if (!requestValidateChecksum(req))
        {
            response.message += "Checksum failed.";
            goto exit;
        }
        */

        int pin  = 0;
        int val  = 0;
        int mode = false;
        if (req.command == "digitalRead")
        {
            CHECK_ARG_COUNT(nargs, 1);
            CHECK_VALID_DIGITAL_PIN(req.arg0)

            pin = req.arg0.toInt();

            val = digitalRead(pin);
            if (val == HIGH)
            {
                res.out0 = "HIGH";
            }
            else if (val == LOW)
            {
                res.out0 = "LOW";
            }
            else
            {
                res.messages += "Unexpected digitalRead outcome. ";
                goto exit;
            }
        }
        else if (req.command == "digitalWrite")
        {
            bool post_write_ok = false;

            CHECK_ARG_COUNT(nargs, 2);
            CHECK_VALID_DIGITAL_PIN(req.arg0);

            pin = req.arg0.toInt();

            if (req.arg1 == "HIGH")
            {
                digitalWrite(pin, HIGH);
                post_write_ok = (digitalRead(pin) == HIGH);
            }
            else if (req.arg1 == "LOW")
            {
                digitalWrite(pin, LOW);
                post_write_ok = (digitalRead(pin) == LOW);
            }
            else
            {
                res.messages += "Unrecognized pin state.";
                goto exit;
            }

            if (!post_write_ok)
            {
                res.messages += "Pin reads different than set value! ";
                goto exit;
            }
        }
        else if (req.command == "pinMode")
        {
            CHECK_ARG_COUNT(nargs, 2);
            CHECK_VALID_DIGITAL_PIN(req.arg0)

            pin  = req.arg0.toInt();
            mode = (req.arg1 == "OUTPUT") ? OUTPUT : INPUT;
            pinMode(pin, mode);
        }
        else
        {
            res.messages += "Unrecognized command ";
            res.messages += req.command;
            goto exit;
        }

        // If arrived here, return as success
        res.retval = "true";

    exit:
        res.messages += " __" + reqStr + "__ ";
        res.checksum = responseComputeChecksum(res);

        Serial.println(responseSerialize(res).c_str());
    }
}

bool isValidDigitalPin(int pin)
{
    return pin >= 0 && pin <= 13; // Example validation for Arduino Uno
}

bool isHighOrLow(String value)
{
    return (value == "HIGH") || (value == "LOW");
}

bool requestParse(const String& input, Request& request, int& nargs)
{
    // Check if the input contains exactly 3 semicolons
    int semicolonCount = 0;
    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == ';')
            semicolonCount++;
    }

    if (semicolonCount != 5)
    {
        Serial.print("Invalid input format. Expected 5 semicolons but found: ");
        Serial.println(semicolonCount);
        return false;
    }

    String nextString;
    int    nextSemicolon;

    nextSemicolon  = input.indexOf(';');
    request.header = input.substring(0, nextSemicolon);
    nextString     = input.substring(nextSemicolon + 1);

    nextSemicolon   = nextString.indexOf(';');
    request.command = nextString.substring(0, nextSemicolon);
    nextString      = nextString.substring(nextSemicolon + 1);

    nextSemicolon = nextString.indexOf(';');
    request.arg0  = nextString.substring(0, nextSemicolon);
    nextString    = nextString.substring(nextSemicolon + 1);

    nextSemicolon = nextString.indexOf(';');
    request.arg1  = nextString.substring(0, nextSemicolon);
    nextString    = nextString.substring(nextSemicolon + 1);

    nextSemicolon    = nextString.indexOf(';');
    request.arg2     = nextString.substring(0, nextSemicolon);
    request.checksum = nextString.substring(nextSemicolon + 1);

    request.header.trim();
    request.command.trim();
    request.arg0.trim();
    request.arg1.trim();
    request.arg2.trim();
    request.checksum.trim();

    if (request.arg0 != "")
        nargs++;
    if (request.arg1 != "")
        nargs++;
    if (request.arg2 != "")
        nargs++;

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

    for (int i = 0; i < response.command.length(); i++)
        checksum += response.command[i];

    for (int i = 0; i < response.arg0.length(); i++)
        checksum += response.arg0[i];

    for (int i = 0; i < response.arg1.length(); i++)
        checksum += response.arg1[i];

    for (int i = 0; i < response.arg2.length(); i++)
        checksum += response.arg2[i];

    return checksum % 256;
}

String responseSerialize(const Response& response)
{
    String result = "";

    result += response.header + ";";
    result += response.retval + ";";
    result += response.out0 + ";";
    result += response.messages + ";";
    result += response.checksum;

    return result;
}

int responseComputeChecksum(Response& response)
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
