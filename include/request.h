#ifndef REQUEST_H
#define REQUEST_H

#include <unistd.h>

// GET /index.html HTTP/1.1
struct REQUEST_LINE{
    char method[256];
    char path[256];
    char protocol_version[256];
};

// USER-AGENT:.... \r\n Content-type: .... \r\n
struct HEADER {
    char header_name[256];
    char header_value[1024];
};


struct BODY {
    char data[1024];
};


struct REQUEST {
    struct REQUEST_LINE request_line;
    struct HEADER headers_section[10];
    struct BODY body;
};

struct REQUEST parse_request(char *buffer);

#endif