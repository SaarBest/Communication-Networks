#include "server.h"

#define MAX_NUM_OF_USERS 25
#define MAX_CRED_LENGTH 16
#define MAX_COURSE_NAME_LENGTH 101
#define MAX_TEXT_RATE_LENGTH 1025
#define MAX_MESSAGE_LENGTH 2000

int set_users_credentials_and_return_actual_number_of_users(char* users_file_path, char** usernames, char** passwords){
    FILE* fp;
    fp = fopen(users_file_path, "r");
    if(fp == NULL){
        throwError();
    }
    int num_of_users=0;
    while(!feof(fp)){
        fscanf(fp, "%s\t%[^\n]", usernames[num_of_users], passwords[num_of_users]);
        num_of_users++;
    }
    return num_of_users;
}

int authorize_user(char* user_creds_input, char** usernames, char** passwords){
    char* username[MAX_CRED_LENGTH] = {0};
    char* password[MAX_CRED_LENGTH] = {0};
    if(sscanf(user_creds_input, "User: %s\nPassword: %s", username, password) != 2){
        return -1;
    }
    for(int i=0;i < MAX_NUM_OF_USERS; i++){
        if(!strcmp(username, usernames[i]) && !strcmp(password, passwords[i])){
            return i;
        }
    }
    return -1;
}

int main(int argc, char* argv[]){
    char* users_file_path = argv[1];
    char* data_dir_path = argv[2];
    int server_listening_port = 1337;
    if(argc == 4){
        server_listening_port = atoi(argv[3]);
    }

    char usernames[MAX_NUM_OF_USERS][MAX_CRED_LENGTH];
    char passwords[MAX_NUM_OF_USERS][MAX_CRED_LENGTH];
    for(int user=0; user < MAX_NUM_OF_USERS; user++){
        for(int i=0; i < MAX_CRED_LENGTH; i++){
            usernames[user][i] = 0;
            passwords[user][i] = 0;
        }
    }
    int actual_number_of_users =
            set_users_credentials_and_return_actual_number_of_users(users_file_path, usernames, passwords);

    int sock = socket(PF_INET, SOC_STREAM, 0);
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_listening_port);
    server_addr.sin_addr = htonl(INADDR_ANY);

    bind(sock, &server_addr, sizeof(server_addr));

    listen(sock, 10);

    char* hello_message = "Welcome! Please log in.";
    char* failed_login_message = "Failed to login.";
    int sin_size = sizeof(struct socaddr_in);
    while(1){
        int new_sock = accept(sock, (struct sockaddr*) &client_addr, &sin_size);
        send_all(new_sock, hello_message, strlen(hello_message));
        int user_index;
        while(1){
            char credentials_input[256] = {0};
            recv_all(new_sock, credentials_input);
            user_index = authorize_user(credentials_input, usernames, passwords);
            if(user_index != -1){
                char* login_success_message[37] = {0};
                sprintf(login_success_message, "Hi %s, good to see you.", usernames[user_index]);
                send_all(new_sock, login_success_message, strlen(login_success_message));
                break;
            }
            else{
                send_all(new_sock, failed_login_message, strlen(failed_login_message));
            }
        }
        int session_is_alive = 1;
        while(session_is_alive){
            char* client_input[MAX_MESSAGE_LENGTH] = {0};
            recv_all(new_sock, client_input);
        }
    }
}
