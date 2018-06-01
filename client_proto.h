#ifndef CLIENT_PROTO_H
#define CLIENT_PROTO_H

#include <stdio.h>
#include <string.h>   
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


void init_first();

void init_second();

int exec_test1();

int exec_test2();

int exec_test3();

#endif