#include "include/response_status_line.h"
#include <stdio.h>
#include <unistd.h>

struct STATUS_LINE generate_response_status_line(char *response_message, int response_status){

    struct STATUS_LINE generated_status_line;

    strcpy(generated_status_line.protocol_version,"HTTP/1.1");
    strcpy(generated_status_line.status_message, response_message);
    generated_status_line.response_status = response_status;

    return generated_status_line;
}