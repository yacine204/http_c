#include "include/response.h"
#include "include/request.h"
#include "include/body.h"
#include "include/response_status_line.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


struct RESPONSE routing(struct REQUEST request){

    struct STATUS_LINE res_status_line;
    struct HEADER res_header;
    struct BODY res_body;
    char full_path[512];
    char *req_path = request.request_line.path;

    // Map root "/" to index.html
    if(strcmp(req_path, "/") == 0){
        snprintf(full_path, sizeof(full_path), "./assets/index.html");
    } else {
        if(req_path[0] == '/') req_path++;
        snprintf(full_path, sizeof(full_path), "./assets/%s", req_path);
    }

    FILE *file = fopen(full_path, "r");
    if(!file){
        res_status_line = generate_response_status_line("Not Found", 404);
        res_header = header_generator("Content-Type", "text/plain", 1);
        strcpy(res_body.data, "file not found");
        return generate_response(res_status_line, res_header, res_body);  // return instead of just return;
    }

    // Determine Content-Type based on extension
    char *ext = strrchr(full_path, '.'); 
    if(ext != NULL) ext++; 
    else ext = ""; 

    if(strcmp(ext, "html") == 0)
        res_header = header_generator("Content-Type", "text/html", 1);
    else if(strcmp(ext, "js") == 0)
        res_header = header_generator("Content-Type", "text/javascript", 1);
    else if(strcmp(ext, "css") == 0)
        res_header = header_generator("Content-Type", "text/css", 1);
    else if(strcmp(ext, "png") == 0)
        res_header = header_generator("Content-Type", "image/png", 1);
    else
        res_header = header_generator("Content-Type", "application/octet-stream", 1);

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    if(filesize > sizeof(res_body.data) - 1)
        filesize = sizeof(res_body.data) - 1;

    fread(res_body.data, 1, filesize, file);
    res_body.data[filesize] = '\0';
    fclose(file);

    res_status_line = generate_response_status_line("OK", 200);
    return generate_response(res_status_line, res_header, res_body);
}