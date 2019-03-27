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
#include <psdk_inc/_ip_types.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

#define MAX_NUM_OF_USERS 25
#define MAX_NUM_OF_COURSES 10000
#define MAX_COURSES_NUMBER_LENGTH 6
#define MAX_COURSE_ID_LENGTH 5
#define MAX_CRED_LENGTH 16
#define MAX_COURSE_NAME_LENGTH 101
#define MAX_COURSE_LINE_LENGTH 106
#define MAX_TEXT_RATE_LENGTH 1025
#define MAX_MESSAGE_LENGTH 2000
#define RATES_FILE_NAME "rates.txt"

struct Course{
    int course_id;
    char course_name[MAX_COURSE_NAME_LENGTH];
};

#endif //COMMUNICATION_NETWORKS_SERVER_H
