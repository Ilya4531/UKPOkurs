#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <string>

using namespace std;

#define DEFAULT_PORT 27015


int main() {
	WSADATA wsaData;
	SOCKET listener;

	struct sockaddr_in ServerAddr;
	struct timeval udateTime;

	int sendToServer, readFromServer, timeout;
	char msg[1024], path[1024], name[1024], ext[10];
	int RetCode, server_socket, activity, maxDescriptor;

	fd_set readSocket;
	char* systemString1 = "findstr /s /i /m /p /c:\"";
	char* systemString2 = "\" *.* > text.txt";
	ifstream file;
	string temp;


	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) printf("Error at WSAStartup() \n");
	else 
	
	listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


	if (listener == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(DEFAULT_PORT);
	ServerAddr.sin_addr.s_addr = inet_addr("192.168.43.121");
	
	RetCode = connect(listener, (SOCKADDR*) &ServerAddr, sizeof(ServerAddr));
	
	if (RetCode != 0) 
	{
		printf("Client: connect failed! Error code: %ld\n", WSAGetLastError());
		closesocket(listener);
		WSACleanup();
		system("PAUSE >> void");
		return -1;
	}
	

	else 
	{
		printf("Client: connect() is OK, got connected...\n");
		printf("Client: Ready for sending and/or receiving data...\n");
		
	}

	readFromServer = recv(listener, msg, sizeof(msg), 0);
	printf("Server:\n %s\n", msg);
	
	udateTime.tv_sec = 2; udateTime.tv_usec = 50;

	for (;;)
	{
		FD_ZERO(&readSocket);
		FD_SET(listener, &readSocket);
		maxDescriptor = listener;

		if (listener > 0)
			FD_SET(listener, &readSocket);
		if (listener > maxDescriptor)
			maxDescriptor = listener;

									   
		activity = select(maxDescriptor + 1, &readSocket, NULL, NULL, &udateTime);
		if ((activity < 0))
		{
			printf("select error");
		}

		if (activity == 0)
		{
			timeout = 200;
			for (; timeout != 0; timeout--)
			{
				if (_kbhit())
				{
					printf("Enter string: ");
					scanf("%s",msg);
					if (!strcmp("0", msg)) 
					{
						printf("Offline.\n");
						closesocket(listener);
						return 0;
					}
					else
					{
						send(listener, msg, sizeof(msg), 0);
						readFromServer = recv(listener, msg, sizeof(msg), 0);
						printf("%s\n", msg);
					}
				}
			}
		}
		else {
			readFromServer = recv(listener, msg, sizeof(msg), 0);
			char *sum = new char[strlen(systemString1) + strlen(msg) + strlen(systemString2) + 1];
			strcpy(sum, systemString1);
			strcat(sum, msg);
			strcat(sum, systemString2);
			system(sum);
			delete sum;
			file.open("text.txt");
			strcpy(msg, "");
			while (!file.eof()) {
				getline(file, temp);
				strcat(msg, temp.c_str());
				strcat(msg, "\n");
			}

			file.close();
			send(listener, msg, sizeof(msg), 0);
		}
	}

	closesocket(listener);
	
	return 0;
}



