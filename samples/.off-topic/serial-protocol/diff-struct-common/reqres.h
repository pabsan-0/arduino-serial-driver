struct Request
{
    int16_t a : 4;
    char    b[6];
} __attribute__((__packed__));

typedef struct Request Request; // do in its own line to avoid packing conflicts

struct Response
{
    int16_t a : 4;
    char    b[4];
} __attribute__((__packed__));

typedef struct Response Response;
