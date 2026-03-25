#include "../include/request.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//reads https throws out its struct
struct REQUEST parse_request(char *buffer){
    printf("raw buffer\n: %s\n", buffer);
    
    //parse into REQUEST struct
    struct REQUEST parsed_request;

    char *tokens;
    char *buf_ptr, *line_ptr;
    tokens = strtok_r(buffer,"\r\n", &buf_ptr);
    //printf("headers:%s", buf_ptr);
    //printf("token:%s\n", tokens);
    
    //parse REQUEST_LINE
    struct REQUEST_LINE parsed_request_line;

    char *request_line_copy = strdup(tokens);

    char *method = strtok_r(request_line_copy, " ",&line_ptr);
    char *path = strtok_r(NULL, " ",&line_ptr);
    char *version = strtok_r(NULL, " ",&line_ptr);

    strcpy(parsed_request_line.method, method);
    strcpy(parsed_request_line.path, path);
    strcpy(parsed_request_line.protocol_version, version);

    parsed_request.request_line = parsed_request_line;
    //printf("request line:\nmethod: %s\npath: %s\nversion: %s\n", parsed_request.request_line.method, parsed_request.request_line.path, parsed_request.request_line.protocol_version);

    

    char *headers_ptr = buf_ptr;
    tokens = strtok_r(headers_ptr, "\r\n", &buf_ptr);
    //printf("reached here, tokens is: %s\n", tokens ? tokens : "NULL");
    
    struct HEADER pasrsed_request_headers;

    int i = 0;
   
    while(tokens != NULL){
        char *hdr_ptr;
        char *header_copy = strdup(tokens);
        char *header_name = strtok_r(header_copy,":",&hdr_ptr);
        char *header_value = strtok_r(NULL, " ",&hdr_ptr);
        if(header_value==NULL || strcmp(header_name, "{")==0) break;
        strcpy(parsed_request.headers_section[i].header_name, header_name);
        strcpy(parsed_request.headers_section[i].header_value, header_value);
        //printf("header[%d]: name=%s, value=%s\n", i, header_name, header_value);
        i++;    
        
        tokens = strtok_r(NULL,"\r\n",&buf_ptr);
        
    }
    struct BODY parsed_request_body;
    strcpy(parsed_request_body.data, buf_ptr);
    strcpy(parsed_request.body.data,buf_ptr);
    //printf("body: \n%s", parsed_request_body.data); 

    return parsed_request;
}

//create