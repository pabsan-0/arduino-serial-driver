#ifndef __RESPONSE_H__
#define __RESPONSE_H__

struct Response
{
    char    header[10];
    uint8_t retval;
    uint8_t out0;
    char    messages[50];
    Request req;

    uint8_t checksum;
};
// TODO packing

typedef struct Response Response;

#endif
