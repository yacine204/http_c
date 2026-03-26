#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h> 

#include "include/response.h"
#include "include/request.h"
#include "include/header.h"

struct RESPONSE handle_response(struct REQUEST request){

    if(strcmp(request.request_line.path,"..")==0){
        struct STATUS_LINE generated_status_line;
        struct HEADER response_header;
        struct BODY generated_body;

        generated_status_line = generate_response_status_line("Forbidden", 403);
        response_header = header_generator("Content-Type", "plain text", 1);
        strcpy(generated_body.data, "nice try bozo!");
        return generate_response(generated_status_line, response_header, generated_body);
    }

    char *request_method = request.request_line.method;

    if(strcmp(request_method, "GET")==0){
        return get_handler(request);
    }
    else if(strcmp(request_method, "POST")==0){
        return post_handler(request);
    }
    else if(strcmp(request_method, "DELETE")==0){
        return delete_handler(request);
    }
    else if(strcmp(request_method, "PUT")==0){
        return put_handler(request);
    }
    else {
        struct STATUS_LINE generated_status_line;
        struct HEADER generated_header;
        struct BODY generated_body;

        generated_status_line = generate_response_status_line("Method Not Allowed", 405);
        generated_header = header_generator("Content-Type", "plain text", 1);
        strcpy(generated_body.data, "method not handled yet, try get, put, post, delete");
        return generate_response(generated_status_line, generated_header, generated_body);
    }
}

struct RESPONSE generate_response(struct STATUS_LINE status_line, struct HEADER response_header, struct BODY response_body){
    struct RESPONSE generated_response;

    generated_response.response_status_line = status_line;
    generated_response.response_headers[0] = response_header;
    generated_response.body = response_body;

    return generated_response;
}

struct RESPONSE get_handler(struct REQUEST request){
    struct REQUEST_LINE *request_line = &request.request_line;

    struct STATUS_LINE generated_status_line;
    struct HEADER generated_header;
    struct BODY generated_body;

    generated_status_line = generate_response_status_line("OK", 200);
    generated_header = header_generator("Content-Type", "plain text", 1);

    if(strcmp(request_line->path, "/") == 0){
        strcpy(generated_body.data, "Goodbye, world!");
        return generate_response(generated_status_line, generated_header, generated_body);
    }

    FILE *file = fopen(request_line->path, "r");
    if(!file){
        generated_status_line = generate_response_status_line("Not Found", 404);
        strcpy(generated_body.data, "file not found, try same api with post!");
        return generate_response(generated_status_line, generated_header, generated_body);
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    if((long)sizeof(generated_body.data) < filesize){
        filesize = (long)sizeof(generated_body.data);
    }

    char *buffer = malloc(filesize + 1);
    if(!buffer){
        fclose(file);
        generated_status_line = generate_response_status_line("Internal Server Error", 500);
        strcpy(generated_body.data, "out of memory");
        return generate_response(generated_status_line, generated_header, generated_body);
    }

    size_t read_size = fread(buffer, 1, filesize, file);
    buffer[read_size] = '\0';
    strncpy(generated_body.data, buffer, sizeof(generated_body.data) - 1);
    generated_body.data[sizeof(generated_body.data) - 1] = '\0';
    free(buffer);
    fclose(file);

    return generate_response(generated_status_line, generated_header, generated_body);
}

struct RESPONSE post_handler(struct REQUEST request){
    struct STATUS_LINE generated_status_line;
    struct HEADER generated_headers;
    struct BODY generated_body;

    char clean_path[256];
    strncpy(clean_path, request.request_line.path, sizeof(clean_path) - 1);
    clean_path[sizeof(clean_path) - 1] = '\0';

    char *path = clean_path;
    
    if(path[0] == '/') path++;
    
    printf("final path for fopen: '%s'\n", path);
    
    generated_headers = header_generator("Content-Type", "plain text", 1);

    FILE *file = fopen(request.request_line.path, "a");
    if(!file){
        strcpy(generated_body.data, "file couldn't be created, verify the path");
        printf("fopen result: %p\n", file);
        printf("errno: %d - %s\n", errno, strerror(errno));
        generated_status_line = generate_response_status_line("Not Found", 404);
        return generate_response(generated_status_line, generated_headers, generated_body);
    }
    
    fprintf(file, "%s", request.body.data);
    fclose(file);

    file = fopen(request.request_line.path, "r");
    if(!file){
        strcpy(generated_body.data, "couldn't re-open file for reading");
        generated_status_line = generate_response_status_line("Internal Server Error", 500);
        return generate_response(generated_status_line, generated_headers, generated_body);
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    if((long)sizeof(generated_body.data) < filesize){
        fseek(file, -(long)sizeof(generated_body.data), SEEK_END);
        filesize = (long)sizeof(generated_body.data);
    }

    char *buffer = malloc(filesize + 1);
    if(!buffer){
        fclose(file);
        strcpy(generated_body.data, "out of memory");
        generated_status_line = generate_response_status_line("Internal Server Error", 500);
        return generate_response(generated_status_line, generated_headers, generated_body);
    }

    size_t read_size = fread(buffer, 1, filesize, file);
    buffer[read_size] = '\0';
    fclose(file);

    int msg_len = snprintf(NULL, 0, "from '%s': ", request.request_line.path);
    char *return_message = malloc(msg_len + 1);
    if(!return_message){
        free(buffer);
        strcpy(generated_body.data, "out of memory");
        generated_status_line = generate_response_status_line("Internal Server Error", 500);
        return generate_response(generated_status_line, generated_headers, generated_body);
    }
    snprintf(return_message, msg_len + 1, "from '%s': ", request.request_line.path);

    int response_len = snprintf(NULL, 0, "%s...%s", return_message, buffer);
    char *response = malloc(response_len + 1);
    if(!response){
        free(buffer);
        free(return_message);
        strcpy(generated_body.data, "out of memory");
        generated_status_line = generate_response_status_line("Internal Server Error", 500);
        return generate_response(generated_status_line, generated_headers, generated_body);
    }
    snprintf(response, response_len + 1, "%s...%s", return_message, buffer);

    strncpy(generated_body.data, response, sizeof(generated_body.data) - 1);
    generated_body.data[sizeof(generated_body.data) - 1] = '\0';

    free(buffer);
    free(return_message);
    free(response);

    generated_status_line = generate_response_status_line("Created", 201);
    return generate_response(generated_status_line, generated_headers, generated_body);
}

struct RESPONSE put_handler(struct REQUEST request){
    struct STATUS_LINE generated_status_line;
    struct HEADER generated_header;
    struct BODY generated_body;

    generated_status_line = generate_response_status_line("Not Implemented", 501);
    generated_header = header_generator("Content-Type", "plain text", 1);
    strcpy(generated_body.data, "PUT not implemented yet");
    return generate_response(generated_status_line, generated_header, generated_body);
}

struct RESPONSE delete_handler(struct REQUEST request){
    struct STATUS_LINE generated_status_line;
    struct HEADER generated_header;
    struct BODY generated_body;

    generated_status_line = generate_response_status_line("Not Implemented", 501);
    generated_header = header_generator("Content-Type", "plain text", 1);
    strcpy(generated_body.data, "DELETE not implemented yet");
    return generate_response(generated_status_line, generated_header, generated_body);
}