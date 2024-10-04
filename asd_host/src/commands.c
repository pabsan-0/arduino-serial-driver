#include "commands.h"

#define COMMAND_COMMON_BLOCK(req, res)                                 \
    do                                                                 \
    {                                                                  \
        int err = serialRequestResponse(req, res, 0);                  \
        if (err)                                                       \
        {                                                              \
            printf("Comms error: Serial failed.\n");                   \
            goto error;                                                \
        }                                                              \
                                                                       \
        if (strcmp(res.messages, ""))                                  \
        {                                                              \
            printf("%s\n", res.messages);                              \
        }                                                              \
                                                                       \
        if (res.retval != 0)                                           \
        {                                                              \
            printf("Remote error: Response code '%d'!\n", res.retval); \
            goto error;                                                \
        }                                                              \
    }                                                                  \
    while (0);

#define COMMAND_COMMON_ERROR(req, res) \
    do                                 \
    {                                  \
        REQUEST_PRINT(req);            \
        RESPONSE_PRINT(res);           \
    }                                  \
    while (0)

void digitalWrite(int pin, PinState value)
{
    Request  req = { 0 };
    Response res = { 0 }; // else may contain stack garbage

    strncpy(req.header, "pc", 3);
    req.cmd  = COMMAND_DIGITAL_WRITE;
    req.arg0 = pin;
    req.arg1 = value;

    COMMAND_COMMON_BLOCK(req, res);

    return;
error:
    COMMAND_COMMON_ERROR(req, res);
    return;
}

PinState digitalRead(int pin)
{
    Request  req = { 0 };
    Response res = { 0 };

    strncpy(req.header, "pc", 3);
    req.cmd  = COMMAND_DIGITAL_READ;
    req.arg0 = pin;

    COMMAND_COMMON_BLOCK(req, res);

    return res.out0;
error:
    COMMAND_COMMON_ERROR(req, res);
    return -1;
}

void pinMode(int pin, PinMode mode)
{
    Request  req = { 0 };
    Response res = { 0 };

    strncpy(req.header, "pc", 3);
    req.cmd  = COMMAND_PIN_MODE;
    req.arg0 = pin;
    req.arg1 = mode;

    COMMAND_COMMON_BLOCK(req, res);

    return;
error:
    COMMAND_COMMON_ERROR(req, res);
    return;
}
