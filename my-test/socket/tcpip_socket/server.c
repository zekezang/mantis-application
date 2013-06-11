#include <sys/types.h>  
#include <sys/socket.h>  
#include <stdio.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <unistd.h>  
#include <stdlib.h>  
char dict(char temp)  
{  
    if(temp == 'Z')  
    {  
        return 'A';  
    }  
    return temp+1;  
}  
int main()  
{  
    int server_sockfd,client_sockfd;  
    int server_len,client_len;  
    struct sockaddr_in server_address;  
    struct sockaddr_in client_address;  
    int i,btye;  
    char char_recv,char_send;  
      
    server_address.sin_family = AF_INET;  
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");  
    server_address.sin_port = 8080;  
    server_len = sizeof(server_address);  
      
    server_sockfd = socket(AF_INET,SOCK_STREAM,0);  
      
    bind(server_sockfd,(struct sockaddr *)&server_address,server_len);  
      
    listen(server_sockfd,5);  
    printf("server waiting for connect\n");  
    
    client_len = sizeof(client_address);  
    client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address,(socklen_t *)&client_len);  
printf("server waiting for connecqqqqqqqqq1111111111111111111t\n");    
	while(1){  
    if((btye = recv(client_sockfd,&char_recv,1,0)) == -1)
    {  
        perror("recv");  
        //exit(EXIT_FAILURE);  
    }  
    printf("receive from client is %c\n",char_recv);  
    char_send = dict(char_recv);  
    if((btye = send(client_sockfd,&char_send,1,0)) == -1)
    {  
        perror("send");  
        //exit(EXIT_FAILURE);  
    }  
  }
    shutdown(client_sockfd,2);  
    shutdown(server_sockfd,2);   
    return 0;  
}
