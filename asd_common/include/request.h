#ifndef __REQUEST_H__
#define __REQUEST_H__

struct Request
{
    char    header[10];
    Command cmd;
    uint8_t arg0;
    uint8_t arg1;
    uint8_t checksum;
};
// TODO packing

typedef struct Request Request;

#endif
