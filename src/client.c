//
// Created by Saar on 26/03/2019.
//

#include "client.h"

int is_valid_authorization_input(char* first, char* second, char* third, char* first_value){
    if(!first || !second){
        return 0;
    }
    if(strcmp(first, first_value) || !(strlen(second) > 0 && strlen(second)<MAX_CRED_LENGTH) || third){
        return 0;
    }
    return 1;
}

int authorize(int socket){
    while(1){
        char user_input[30];
        char password_input[30];
        char *username = NULL, *password = NULL;
        int isValid = 1;
        fgets(user_input, 30, stdin);
        const char delim[3] = " \n";
        char *result1[3], *result2[3];
        result1[0] = strtok(user_input, delim);
        result1[1] = strtok(NULL, delim);
        result1[2] = strtok(NULL, delim);
        if(!is_valid_authorization_input(result1[0], result1[1], result1[2], "User:")){
            isValid = 0;
        }
        else{
            username = result1[1];
        }
        fgets(password_input, 30, stdin);
        result2[0] = strtok(password_input, delim);
        result2[1] = strtok(NULL, delim);
        result2[2] = strtok(NULL, delim);
        if(!is_valid_authorization_input(result2[0], result2[1], result2[2], "Password:") || !isValid){
            printf("Failed to login.\n");
            return 0;
        }
        password = result2[1];
        char credentials[32] = {0};
        if(sprintf(credentials, "%s %s", username, password) < 0){throwError(); }
        send_all(socket, credentials, strlen(credentials));
        char response[5] = {0};
        recv_all(socket, response);
        if(!strcmp(response,OK_MESSAGE)){
            printf("Hi %s, good to see you.\n", username);
            return 1;
        }
        else{
            printf("Failed to login.\n");
            return 0;
        }
    }
}

void print_illegal_command(){
    printf("Illegal command.\n");
}

void send_list_of_courses_command(int socket, char* user_input){
    char *result[2];
    char delim[3] = " \n";
    result[0] = strtok(user_input, delim);
    result[1] = strtok(NULL, delim);
    if(result[1]){
        print_illegal_command();
        return;
    }
    send_all(socket, LIST_OF_COURSES_MESSAGE, COMMAND_MESSAGE_LENGTH);
    char course_message[MAX_COURSE_LINE_LENGTH] = {0};
    char delim1[2] = " ";
    char delim2[1] = "";
    recv_all(socket, course_message);
    while(strcmp(course_message,END_MESSAGE)){
        result[0] = strtok(course_message, delim1);
        result[1] = strtok(NULL, delim2);
        int course_number = atoi(result[0]);
        char *course_name = result[1];
        printf("%d:\t%s\n",course_number,course_name);
        for(int i=0; i<MAX_COURSE_LINE_LENGTH; i++) {course_message[i]=0; }
        recv_all(socket, course_message);
    }
}

void send_add_course_command(int socket,char* user_input){
    char *result[2];
    char delim[3] = " \n";
    result[0] = strtok(user_input, delim);
    result[1] = strtok(NULL, delim);
    if(!result[1]){
        print_illegal_command();
        return;
    }
    char* text = NULL;
    char arr[MAX_COURSE_NAME_LENGTH] = {0};
    char* arr_copy = arr;
    text = strtok(NULL, delim);
    if(!text || text[0] != '\"'){
        print_illegal_command();
        return;
    }
    int total_name_length = 0;
    while(1){
        if(!text){
            print_illegal_command();
            return;
        }
        total_name_length += strlen(text);
        if(total_name_length >= MAX_COURSE_NAME_LENGTH){
            print_illegal_command();
            return;
        }
        strncpy(arr_copy, text, strlen(text));
        arr_copy += strlen(text);
        if(text[strlen(text)-1] == '\"'){
            text = strtok(NULL, delim);
            if(!text){
                break;
            }
            else{
                print_illegal_command();
                return;
            }
        }
        total_name_length++;
        if(total_name_length >= MAX_COURSE_NAME_LENGTH){
            print_illegal_command();
            return;
        }
        arr_copy[0] = ' ';
        arr_copy++;
        text = strtok(NULL, delim);
    }

    if(!is_int(result[1])){
        print_illegal_command();
        return;
    }
    int course_number_to_add = atoi(result[1]);
    char *course_name_to_add = arr;
    if(!(0<=course_number_to_add && course_number_to_add<=9999)){
        print_illegal_command();
        return;
    }
    char command_message[108] = {0};
    if(sprintf(command_message, "%s %d %s", ADD_COURSE_MESSAGE, course_number_to_add, course_name_to_add) < 0){
        throwError();
    }
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
    char *result[3];
    char delim[3] = " \n";
    result[0] = strtok(user_input, delim);
    result[1] = strtok(NULL, delim);
    result[2] = strtok(NULL, delim);
    if(!result[1] || !result[2]){
        print_illegal_command();
        return;
    }
    char* text = NULL;
    char arr[MAX_TEXT_RATE_LENGTH] = {0};
    char* arr_copy = arr;
    text = strtok(NULL, delim);
    if(!text || text[0] != '\"'){
        print_illegal_command();
        return;
    }
    int total_rate_length = 0;
    while(1){
        if(!text){
            print_illegal_command();
            return;
        }
        total_rate_length += strlen(text);
        if(total_rate_length >= MAX_TEXT_RATE_LENGTH){
            print_illegal_command();
            return;
        }
        strncpy(arr_copy, text, strlen(text));
        arr_copy += strlen(text);
        if(text[strlen(text)-1] == '\"'){
            text = strtok(NULL, delim);
            if(!text){
                break;
            }
            else{
                print_illegal_command();
                return;
            }
        }
        total_rate_length++;
        if(total_rate_length >= MAX_TEXT_RATE_LENGTH){
            print_illegal_command();
            return;
        }
        arr_copy[0] = ' ';
        arr_copy++;
        text = strtok(NULL, delim);
    }
    if(!is_int(result[1]) || !is_int(result[2])){
        print_illegal_command();
        return;
    }
    int course_number_to_rate = atoi(result[1]);
    int rate_value = atoi(result[2]);
    char *rate_text = arr;
    if((course_number_to_rate < 0 || course_number_to_rate > 9999) ||(rate_value < 0 || rate_value > 100)){
        print_illegal_command();
        return;
    }
    char command_message[MAX_MESSAGE_LENGTH] = {0};
    if(sprintf(command_message, "%s %d %d %s", RATE_COURSE_MESSAGE, course_number_to_rate, rate_value, rate_text) < 0){
        throwError();
    }
    send_all(socket, command_message, strlen(command_message));
}

void send_get_rate_command(int socket, char* user_input){
    char *result[3];
    char delim[3] = " \n";
    result[0] = strtok(user_input, delim);
    result[1] = strtok(NULL, delim);
    result[2] = strtok(NULL, delim);
    if(!result[1] || !is_int(result[1]) || result[2]){
        print_illegal_command();
        return;
    }
    int course_number = atoi(result[1]);
    if(course_number < 0 || course_number > 9999){
        print_illegal_command();
        return;
    }
    char command_message[7] = {0};
    if(sprintf(command_message, "%s %d", GET_RATE_MESSAGE, course_number) < 0){
        throwError();
    }
    send_all(socket, command_message, strlen(command_message));

    char rate_message[MAX_MESSAGE_LENGTH] = {0};
    recv_all(socket, rate_message);

    char *result2[4];
    char delim2[1] = "";

    while(strcmp(rate_message,END_MESSAGE)){
        result2[0] = strtok(rate_message, delim);
        result2[1] = strtok(NULL, delim);
        result2[2] = strtok(NULL, delim);
        result2[3] = strtok(NULL, delim2);
        char *username = result2[1];
        int rate_value = atoi(result2[2]);
        char *rate_text = result2[3];
        printf("%s:\t%d\t%s",username, rate_value, rate_text);
        for(int i=0; i<MAX_MESSAGE_LENGTH; i++) {rate_message[i]=0; }
        recv_all(socket, rate_message);
    }
}

void send_quit_command(int socket, char* user_input, int* session_is_alive){
    char *result[2];
    char delim[3] = " \n";
    result[0] = strtok(user_input, delim);
    result[1] = strtok(NULL, delim);
    if(result[1]){
        print_illegal_command();
        return;
    }
    send_all(socket, QUIT_MESSAGE, strlen(QUIT_MESSAGE));
    *session_is_alive = 0;
}

void handle_command(int socket, char* user_input, int* session_is_alive){
    char command_prefix[MAX_MESSAGE_LENGTH] = {0};
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
    //validate the arguments.
    char* server_hostname = LOCALHOST;
    int server_port = 1337;
    if(argc <= 3){
        if(argc > 1){
            server_hostname = argv[1];
        }
        if(argc > 2){
            if(is_int(argv[2])) {
                server_port = atoi(argv[2]);
            }
            else{
                throwInvalidArguments();
            }
        }
    }
    else{
        throwInvalidArguments();
    }

    //create new socket.
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock < 0){throwError();}

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    //convert hostname string to ip address.
    struct hostent *lh = gethostbyname(server_hostname);
    if(inet_pton(AF_INET, lh->h_name, &server_addr.sin_addr) < 0){throwError(); }

    //connect to the socket.
    if(connect(sock, (struct sockaddr*) &server_addr, sizeof(struct sockaddr)) < 0){throwError(); }

    //authorize.
    char message[MAX_MESSAGE_LENGTH] = {0};
    recv_all(sock, message);
    printf("%s\n", message);
    while(!authorize(sock));

    //manage a session with the server.
    int session_is_alive = 1;
    while(session_is_alive){
        char user_input[MAX_MESSAGE_LENGTH] = {0};
        fgets(user_input, MAX_MESSAGE_LENGTH, stdin);
        handle_command(sock, user_input, &session_is_alive);
    }

    close(sock);
}