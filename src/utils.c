#include "utils.h"

//throw error, error message is set according to 'errno'.
void throwError(){
    printf("Error: %s\n", strerror(errno));
    exit(0);
}

//throw error when there are invalid arguments.
void throwInvalidArguments(){
    printf("Error: Invalid arguments\n");
    exit(0);
}

//check if a string is consist only from digits.
int is_int(char* str){
    for(int i=0; i<strlen(str); i++){
        if(!('0'<=str[i] && str[i] <='9')){return 0;}
    }
    return 1;
}

//send message with length of 'len' written in 'buffer' through 'socket'.
void send_all(int socket, char* buffer, int len){
    int message_length = htonl(len);
    char message_length_str[WORD_SIZE_IN_BYTES];
    memcpy(message_length_str, &message_length, WORD_SIZE_IN_BYTES);

    int total = 0;
    int bytes_left = WORD_SIZE_IN_BYTES;
    int n = 0;

    while(total < WORD_SIZE_IN_BYTES){
        n = send(socket, message_length_str+total, bytes_left, 0);
        if(n < 0){throwError();}
        total+=n;
        bytes_left -= n;
    }

    total = 0;
    bytes_left = len;
    n = 0;

    while(total < len){
        n = send(socket, buffer+total, bytes_left, 0);
        if(n < 0){throwError();}
        total+=n;
        bytes_left -= n;
    }
}

//receive message read from 'socket' and write it to 'buffer'.
void recv_all(int socket, char* buffer){
    char message_length_str[WORD_SIZE_IN_BYTES];
    int message_length = 0;
    int len = WORD_SIZE_IN_BYTES;
    int total = 0;
    int bytes_left = WORD_SIZE_IN_BYTES;
    int n = 0;

    while(total < len){
        n = recv(socket, message_length_str+total, bytes_left, 0);
        if(n < 0){throwError();}
        total+=n;
        bytes_left -= n;
    }

    memcpy(&message_length, message_length_str, WORD_SIZE_IN_BYTES);
    len = ntohl(message_length);

    total = 0;
    bytes_left = len;
    n = 0;

    while(total < len){
        n = recv(socket, buffer+total, bytes_left, 0);
        if(n < 0){throwError();}
        total+=n;
        bytes_left -= n;
    }
}