#ifndef HEADER_H
#define HEADER_H


struct HEADER {
    char header_name[256];
    char header_value[1024];
};

struct HEADER header_generator(char* header_name, char* header_value, int number_of_headers);
#endif