#include <stdio.h>
#include <string.h>   
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

  
#define PORT 27015
 
struct Clients
{
    int client_socket;
   
};

int main()
{
     struct timeval timeout;      
        timeout.tv_sec = 20;
     timeout.tv_usec = 0;
    int  addrlen , maxClients = 10;
    int listener , newConnection, activity, tempSocket,maxDescriptor,readFromSocket;
    struct sockaddr_in address;
    struct  Clients myClient[maxClients];
    char helloStr[] ="Hello!";
    char info[1024];
    char msg[1024];  

    fd_set readSocket;
  
    for (int i = 0; i < maxClients; i++) 
    {
        myClient[i].client_socket = 0;
    }
      
    
    if( (listener = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
    {
        perror("Socket error.");
        exit(EXIT_FAILURE);
    }

    setsockopt (listener, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(timeout));

  
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);// IP-адрес (htonl-порядок хоста через любой интерфейс).
    address.sin_port = htons(PORT);//Порт(сетевой порядок).
    addrlen = sizeof(address);
      
    //Привязка сокета к адресу.
    if (bind(listener, (struct sockaddr *)&address, sizeof(address))<0) 
    {
        perror("Bind error.");
        exit(EXIT_FAILURE);
    }

    if (listen(listener, 10) < 0)//Очередь запросов (10)
    {
        perror("Listen error.");
        exit(EXIT_FAILURE);
    }
      


    printf("Waiting for connections ...\n");     
    for(;;)
    {
        FD_ZERO(&readSocket);
        FD_SET(listener, &readSocket);
        maxDescriptor = listener; 
        for (int i = 0 ; i < maxClients ; i++) 
        {
            tempSocket = myClient[i].client_socket;
            if(tempSocket > 0)
                FD_SET( tempSocket , &readSocket);
             
            if(tempSocket > maxDescriptor)
                        maxDescriptor = tempSocket;
            }

        
        activity = select(maxDescriptor + 1 , &readSocket , NULL , NULL , NULL);
    
        if ((activity < 0) && (errno!=EINTR)) 
        {
            printf("Select error.");
        }
            
        if (FD_ISSET(listener, &readSocket)) 
        {
            if ((newConnection = accept(listener, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
                
            printf("User connect.\n");
            send(newConnection, helloStr,sizeof(helloStr),0);
            
            

            
            for (int i = 0; i < maxClients; i++) 
            {
                if( myClient[i].client_socket == 0 )
                {
                    myClient[i].client_socket = newConnection;
                    
                    break;
                 }
            }
            
          
        }
        else 
        {   
            for (int i = 0; i < maxClients; i++) 
            {
                tempSocket = myClient[i].client_socket;
                if (FD_ISSET( tempSocket , &readSocket)) 
                {
                    
                    if ((readFromSocket = recv(tempSocket , msg, sizeof(msg), 0)) == 0)
                    {
                        getpeername(tempSocket , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                        printf("Host disconnected , IP: %s , PORT: %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                                    
                        
                        close( tempSocket );
                        myClient[i].client_socket= 0;
                        break;
                    }

                    else 
                    {
                        readFromSocket = recv(listener, msg, sizeof(msg), 0);
                        printf("IP: %s MSG: %s\n",inet_ntoa(address.sin_addr),msg);      
                        strcpy(info,msg);
                        info[sizeof(info)]='\0';
                       
                        for (int k = 0; k < maxClients; k++) 
                        {
                            if(( myClient[k].client_socket != 0 )&&( myClient[k].client_socket != tempSocket ))
                            {
                                printf("Send msg %s\n",info);
                                send(myClient[k].client_socket,info,sizeof(info),0);
                            }
                        }
                    }
                }
            }
        }
    }
    close(listener);
    return 0;
} 
