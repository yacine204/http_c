#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "include/header.h"

struct HEADER header_generator(char* header_name, char* header_value, int number_of_headers){
    struct HEADER header;
    strncpy(header.header_name, header_name, sizeof(header.header_name) - 1);
    strncpy(header.header_value, header_value, sizeof(header.header_value) - 1);
    return header;
}