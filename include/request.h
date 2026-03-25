#ifndef REQUEST_H
#define REQUEST_H
#include <unistd.h>

#include "body.h"
#include "header.h"

struct REQUEST_LINE{
    char method[256];
    char path[256];
    char protocol_version[256];
};


struct REQUEST {
    struct REQUEST_LINE request_line;
    struct HEADER headers_section[32];
    struct BODY body;
};

struct REQUEST parse_request(char *buffer);

#endif