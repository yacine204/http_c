#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){

    char BUFFER[1024];
    int data;
    int soc;

    struct sockaddr_in new_server;
    new_server.sin_family = AF_INET;
    new_server.sin_port = htons(8080);
    //contains the ip address, INDARR_ANY to accept any connection available.
    new_server.sin_addr.s_addr = INADDR_ANY;

    socklen_t socket_addr = sizeof(new_server);

    if((soc = socket(AF_INET,SOCK_STREAM,0))<0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if((bind(soc, (struct sockaddr *)&new_server, sizeof(new_server)))<0){
        perror("binding failed");
        exit(EXIT_FAILURE);
    }

    if((listen(soc,2)<0)){
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    
    while(1){
        int client = accept(soc, (struct sockaddr *)&new_server, &socket_addr);
        if (client<0){
            perror("acception failed");
            exit(EXIT_FAILURE);        
        }
        
        //read the http
        if(read(client, BUFFER, sizeof(BUFFER))<0){
            perror("http reading failed");
            exit(EXIT_FAILURE);
        }
        
        char res[255];
        char body[] = "larp larp larp, SINISTER!!!!!";
        snprintf(res, sizeof(res), "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %zu\r\n\r\n%s",strlen(body),body);
        send(client, res, strlen(res),0);
    }
    
    return 0;
}