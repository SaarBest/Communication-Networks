#include "server.h"

struct Course courses[MAX_NUM_OF_COURSES];
node_t * active_clients = NULL;
int actual_num_of_courses = 0;

void set_users_credentials(char *users_file_path,
                           char usernames[MAX_NUM_OF_USERS][MAX_CRED_LENGTH],
                           char passwords[MAX_NUM_OF_USERS][MAX_CRED_LENGTH]){
    FILE* fp;
    fp = fopen(users_file_path, "r");
    if(fp == NULL){throwError();}
    int i = 0;
    while(!feof(fp)){
        fscanf(fp, "%s\t%[^\n]", usernames[i], passwords[i]);
        i++;
    }
    fclose(fp);
}

int authorize_user(char* user_creds_input, char usernames[MAX_NUM_OF_USERS][MAX_CRED_LENGTH],
                   char passwords[MAX_NUM_OF_USERS][MAX_CRED_LENGTH]){
    char username[MAX_CRED_LENGTH] = {0};
    char password[MAX_CRED_LENGTH] = {0};
    if(sscanf(user_creds_input, "%s %s", username, password) != 2){
        return -1;
    }
    for(int i=0;i < MAX_NUM_OF_USERS; i++){
        if(!strcmp(username, usernames[i]) && !strcmp(password, passwords[i])){
            return i;
        }
    }
    return -1;
}

void send_courses_list(int socket){
    for(int i=0; i < actual_num_of_courses; i++){
        char course_line_str[MAX_COURSE_LINE_LENGTH] = {0};
        if(sprintf(course_line_str, "%d %s", courses[i].course_id, courses[i].course_name) < 0){throwError();}
        send_all(socket, course_line_str, strlen(course_line_str));
    }
    send_all(socket, END_MESSAGE, strlen(END_MESSAGE));
}

void add_course_to_list(int socket, char* client_input){
/*    char course_number_str[MAX_COURSE_ID_LENGTH] = {0};
    int course_number = atoi(course_number_str);
    char course_name[MAX_COURSE_NAME_LENGTH] = {0};
    sscanf(client_input, "%s %s", course_number_str, course_name);*/
    char *result[2];
    char delim1[2] = " ";
    char delim2[1] = "";
    result[0] = strtok(client_input, delim1);
    result[1] = strtok(NULL, delim2);
    int course_number = atoi(result[0]);
    char *course_name = result[1];
    //check if this course already exists
    for(int i=0; i < actual_num_of_courses; i++){
        if(courses[i].course_id == course_number){
            send_all(socket, FAIL_MESSAGE, strlen(FAIL_MESSAGE));
            return;
        }
    }
    courses[actual_num_of_courses].course_id = course_number;
    strcpy(courses[actual_num_of_courses].course_name, course_name);
    actual_num_of_courses++;
    send_all(socket, OK_MESSAGE, strlen(OK_MESSAGE));
}

void end_user_session(int* session_is_alive){
    *session_is_alive = 0;
}

void add_course_rate(char* rates_file_path, char* client_input, char* username){
    FILE* fp = fopen(rates_file_path, "a");
    if(!fp){throwError();}
    char *result[3];
    char delim1[2] = " ";
    char delim2[1] = "";
    result[0] = strtok(client_input, delim1);
    result[1] = strtok(NULL, delim1);
    result[2] = strtok(NULL, delim2);
    int course_number = atoi(result[0]), rate_value = atoi(result[1]);
    char *rate_str = result[2];
    for(int i=0; i < actual_num_of_courses; i++){
        if(courses[i].course_id == course_number){
            if(fprintf(fp, "%d %s %d %s\n", course_number, username, rate_value, rate_str) < 0){throwError();}
            break;
        }
    }
    fclose(fp);
}

void send_course_rates(int socket, char* rates_file_path, char* client_input){
    int requested_course_number;
    sscanf(client_input, "%d", &requested_course_number);
    FILE* fp = fopen(rates_file_path, "r");
    if(!fp){throwError();}
    char line[MAX_MESSAGE_LENGTH] = {0};
    while(fgets(line, MAX_MESSAGE_LENGTH + 1, fp)){
        int course_number;
        sscanf(line, "%d", &course_number);
        if(requested_course_number == course_number){
            send_all(socket, line, strlen(line));
        }
    }
    fclose(fp);
    //sending the word end to inform there are no more rates for the specified course.
    send_all(socket, END_MESSAGE, strlen(END_MESSAGE));
}

void handle_user_command(int socket, char* rates_file_path, char* client_input, int* session_is_alive, char* username){
    char command_index_char;
    sscanf(client_input, "%c", &command_index_char);
    int command_index = command_index_char - '0';
    switch(command_index){
        case(1):
            send_courses_list(socket);
            break;
        case(2):
            //adding 2 to the client input address in order to skip the command index.
            add_course_to_list(socket, client_input + 2);
            break;
        case(3):
            //adding 2 to the client input address in order to skip the command index.
            add_course_rate(rates_file_path, client_input + 2, username);
            break;
        case(4):
            //adding 2 to the client input address in order to skip the command index.
            send_course_rates(socket, rates_file_path, client_input + 2);
            break;
        default:
            end_user_session(session_is_alive);
            break;
    }
}

int main(int argc, char* argv[]){
    //validate arguments.
    char* users_file_path = NULL;
    char* data_dir_path = NULL;
    int server_listening_port = 1337;
    if(argc == 3 || argc == 4){
        users_file_path = argv[1];
        data_dir_path = argv[2];
        if(argc == 4){
            if(is_int(argv[3])) {
                server_listening_port = atoi(argv[3]);
            }
            else{
                throwInvalidArguments();
            }
        }
    }
    else{
        throwInvalidArguments();
    }

    //create rates data file.
    char path[2000] = {0};
    if(sprintf(path, "%s/%s", data_dir_path, RATES_FILE_NAME) < 0){throwError();}
    FILE* fp = fopen(path, "w");
    if(!fp){throwError();}
    fclose(fp);

    //initialize the usernames and passwords data structures from a given file.
    char usernames[MAX_NUM_OF_USERS][MAX_CRED_LENGTH];
    char passwords[MAX_NUM_OF_USERS][MAX_CRED_LENGTH];
    for(int user=0; user < MAX_NUM_OF_USERS; user++){
        for(int i=0; i < MAX_CRED_LENGTH; i++){
            usernames[user][i] = 0;
            passwords[user][i] = 0;
        }
    }
    set_users_credentials(users_file_path, usernames, passwords);

    //create socket.
    int listening_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(listening_sock < 0){throwError();}

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_listening_port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind the server to the socket.
    if(bind(listening_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) < 0){throwError();}

    //listen to the socket.
    if(listen(listening_sock, 10) < 0){throwError();}

    fd_set client_fds;
    node_t * unauthorized_clients = NULL;

    socklen_t sin_size = sizeof(struct sockaddr_in);
    while(1){

        FD_ZERO(&client_fds);
        FD_SET(listening_sock, &client_fds);
        node_t * current = active_clients;
        while(current != NULL){
            FD_SET(current->sock, &client_fds);
            current = current->next;
        }
        current = unauthorized_clients;
        while(current != NULL){
            FD_SET(current->sock, &client_fds);
            current = current->next;
        }

        int fd_max = listening_sock;
        if(active_clients != NULL){
            int x = get_fd_max(active_clients);
            fd_max = x > fd_max ? x : fd_max;
        }

        if(unauthorized_clients != NULL){
            int x = get_fd_max(unauthorized_clients);
            fd_max = x > fd_max ? x : fd_max;
        }

        if(select(fd_max+1, &client_fds, NULL, NULL, NULL) < 0) {throwError();};

        current = unauthorized_clients;
        while(current != NULL){
            if(FD_ISSET(current->sock,&client_fds)){
                //authorize client.
                int user_index;
                char credentials_input[32] = {0};
                recv_all(current->sock, credentials_input);
                user_index = authorize_user(credentials_input, usernames, passwords);
                if(user_index != -1){
                    send_all(current->sock, OK_MESSAGE, strlen(OK_MESSAGE));
                    int current_sock = current->sock;
                    current = current->next;
                    add_to_list(&active_clients, current_sock, usernames[user_index]);
                    remove_from_list(&unauthorized_clients, current_sock);
                    FD_CLR(current_sock, &client_fds);
                }
                else{
                    send_all(current->sock, FAIL_MESSAGE, strlen(FAIL_MESSAGE));
                    current = current->next;
                }
            }
        }

        current = active_clients;
        while(current != NULL){
            if(FD_ISSET(current->sock,&client_fds)){
                int session_is_alive = 1;
                char client_input[MAX_MESSAGE_LENGTH] = {0};
                recv_all(current->sock, client_input);
                handle_user_command(current->sock, path, client_input, &session_is_alive, current->username);
                if(!session_is_alive){
                    int current_sock = current->sock;
                    current = current->next;
                    remove_from_list(&active_clients, current_sock);
                    close(current_sock);
                }
                else {current = current->next;}
            }
        }

        if(FD_ISSET(listening_sock, &client_fds)){
            //accept new client connection.
            int new_sock = accept(listening_sock, (struct sockaddr *) &client_addr, &sin_size);
            if(new_sock < 0){throwError();}
            //send welcome message.
            send_all(new_sock, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));
            add_to_list(&unauthorized_clients,new_sock, NULL);
        }
    }

    /* unreachable anyway */
    //close(listening_sock);
}
