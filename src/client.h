//
// Created by Saar on 26/03/2019.
//

#ifndef COMMUNICATION_NETWORKS_CLIENT_H
#define COMMUNICATION_NETWORKS_CLIENT_H

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

#define LOCALHOST "localhost"
#define COMMAND_LIST_OF_COURSES "list_of_courses"
#define COMMAND_ADD_COURSE "add_course"
#define COMMAND_RATE_COURSE "rate_course"
#define COMMAND_GET_RATE "get_rate"
#define COMMAND_QUIT "quit"
#define LIST_OF_COURSES_MESSAGE "1"
#define ADD_COURSE_MESSAGE "2"
#define RATE_COURSE_MESSAGE "3"
#define GET_RATE_MESSAGE "4"
#define QUIT_MESSAGE "5"

#define COMMAND_MESSAGE_LENGTH 2


#endif //COMMUNICATION_NETWORKS_CLIENT_H
