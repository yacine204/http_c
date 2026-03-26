#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "include/request.h"
#include "include/header.h"
#include "include/body.h"
#include "include/response.h"
#include "include/response_status_line.h"

int main(){
    char BUFFER[1024];
    int soc;
    struct sockaddr_in new_server;
    new_server.sin_family = AF_INET;
    new_server.sin_port = htons(8080);
    new_server.sin_addr.s_addr = INADDR_ANY;
    socklen_t socket_addr = sizeof(new_server);

    if((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if((bind(soc, (struct sockaddr *)&new_server, sizeof(new_server))) < 0){
        perror("binding failed");
        exit(EXIT_FAILURE);
    }

    if((listen(soc, 2)) < 0){
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    while(1){
        int client = accept(soc, (struct sockaddr *)&new_server, &socket_addr);
        if(client < 0){
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        int bytes;
        if((bytes = read(client, BUFFER, sizeof(BUFFER)-1)) < 0){
            perror("http reading failed");
            exit(EXIT_FAILURE);
        }
        BUFFER[bytes] = '\0';

        struct REQUEST client_request = parse_request(BUFFER);
        struct RESPONSE response = handle_response(client_request);

        char res[2048];
        snprintf(res, sizeof(res),
            "%s %d %s\r\n%s: %s\r\nContent-Length: %zu\r\n\r\n%s",
            response.response_status_line.protocol_version,  
            response.response_status_line.response_status,
            response.response_status_line.status_message,
            response.response_headers[0].header_name,
            response.response_headers[0].header_value,
            strlen(response.body.data),
            response.body.data
        );

        printf("response:\n%s\n", res);

        send(client, res, strlen(res), 0);
        close(client);
    }

    return 0;
}