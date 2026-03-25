#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "include/header.h"

void header_generator(struct HEADER *header, char* header_name, char* header_value){
    struct HEADER header;
    strcpy(header->header_name, header_name);
    strcpy(header->header_value, header_value);
}
