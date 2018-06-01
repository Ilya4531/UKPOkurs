#include "client_proto.h"

#include <stdio.h>
#include <string.h>   
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int first_sock,second_sock;
struct sockaddr_in first_addr,second_addr;
char first_buf[1024],second_buf[1024];
int first_send,second_send,first_recv,second_recv;
char* sample = "Hello";
char* service_str1 = "grep -w \"";
char* service_str2 = "\" *.txt > results.txt";
ifstream file;
string temp;

void init_first()
{
	if((first_sock = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("socket");
		return;
	}

	first_addr.sin_family = AF_INET;
	first_addr.sin_port = htons(27015);
	first_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(connect(first_sock,(struct sockaddr *)&first_addr, sizeof(first_addr)) < 0)
	{
		perror("connect");
		return;
	}

	first_recv = recv(first_sock,first_buf,sizeof(first_buf),0);



}


void init_second()
{
	if((second_sock = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("socket");
		return;
	}

	second_addr.sin_family = AF_INET;
	second_addr.sin_port = htons(27015);
	second_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(connect(second_sock,(struct sockaddr *)&second_addr, sizeof(second_addr)) < 0)
	{
		perror("connect");
		return;
	}

	second_recv = recv(second_sock,second_buf,sizeof(second_buf),0);



}

int exec_test1()
{
	init_first();
	close(first_sock);
	if((strcmp(first_buf,"Hello!")) == 0)
	{
		return 0;
	}

	else 
	{
		return -1;
	}
}

int exec_test2()
{
	init_first();
	init_second();
	first_send = send(first_sock,sample,sizeof(sample),0);
	second_recv = recv(second_sock,second_buf,sizeof(second_buf),0);
	close(first_sock);
	close(second_sock);
	if((strcmp(second_buf,"Hello")) == 0)
	{
		return 0;
	}

	else 
	{
		return -1;
	}

}

int exec_test3()
{
	init_first();
	init_second();
	first_send = send(first_sock,sample,sizeof(sample),0);
	second_recv = recv(second_sock,second_buf,sizeof(second_buf),0);
	char* sum = new char[strlen(service_str1) + strlen(second_buf) + strlen(service_str2)];
	strcpy(sum,service_str1);
	strcat(sum,second_buf);
	strcat(sum,service_str2);
	system(sum);
	file.open("results.txt");
	strcpy(second_buf, "");
	while (!file.eof()) 
	{
		getline(file, temp);
		strcat(second_buf, temp.c_str());
		strcat(second_buf, "\n");
	}

	file.close();
	second_send = send(second_sock, second_buf, sizeof(second_buf), 0);
	first_recv = recv(first_sock,first_buf,sizeof(first_buf),0);
	close(first_sock);
	close(second_sock);
	if((strcmp(first_buf,"2.txt:Hello\n3.txt:Hello\n\n")) == 0)
	{
		return 0;
	}
	else 
	{
		return -1;
	}

}