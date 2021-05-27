#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <time.h>
#include <arpa/inet.h> // For byte ordering

#include <mysql/mysql.h>
#define CLIENT
#include "Database.h"
#include "User.h"
#include "Utility.h"
#include "UServer.h"
#include "Holder.h"

#include "Quiz.h"
#include "Quiz_r.h"
#include "Quiz_q.h"
#include "DAOQuiz.h"
#include "DAOQuiz_r.h"
#include "DAOQuiz_q.h"

#include "config_parser.h"
#include "Color.h"

#define MIN_PORTNUMBER 1200
#define MAX_PORTNUMBER 50000



static void __SendPACKET(int,PACKET*);
static Client* __HandleLog(PACKET*);
static void __HandleReg(PACKET*);
static void __HandleQuizR(int,PACKET*);
static void __HandleQuizC(Holder*,int,PACKET*);
static void __Handle(PACKET*);


static char* WELC_MESSAGE=RED_N"Welcome Quiz Game v0.1"RESET;


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

	if(!strcmp(address,"*"))
	{
		server_address.sin_addr.s_addr=htonl(INADDR_ANY);	
	}
	else
	{	if(!inet_aton(address,&server_address.sin_addr))
			bail("[SERVER] Bad Address");	
	}

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

void __HandleMessage(int sock,PACKET* packet)
{
	Holder* holder=__GetHolderBySock(sock);
	Client* client=NULL;

	if(!holder)
	{
		print_logerr("[SERVER] Anonymous data : holder not found ");
		free(packet);
		return;
	}		
	switch(packet->type)
	{
		case LOG:
			client=__HandleLog(packet);
			if(client)
				if(__HSetClient(sock,client))
					print_log("Client inserted successfully");
			goto END;	
			break;
		case REG:
			__HandleReg(packet);	
			goto END;	
		break;
		case QUIZ_C:
			__HandleQuizC(holder,sock,packet);
		break;	
		case STATUS:
		break;		
		case QUIZ_R:
			__HandleQuizR(sock,packet);
			return;	
		break;
		case LOGOUT:
			__FreeClient(&holder->client);
			holder->client=NULL;
			return;
			break;	
		default:
		print_logerr("[SERVER] Unknown packet type");
		packet->type=ERROR;
		goto END;	
	}


END:
	__SendPACKET(sock,packet);
}

void __SendWELCM(int sock)
{
	PACKET* packet=malloc(sizeof(PACKET));
	if(!packet)
	{
		print_logerr("[SERVER] Not Enough Memory for PACKET");
		return;
	}
	Bzero(packet,sizeof(PACKET));	
	packet->type=WELC;
	strcpy(packet->message,WELC_MESSAGE);	
	__SendPACKET(sock,packet);
	
	free(packet);	
}


static void __SendPACKET(int sock,PACKET* packet)
{
	if(send(sock,packet,sizeof(PACKET),0)<0)
		print_logerr("[SERVER] Send Error");
}





static Client*  __HandleLog(PACKET* packet)
{
	char* buffer=malloc(200*sizeof(char));	
	if(!buffer)
	{
		print_logerr("[SERVER] Not Enough Memory for buffer \n");
		return NULL; 
	}
	
	strcpy(buffer,packet->message);
	
	
	Client* client;
	char username[NICKNAME];
	char password[PASSWORD];	
	
	Bzero(username,NICKNAME);
	Bzero(password,PASSWORD);
			
	char *token=strtok(buffer,";");
	if(!token)
	{
		packet->type=LOG_UNCC;
		return NULL;	
	}
	else
	{
		strcpy(username,token);
		if(token)
			token=strtok(NULL,";");
		
		if(token)
			strcpy(password,token);	
	
		client = __GetClient(username);
		if(!client)
		{
			packet->type=LOG_UNCC;
			free(buffer);
			return client;
		}
		else
		{
			if(!strcmp(client->password,password))
			{
				packet->type=LOG_SUCC;
				free(buffer);
				return client;
			}
			else
			{
				packet->type=LOG_UNCC;
				free(buffer);
				return NULL;
			}
		}			
	}
}

static void __HandleReg(PACKET* packet)
{
	char* buffer=malloc(200*sizeof(char));	
	if(!buffer)
	{
		print_logerr("[SERVER] Not Enough Memory for buffer \n");
		return ; 
	}
	
	strcpy(buffer,packet->message);
	
	
	Client* client;
	char username[NICKNAME];
	char password[PASSWORD];	
	
	Bzero(username,NICKNAME);
	Bzero(password,PASSWORD);
	char *token=strtok(buffer,";");
	if(!token)
	{
		packet->type=REG_UNCC;
		return;	
	}
	else
	{
		strcpy(username,token);
		if(token)
			token=strtok(NULL,";");
		
		if(token)
			strcpy(password,token);	
	
			
		client=__AddClient(username,password,"NULL",0,0,0,0);	
		if(client)
		{
			packet->type=REG_SUCC;
			free(buffer);
			return; 
		}
		else
		{
			packet->type=REG_UNCC;
			free(buffer);
			return;
		}			
	}

}



static void __HandleQuizR(int sock,PACKET* packet)
{
	unsigned code=atoi(packet->message); 
	
	MYSQL* con=__GetMysqlCon();
	Quiz* quiz=__GetQuiz(con,code);

		
	if(!quiz)
	{
		packet->type=QUIZ_404;
		__SendPACKET(sock,packet);
		return;
	}
	else
	{
		
		packet->type=QUIZ_OK;
		__SendPACKET(sock,packet);
		Bzero(packet,sizeof(PACKET));
			
		packet->type=QUIZ_OK;
		Quiz_q* question=__GetQuiz_q(con,quiz);	
		quiz->questions=question;
		
		unsigned nombre_quest=QstCount(quiz->questions);
		sprintf(packet->message,"%d",nombre_quest);
		__SendPACKET(sock,packet);
		unsigned i=0;
		while(question)
		{
			i=0;
			Bzero(packet,sizeof(PACKET));
			Quiz_r* res=question->answers;
			strcpy(packet->message,question->question);
			__SendPACKET(sock,packet);

			while(res)
			{
				if(i == 3)break;
				Bzero(packet,sizeof(PACKET));
				if(res->isCorrect)
					packet->type=QUIZ_CORR;
				else
					packet->type=QUIZ_UNCORR;
				strcpy(packet->message,res->answer);
				__SendPACKET(sock,packet);
				res=res->next;
				i++;
			}
			question=question->next;	
		}
		
		__FreeQuiz(&quiz);
	}
}


int RECEIVE_PACKET(int sock,PACKET* packet_r)
{
	Bzero(packet_r,sizeof(PACKET));
	int r_byte=(int)recv(sock,packet_r,sizeof(PACKET),0);	
	if(!r_byte)
		return 0;	
	else if(r_byte < 0)
		print_logerr("[SERVER] Read PACKET error");	

	return r_byte;
}


static void __HandleQuizC(Holder* holder,int sock,PACKET* packet)
{
	int nbr_question=atoi(packet->message),r;
	Quiz_q* questions=NULL;
	Quiz_r* answers=NULL;

	
	for(int i=0;i<nbr_question;i++)
	{
		Bzero(packet,sizeof(PACKET));
		RECEIVE_PACKET(sock,packet);		
		Quiz_q* tmp =__AddQuestion(&questions,1,1,1,packet->message,NULL);				
		answers=NULL;
		for(r=0;r<3;r++)
		{
			Bzero(packet,sizeof(PACKET));
			RECEIVE_PACKET(sock,packet);		
			if(packet->type == QUIZ_CORR)
				__AddAnswer(&answers,1,1,1,packet->message);	
			else
				__AddAnswer(&answers,1,1,0,packet->message);	

		}
		tmp->answers=answers;	
	}

	time_t te=time(NULL);
	struct tm * info=localtime(&te);
	
	Bzero(packet,sizeof(PACKET));
		sprintf(packet->message,"%d%d%d%d",holder->client->id,info->tm_mday,info->tm_min,info->tm_sec);

	
	unsigned int code=atoi(packet->message);	
	
	Quiz* quiz=__InitQuiz(1,holder->client->id,code,0,0,0);	
	quiz->questions=questions;	

	
	MYSQL* con=__GetMysqlCon();
	__PersistQuiz(con,quiz);	
	
	
	Quiz* qq=__GetQuiz(con,code);	
	
	if(!qq)
	{
		packet->type=ERROR;
	}
	__PersistQuiz_q(con,questions,qq->quizId);

	__FreeQuiz(&quiz);
	__SendPACKET(sock,packet);
}
