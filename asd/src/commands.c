#include "commands.h"

int digitalWrite(int pin, int value)
{
    Request  req;
    Response res;

    req.command  = "digitalWrite";
    sprintf(req.arg0, "%d", pin);
    sprintf(req.arg1, "%d", value);

    serialRequestResponse(req, res);

    return atoi(res.retval);
};

int digitalRead(int pin)
{
    Request  req;
    Response res;

    req.command  = "digitalRead";
    sprintf(req.arg0, "%d", pin);

    serialRequestResponse(req, res);

    return atoi(res.retval);
}

int pinMode(int pin, int mode)
{
    Request  req;
    Response res;

    req.command  = "pinMode";
    sprintf(req.arg0, "%d", pin);
    sprintf(req.arg1, "%d", mode);

    serialRequestResponse(req, res);

    return atoi(res.retval);
}
