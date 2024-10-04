#include "commands.h"

// TODO add request printing here to diagnose commands
// TODO refactor? three-times repeated code

void digitalWrite(int pin, PinState value)
{
    Request  req;
    Response res;

    strncpy(req.header, "pc", 3);
    req.cmd = COMMAND_DIGITAL_WRITE;
    req.arg0 = pin;
    req.arg1 = value;
    
    if (!serialRequestResponse(req, res, 0)) {
        printf("Serial comms failed.\n");
        return;
    }

    if (strcmp(res.messages, "")) {
        printf("%s\n", res.messages); 
    }

    if (res.retval != 0) {
        printf("Remote error!\n"); 
        return;
    }

    return;
}

PinState digitalRead(int pin)
{
    Request  req;
    Response res;

    strncpy(req.header, "pc", 3);
    req.cmd = COMMAND_DIGITAL_READ;
    req.arg0 = pin;

    if (!serialRequestResponse(req, res, 0)) {
        printf("Serial comms failed.\n");
        return -1;
    }

    if (strcmp(res.messages, "")) {
        printf("%s\n", res.messages); 
    }

    if (res.retval != 0) {
        printf("Remote error!\n"); 
        return -1;
    }

    return res.out0;
}

void pinMode(int pin, PinMode mode)
{
    Request  req;
    Response res;

    strncpy(req.header, "pc", 3);
    req.cmd = COMMAND_PIN_MODE;
    req.arg0 = pin;
    req.arg1 = mode;

    if (!serialRequestResponse(req, res, 0)) {
        printf("Serial comms failed.\n");
        return;
    }

    if (strcmp(res.messages, "")) {
        printf("%s\n", res.messages); 
    }

    if (res.retval != 0) {
        printf("Remote error!\n"); 
        return;
    }

    return;
}
