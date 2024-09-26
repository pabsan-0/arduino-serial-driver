// TODO get rid of strcpy and other memory issues
// TODO nargs is not nicely handled
#define CHECK_ARG_COUNT(nargs, expected)                                     \
    if ((nargs) != (expected))                                               \
    {                                                                        \
        strlcat(res.messages, "Bad number of arguments. ", ResponseMsgSize); \
        goto exit;                                                           \
    }

#define CHECK_VALID_DIGITAL_PIN(pin)                         \
    if (!isValidDigitalPin(atoi(pin)))                       \
    {                                                        \
        strlcat(res.messages, "Bad pin. ", ResponseMsgSize); \
        goto exit;                                           \
    }

// Struct definitions to match C library
struct Request
{
    char header[10]; // Use fixed size for strings
    char command[20];
    char arg0[10];
    char arg1[10];
    char arg2[10];

    char checksum[10];
};

#define ResponseMsgSize (49)
struct Response
{
    char header[10];
    char retval[10];
    char out0[10];
    char messages[ResponseMsgSize + 1];

    char checksum[10];
    // TODO add Request data here for tracebacks
};

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if (Serial.available() >= sizeof(Request))
    {
        // Response with header, assume retval=False until changed
        Response res = { "arduino->pc", "false", "", "", "" };
        Request  req;

        // Read binary data into Request struct
        Serial.readBytes((char*)&req, sizeof(req));

        // Check the header to ensure it's a valid request
        if (strcmp(req.header, "pc->arduino") != 0)
        {
            strlcat(res.messages, "Invalid header. ", ResponseMsgSize);
            goto exit;
        }

        /*
        if (!requestValidateChecksum(req))
        {
            response.message += "Checksum failed.";
            goto exit;
        }
        */

        // Variable initialization
        int pin  = 0;
        int val  = 0;
        int mode = 0;

        if (strcmp(req.command, "digitalRead") == 0)
        {
            // CHECK_ARG_COUNT(nargs, 1);
            CHECK_VALID_DIGITAL_PIN(req.arg0);

            pin = atoi(req.arg0);

            val = digitalRead(pin);

            if (val == HIGH)
            {
                strcpy(res.out0, "HIGH");
            }
            else if (val == LOW)
            {
                strcpy(res.out0, "LOW");
            }
            else
            {
                strlcat(res.messages, "Bad digitalRead outcome. ", ResponseMsgSize);
                goto exit;
            }
        }
        else if (strcmp(req.command, "digitalWrite") == 0)
        {
            bool post_write_ok = false;

            // CHECK_ARG_COUNT(nargs, 2);
            CHECK_VALID_DIGITAL_PIN(req.arg0);

            pin = atoi(req.arg0);

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
                strlcat(res.messages, "Bad input pin state. ", ResponseMsgSize);
                goto exit;
            }

            if (!post_write_ok)
            {
                strlcat(res.messages, "Written/read values differ. ", ResponseMsgSize);
                goto exit;
            }
        }
        else if (strcmp(req.command, "pinMode") == 0)
        {
            // CHECK_ARG_COUNT(nargs, 2);
            CHECK_VALID_DIGITAL_PIN(req.arg0)

            pin  = atoi(req.arg0);
            mode = (strcmp(req.arg1, "OUTPUT") == 0) ? OUTPUT : INPUT;
            pinMode(pin, mode);
        }
        else
        {
            strlcat(res.messages, "Unrecognized command. ", ResponseMsgSize);
            goto exit;
        }

        // If arrived here, return value is success
        strcpy(res.retval, "true");

    exit:
        // TODO add checksum verification here
        // spritnf(res.checksum, "%d", responseComputeChecksum(res));
        sendResponse(res);
    }
}

// Helper function to send a Response struct back to the PC
void sendResponse(Response& res)
{
    Serial.write((const char*)&res, sizeof(res));
}

// Helper functions
bool isValidDigitalPin(int pin)
{
    return pin >= 0 && pin <= 13; // Example validation for Arduino Uno
}

bool isHighOrLow(const char* value)
{
    return (strcmp(value, "HIGH") == 0 || strcmp(value, "LOW") == 0);
}
