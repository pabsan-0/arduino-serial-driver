#include "asd_common/include/misc.h"
#include "asd_common/include/request.h"
#include "asd_common/include/response.h"

#define CHECK_VALID_DIGITAL_PIN(pin)                           \
    if (!isValidDigitalPin(pin))                               \
    {                                                          \
        strlcat(res.messages, "Bad pin. ", RESPONSE_MSG_SIZE); \
        goto exit;                                             \
    }

#define CHECK_VALID_DIGITAL_VAL(val)                                              \
    if (!isHighOrLow(pin))                                                        \
    {                                                                             \
        strlcat(res.messages, "Neither HIGH nor LOW value. ", RESPONSE_MSG_SIZE); \
        goto exit;                                                                \
    }

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB
    }
    digitalWrite(LED_BUILTIN, HIGH); // teller that we made changes if in doubt
}

void loop()
{
    // delay(500);
    char   buffer[sizeof(Request) * 2];
    size_t read_count = Serial.readBytesUntil(SERIAL_DELIMITER_INO, buffer, sizeof(Request) * 2);
    if (read_count != sizeof(Request))
    {
        return;
    }

    // Read binary data into Request struct
    Request req;
    memcpy(&req, buffer, sizeof(Request));

    // Response with header, assume retval=False until changed
    Response res = { "", "", -1, -1 };
    strcpy(res.header, "arduino");
    // strlcat(res.messages, "", RESPONSE_MSG_SIZE);

    // Check the header to ensure it's a valid request
    if (strcmp(req.header, "pc") != 0)
    {
        strlcat(res.messages, "Invalid header. ", RESPONSE_MSG_SIZE);
        goto exit;
    }

    // Variable initialization
    int pin  = 0;
    int val  = 0;
    int mode = 0;

    switch (req.cmd)
    {
        case COMMAND_DIGITAL_READ:

            CHECK_VALID_DIGITAL_PIN(req.arg0);

            pin = req.arg0;
            val = digitalRead(pin);

            res.out0 = val;
            break;

        case COMMAND_DIGITAL_WRITE:

            CHECK_VALID_DIGITAL_PIN(req.arg0);
            CHECK_VALID_DIGITAL_VAL(req.arg1);

            pin = req.arg0;
            val = req.arg1;

            digitalWrite(pin, val);

            if (digitalRead(pin) != val)
            {
                strlcat(res.messages, "Failed digitalWrite. ", RESPONSE_MSG_SIZE);
                goto exit;
            }
            break;

        case COMMAND_PIN_MODE:

            CHECK_VALID_DIGITAL_PIN(req.arg0)

            pin  = req.arg0;
            mode = req.arg1;
            pinMode(pin, mode);

            break;

        default:
            strlcat(res.messages, "Bad input command. ", RESPONSE_MSG_SIZE);
            goto exit;
            break;
    }

    // If arrived here, return value is success
    res.retval = 0;

exit:
    sendResponse(res);
}

void sendResponse(Response& res)
{
    char buf[] = { SERIAL_DELIMITER_INO };

    Serial.write((const char*)&res, sizeof(Response));
    Serial.write((const char*)buf, 1);
    Serial.flush();
}

bool isValidDigitalPin(int pin)
{
    return pin >= 0 && pin <= 13; // Example validation for Arduino Uno
}

bool isHighOrLow(int val)
{
    return ((val == HIGH) || (val == LOW));
}
