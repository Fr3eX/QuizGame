#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h> // For byte ordering

#include "Utility.h"
#include "config_parser.h"


#define MIN_PORTNUMBER 1200
#define MAX_PORTNUMBER 50000

unsigned int __InitServer()
{
	char* address=(char*)__Get("SERVER_IP");
	char* backlog_str=(char*)__Get("SERVER_BACKLOG");
	char* port_str=(char*)__Get("SERVER_PORT");
	struct sockaddr_in server_address;
	unsigned int server_fd;
	unsigned int port,backlog;
	
	
	if(!port_str)
		bail("[SERVER] Cannot find port number in configuration file");

	if(!address)
		bail("[SERVER] Cannot find ip addres in configuration file");

	if(!backlog_str)
		bail("[SERVER] Cannot find backlog number in configuration file");
			
	
	port=atoi(port_str);
	backlog=atoi(backlog_str);
		
	

	/***** Zeroing SOCKADDR_STRUCT *****/
	Bzero(&server_address,sizeof(server_address));


	/**** Form IPV4 ADDRESS ****/
	server_address.sin_family=AF_INET;

	if( port < MIN_PORTNUMBER || port > MAX_PORTNUMBER )
		bail("[SERVER] Invalid port number");

	server_address.sin_port=htons(port);

	if(!inet_aton(address,&server_address.sin_addr))
		bail("[SERVER] Bad Address");	


	/*** Creating socket ****/

	server_fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	if(server_fd < 0 )
	{
		bail("socket()");	
	}

	/*** Bind the socket *****/
	if(bind(server_fd,(struct sockaddr*)&server_address,sizeof(struct sockaddr_in))<0)
		bail("bind()");
	else
		fprintf(stdout,"[SERVER] Bindind Server to %s:%s\n",address,port_str);


	/****** Make it a listening socket *******/

	if(listen(server_fd,backlog)<0)
		bail("listen()");

	return server_fd;
}


