#ifndef RESPONSE_STATUS_LINE_H
#define RESPONSE_STATUS_LINE_H

struct STATUS_LINE {
    char protocol_version[32];
    int response_status;
    char status_message[1024];
};

struct STATUS_LINE generate_response_status_line(char *response_message, int response_status);

#endif