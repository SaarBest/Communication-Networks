//
// Created by Saar on 26/03/2019.
//

#include "client.h"

void authorize(int socket){
    while(1){
        char username[MAX_CRED_LENGTH];
        char password[MAX_CRED_LENGTH];
        scanf("User: %s\nPassword: %s", username, password);
        char credentials[32] = {0};
        sprintf(credentials, "%s %s", username, password);
        send_all(socket, credentials, strlen(credentials));
        char response[5] = {0};
        recv_all(socket, response);
        if(!strcmp(response,OK_MESSAGE)){
            printf("Hi %s, good to see you.\n", username);
            break;
        }
        else{
            printf("Failed to login.\n");
        }
    }
}

void print_illegal_command(){
    printf("Illegal command.\n");
}

void send_list_of_courses_command(int socket, char* user_input){
    char command_validity[2] = {0};
    sscanf(user_input, "courses_list%c", command_validity);
    if(strcmp(command_validity, EMPTY_STRING)){
        print_illegal_command();
        return;
    }
    send_all(socket, LIST_OF_COURSES_MESSAGE, COMMAND_MESSAGE_LENGTH);
    char course_message[MAX_COURSE_LINE_LENGTH] = {0};
    recv_all(socket, course_message);
    while(strcmp(course_message,END_MESSAGE)){
        int course_number;
        char course_name[MAX_COURSE_NAME_LENGTH] = {0};
        sscanf(course_message,"%d %s",&course_number,course_name);
        printf("%d:\t%s\n",course_number,course_name);
        for(int i=0; i<MAX_COURSE_LINE_LENGTH; i++) {course_message[i]=0; }
        recv_all(socket, course_message);
    }
}

void send_add_course_command(int socket,char* user_input){
    char command_validity[2] = {0};
    int course_number_to_add;
    char course_name_to_add[MAX_COURSE_NAME_LENGTH] = {0};
    sscanf(user_input, "add_course %d %s %c", &course_number_to_add, course_name_to_add, command_validity);
    if(strcmp(command_validity, EMPTY_STRING) || (course_number_to_add < 0 || course_number_to_add > 9999)){
        print_illegal_command();
        return;
    }
    char command_message[108] = {0};
    sprintf(command_message, "%s %d %s", ADD_COURSE_MESSAGE, course_number_to_add, course_name_to_add);
    send_all(socket, command_message, strlen(command_message));
    char response_massage[5] = {0};
    recv_all(socket, response_massage);
    if(!strcmp(response_massage, OK_MESSAGE)){
        printf("%d added successfully.\n", course_number_to_add);
    }
    else{
        printf("%d exists in the database!\n", course_number_to_add);
    }
}

void send_rate_course_command(int socket, char* user_input){
    char command_validity[2] = {0};
    int course_number_to_rate;
    int rate_value;
    char rate_text[MAX_TEXT_RATE_LENGTH] = {0};
    sscanf(user_input, "rate_course %d %d %s %c", &course_number_to_rate, &rate_value, rate_text, command_validity);
    if(strcmp(command_validity, EMPTY_STRING) || (course_number_to_rate < 0 || course_number_to_rate > 9999)
            ||(rate_value < 0 || rate_value > 100)){
        print_illegal_command();
        return;
    }
    char command_message[MAX_MESSAGE_LENGTH] = {0};
    sprintf(command_message, "%s %d %d %s", RATE_COURSE_MESSAGE, course_number_to_rate, rate_value, rate_text);
    send_all(socket, command_message, strlen(command_message));
}

void send_get_rate_command(int socket, char* user_input){
    char command_validity[2] = {0};
    int course_number;
    sscanf(user_input, "get_rate %d %c", &course_number, command_validity);
    if(strcmp(command_validity, EMPTY_STRING) || (course_number < 0 || course_number > 9999)){
        print_illegal_command();
        return;
    }
    char command_message[7] = {0};
    sprintf(command_message, "%s %d", GET_RATE_MESSAGE, course_number);
    send_all(socket, command_message, strlen(command_message));

    char rate_message[MAX_MESSAGE_LENGTH] = {0};
    recv_all(socket, rate_message);
    while(strcmp(rate_message,END_MESSAGE)){
        char username[MAX_CRED_LENGTH] = {0};
        int rate_value;
        char rate_text[MAX_TEXT_RATE_LENGTH] = {0};
        sscanf(rate_message,"%d %s %d %s",&course_number, username, &rate_value, rate_text);
        printf("%s:\t%d\t%s\n",username, rate_value, rate_text);
        for(int i=0; i<MAX_MESSAGE_LENGTH; i++) {rate_message[i]=0; }
        recv_all(socket, rate_message);
    }
}

void send_quit_command(int socket, char* user_input, int* session_is_alive){
    char command_validity[2] = {0};
    int course_number;
    sscanf(user_input, "quit%c", command_validity);
    if(strcmp(command_validity, EMPTY_STRING)){
        print_illegal_command();
        return;
    }
    send_all(socket, QUIT_MESSAGE, strlen(QUIT_MESSAGE));
    *session_is_alive = 0;
}

void handle_command(int socket, char* user_input, int* session_is_alive){
    char command_prefix[MAX_COMMAND_LENGTH] = {0};
    sscanf(user_input, "%s", command_prefix);
    if(!strcmp(command_prefix, COMMAND_LIST_OF_COURSES)){
        send_list_of_courses_command(socket, user_input);
    }
    else if(!strcmp(command_prefix, COMMAND_ADD_COURSE)){
        send_add_course_command(socket,user_input);
    }
    else if(!strcmp(command_prefix, COMMAND_RATE_COURSE)){
        send_rate_course_command(socket, user_input);
    }
    else if(!strcmp(command_prefix, COMMAND_GET_RATE)){
        send_get_rate_command(socket, user_input);
    }
    else if(!strcmp(command_prefix, COMMAND_QUIT)){
        send_quit_command(socket, user_input, session_is_alive);
    }
    else{
        print_illegal_command();
    }
}

int main(int argc, char* argv[]){
    char* server_hostname = LOCALHOST;
    int server_port = 1337;
    if(argc > 1){
        server_hostname = argv[1];
        server_port = atoi(argv[2]);
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock < 0){throwError();}

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_hostname, &server_addr.sin_addr);

    connect(sock, (struct sockaddr*) &server_addr, sizeof(struct sockaddr));

    char message[MAX_MESSAGE_LENGTH] = {0};
    recv_all(sock, message);
    printf("%s\n", message);

    authorize(sock);

    int session_is_alive = 1;
    while(session_is_alive){
        char user_input[MAX_MESSAGE_LENGTH] = {0};
        scanf("%s", user_input);
        handle_command(sock, user_input, &session_is_alive);
    }
}