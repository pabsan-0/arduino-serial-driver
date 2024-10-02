#include "commands.h"

int digitalWrite(int pin, PinState value)
{
    Request  req;
    Response res;

    req.cmd = COMMAND_DIGITAL_WRITE;
    req.arg0 = pin;
    req.arg1 = value;
    
    serialRequestResponse(req, res);

    return atoi(res.retval);
};

int digitalRead(int pin)
{
    Request  req;
    Response res;

    req.cmd = COMMAND_DIGITAL_READ;
    req.arg0 = pin;

    serialRequestResponse(req, res);

    return atoi(res.retval);
}

int pinMode(int pin, PinMode mode)
{
    Request  req;
    Response res;

    req.cmd = COMMAND_PIN_MODE;
    req.arg0 = pin;
    req.arg1 = mode;

    serialRequestResponse(req, res);

    return atoi(res.retval);
}
