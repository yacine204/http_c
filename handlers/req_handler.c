#include "../include/request.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_HEADERS 50

struct REQUEST parse_request(char *buffer){
    printf("raw buffer:\n%s\n", buffer);

    struct REQUEST parsed_request;
    memset(&parsed_request, 0, sizeof(parsed_request));

    //request line parsing
    char *buffer_copy = strdup(buffer);
    char *saveptr;

    char *line = strtok_r(buffer_copy, "\r\n", &saveptr);
    if(!line){
        free(buffer_copy);
        return parsed_request;
    }

    char *line_copy = strdup(line);
    char *line_ptr;

    char *method = strtok_r(line_copy, " ", &line_ptr);
    char *path = strtok_r(NULL, " ", &line_ptr);
    char *version = strtok_r(NULL, " ", &line_ptr);

    if(method) {
        strncpy(parsed_request.request_line.method, method,
                sizeof(parsed_request.request_line.method)-1);
    }
    if(path) {
        strncpy(parsed_request.request_line.path, path,
                sizeof(parsed_request.request_line.path)-1);
    }
    if(version) {
        strncpy(parsed_request.request_line.protocol_version, version,
                sizeof(parsed_request.request_line.protocol_version)-1);
    }

    free(line_copy);

    //header parsing
    int i = 0;

    while((line = strtok_r(NULL, "\r\n", &saveptr)) != NULL){
        // empty line = end of headers
        if(strlen(line) == 0) break;

        char *header_copy = strdup(line);
        char *hdr_ptr;

        char *name = strtok_r(header_copy, ":", &hdr_ptr);
        char *value = strtok_r(NULL, "", &hdr_ptr); // rest of string

        if(name && value && i < MAX_HEADERS){
            // trim leading space
            while(*value == ' ') value++;

            strncpy(parsed_request.headers_section[i].header_name,
                    name,
                    sizeof(parsed_request.headers_section[i].header_name)-1);

            strncpy(parsed_request.headers_section[i].header_value,
                    value,
                    sizeof(parsed_request.headers_section[i].header_value)-1);

            i++;
        }

        free(header_copy);
    }

    parsed_request.header_count = i;

    free(buffer_copy);

    // body parsing
    char *body_start = strstr(buffer, "\r\n\r\n");

    if(body_start){
        body_start += 4; // skip "\r\n\r\n"

        strncpy(parsed_request.body.data,
                body_start,
                sizeof(parsed_request.body.data)-1);
    } else {
        parsed_request.body.data[0] = '\0';
    }

    return parsed_request;
}