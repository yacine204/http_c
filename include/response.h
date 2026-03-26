#ifndef RESPONSE_H
#define RESPONSE_H

#include "body.h"
#include "header.h"
#include "response_status_line.h"
#include "request.h"
  
struct RESPONSE {
    struct STATUS_LINE response_status_line;
    struct HEADER response_headers[32];
    struct BODY body;
};

struct RESPONSE handle_response(struct REQUEST request);
struct RESPONSE get_handler(struct REQUEST request);
struct RESPONSE post_handler(struct REQUEST request);
struct RESPONSE put_handler(struct REQUEST request);
struct RESPONSE delete_handler(struct REQUEST request);
struct RESPONSE generate_response(struct STATUS_LINE status_line, struct HEADER response_header, struct BODY response_body);

#endif