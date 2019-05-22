//
// Created by Saar on 26/03/2019.
//

#ifndef COMMUNICATION_NETWORKS_SERVER_H
#define COMMUNICATION_NETWORKS_SERVER_H

#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#include "utils.h"

#define MAX_NUM_OF_USERS 25
#define MAX_NUM_OF_COURSES 10000
#define RATES_FILE_NAME "rates.txt"
#define WELCOME_MESSAGE "Welcome! Please log in."
#define NEW_COURSE_ADDED_MESSAGE "A new course was just added!"

//data structure for maintaining the courses list.
struct Course{
    int course_id;
    char course_name[MAX_COURSE_NAME_LENGTH];
};

#endif //COMMUNICATION_NETWORKS_SERVER_H
