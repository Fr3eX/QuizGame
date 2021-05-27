#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <ctype.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "Utility.h"
#include "Client.h"
#include "Color.h"
#include "Quiz.h"
#include "Quiz_r.h"
#include "Quiz_q.h"

int main(int argv,char* argc[])
{

	int len_inet=sizeof(struct sockaddr_in);
	int cln_socket;


	char buffer[MSG_SIZE];	
	char *addr=NULL;
	char *port=NULL;
	char* ref;
	char* input;
	int r_byte;

	struct sockaddr_in srv_addr;
	struct hostent* hp=NULL;
	

	bool isOn=true;
	
	PACKET* packet_r=malloc(sizeof(PACKET));	
	PACKET* packet_s=malloc(sizeof(PACKET));	
	
	if(!packet_r || !packet_s)
		bail("[CLIENT] Not enough Memory for packet");

	/**** Zeroing Server address and msg bytes ****/
	Bzero(&srv_addr,sizeof (struct sockaddr_in));
	Bzero(packet_r,sizeof(PACKET));	
	Bzero(packet_s,sizeof(PACKET));	
	

	/**** Forming Server address ****/
	srv_addr.sin_family=AF_INET;
	
	if(argv>=3)
	{
		if(isdigit(argc[1][0]))
		{
			addr=argc[1];
		}
		else
		{
			
			if(!(hp=gethostbyname(argc[1])))
				print_logerr(hstrerror(h_errno));
			else
			{
				if(!hp->h_addr_list[0])
					bail("[CLIENT] Cannot find address ");
				else
					addr=inet_ntoa(*(struct in_addr*)hp->h_addr_list[0]);
			}
		}
	
		if(isdigit(argc[2][0]))
		 	port=argc[2];		
		else
			bail("[CLIENT] Port error");
	}
	else
	{
		fprintf(stderr,"Usage : %s <hostname> <port number>\n",argc[0]);
		return EXIT_FAILURE;
	}

	srv_addr.sin_family=(!hp)?AF_INET:hp->h_addrtype;
	srv_addr.sin_port=htons(atoi(port));	
	srv_addr.sin_addr.s_addr=inet_addr(addr);	
	

	if(srv_addr.sin_addr.s_addr==INADDR_NONE)
		bail("[CLIENT] Bad Address");

	/******************** Creating a socket ***********************/
	cln_socket=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(cln_socket<0)
		bail("socket()");
	/********************* Connect to the server *******************/

	if(connect(cln_socket,(struct sockaddr*)&srv_addr,sizeof(struct sockaddr_in))<0)
		bail("connect()");
		

	if(RECEIVE_PACKET(cln_socket,packet_r) > 0)
		fprintf(stdout,"%s\n",packet_r->message);
	
	int choix;
	bool logged=false;
	char username[22];
	char password[22];
	
	while (isOn)
	{
		/****** Zeroing Message bytes *****/	
		Bzero(packet_r,sizeof(PACKET));	
		Bzero(packet_s,sizeof(PACKET));	
		Bzero(buffer,sizeof(PACKET));	
		
		do
		{
			fprintf(stdout,"1-Login\n2-Register\n3-Exist\nchoix->: ");
			scanf("%d",&choix);	
			freeBuffer();
		}while(choix !=1 && choix!=2 && choix!=3);

		if(choix == 1)
		{	
RETRY:
			Bzero(password,22);	
			Bzero(buffer,sizeof(PACKET));	
			Bzero(username,22);	
			fprintf(stdout,"Username : ");
			read_str(username,20);
			
			fprintf(stdout,"Password : ");
			read_str(password,20);
		
				
			strcat(username,";");
			strcat(buffer,username);
			strcat(buffer,password);
		
			packet_s->type=LOG;
			strcpy(packet_s->message,buffer);			
			SEND_PACKET(cln_socket,packet_s);	
			RECEIVE_PACKET(cln_socket,packet_r);		
				
			switch (packet_r->type)
			{
				case LOG_SUCC:
					logged=true;
					printf(GREEN_N"Log successfully,redirection ..\n"RESET);
					sleep(2);
					system("clear");
					break;
				case LOG_UNCC:
					printf(RED_N"Login or password incorrect\n"RESET);
					sleep(2);
					system("clear");
					goto RETRY;		
					break;	
				default :
					fprintf(stdout,RED_N"Internal error try again\n"RESET);
					sleep(2);
					system("clear");
					goto RETRY;		
			}		
		}
		else if (choix == 3)
		{
			fprintf(stdout,"Have a nice day\n\n");
			close(cln_socket);
			free(packet_s);
			free(packet_r);
			return EXIT_SUCCESS;

		}
		else
		{

			Bzero(password,22);	
			Bzero(buffer,sizeof(PACKET));	
			Bzero(username,22);	
			fprintf(stdout,"Username : ");
			read_str(username,20);
			
			fprintf(stdout,"Password : ");
			read_str(password,20);
		
			strcat(username,";");
			strcat(buffer,username);
			strcat(buffer,password);
		
			packet_s->type=REG;
			strcpy(packet_s->message,buffer);			
			SEND_PACKET(cln_socket,packet_s);	
			RECEIVE_PACKET(cln_socket,packet_r);		
				
			switch (packet_r->type)
			{
				case REG_SUCC:
					printf(GREEN_N"REG successfully,redirection to lobby..\n"RESET);
					sleep(2);
					system("clear");
					break;
				case REG_UNCC:
					printf(RED_N"Username already exist\n"RESET);
					sleep(2);
					system("clear");
					break;	
				default :
					fprintf(stdout,RED_N"Internal error try again\n"RESET);
					sleep(2);
					system("clear");
			}	
		}	

		while(logged)
		{
HERE:			
			Bzero(packet_r,sizeof(PACKET));	
			int tmp;		
			Bzero(packet_s,sizeof(PACKET));	
			Bzero(buffer,sizeof(buffer));	
			
			do
			{
				fprintf(stdout,"\n\n1-Add Quiz\n2-Join Quiz\n3-Logout\nChoix --> : ");
				scanf("%d",&choix);	
				freeBuffer();
			}while(choix !=1 && choix!=2 && choix!=3 && choix !=4);
		
			if(choix == 1)
			{
				Quiz_q* q=NULL;
				Quiz_r* ran=NULL;
				int q_count=0;
				unsigned correct = 0;
				fprintf(stdout,"Max Question number = 10 (3 answers / question )\nNombre de question : ");
				if(scanf("%d",&q_count)!=1)
				{
					fprintf(stdout,RED_N"Error while parsing number\n"RESET);
					sleep(1);
					system("clear");
					choix=-1;
					goto HERE;
				}
				else
					if(choix > 10)
					{
						choix=-1;
						fprintf(stdout,RED_N"Error while parsing number\n"RESET);
						sleep(1);
						system("clear");
						goto HERE;
					}

				freeBuffer();
				for(int i=0;i<q_count;i++)
				{
					Bzero(buffer,sizeof(buffer));	
					fprintf(stdout,"Question %d : ",i+1);
					read_str(buffer,sizeof buffer-1);
					buffer[BUFFER_LENGTH -1]='\0';
					Quiz_q* tmp=__AddQuestion(&q,1,1,1,buffer,NULL);			
					ran=NULL;	
					for(int r=0;r<3;r++)
					{
						Bzero(buffer,sizeof(buffer));	
						fprintf(stdout,"Réponse %d : ",r+1);
						read_str(buffer,sizeof buffer-1);
						fprintf(stdout,"IsCorrect ?(1 or 0) : ");
						if(scanf("%d",&choix)!=1)
						{	
							freeBuffer();
							fprintf(stdout,RED_N"Error while parsing number\n"RESET);
							__FreeQuiz_q(&q);
							choix=-1;
							sleep(1);
							system("clear");	
							goto HERE;
						}
						freeBuffer();
						__AddAnswer(&ran,1,1,choix,buffer);
					}
					tmp->answers=ran;
				}

				unsigned nombre_question=QstCount(q);	
				
				Bzero(packet_s,sizeof(PACKET));	
				packet_s->type=QUIZ_C;		
				sprintf(packet_s->message,"%d",nombre_question);
				
				SEND_PACKET(cln_socket,packet_s);	
						
				Quiz_q* p_q=q;
				Quiz_r* p_r=NULL;	

				while(p_q)
				{
					Bzero(packet_s,sizeof(PACKET));	
					p_r=p_q->answers;	
					strcpy(packet_s->message,p_q->question);
					SEND_PACKET(cln_socket,packet_s);
					while(p_r)
					{
						Bzero(packet_s,sizeof(PACKET));	
						strcpy(packet_s->message,p_r->answer);
						if(p_r->isCorrect)
							packet_s->type=QUIZ_CORR;
						else
							packet_s->type=QUIZ_UNCORR;
						
						SEND_PACKET(cln_socket,packet_s);
						p_r=p_r->next;
					}
					
					p_q=p_q->next;
				}
				
				
				int code;	
				RECEIVE_PACKET(cln_socket,packet_r);	
				if(packet_r->type == ERROR)
				{
					fprintf(stdout,RED_B"\n\nError occured try again\n"RESET);
					goto HERE;
				}
				else
			 	{
					code=atoi(packet_r->message);
					fprintf(stdout,GREEN_B"\n\nYour Quiz code : %d\n\n\n"RESET,code);
					fprintf(stdout,"Print any character to exist...\n\n\n");	
					getchar();
				}	
				
					
				system("clear");
				
				__FreeQuiz_q(&q);
			}
			else if(choix == 2)
			{					
				Bzero(packet_r,sizeof(PACKET));	
				Bzero(buffer,sizeof(buffer));	
				do
				{
					fprintf(stdout,"Entrer -1 pour retourner\nEntrer code :");
					tmp=scanf("%d",&choix);
					freeBuffer();
				}while(tmp!=1);	
			
				if(choix == -1)
					system("clear");
				else
				{
					Bzero(packet_r,sizeof(PACKET));	
					Bzero(buffer,sizeof(buffer));	
					
					packet_s->type=QUIZ_R;
					sprintf(packet_s->message,"%d",choix);
					
					SEND_PACKET(cln_socket,packet_s);	
					
					RECEIVE_PACKET(cln_socket,packet_r);
					switch(packet_r->type)
					{
						case QUIZ_OK:
							
							RECEIVE_PACKET(cln_socket,packet_r);
							int r;
							unsigned int nbr_question=atoi(packet_r->message);		
							
							
							Quiz_q* questions=NULL;
							Quiz_r* answers=NULL;
							for(int i=0;i<nbr_question;i++)
							{
								RECEIVE_PACKET(cln_socket,packet_r);		
								Quiz_q* tmp =__AddQuestion(&questions,1,1,1,packet_r->message,NULL);				
								answers=NULL;
								for(r=0;r<3;r++)
								{
									RECEIVE_PACKET(cln_socket,packet_r);		
									if(packet_r->type == QUIZ_CORR)
										__AddAnswer(&answers,1,1,1,packet_r->message);	
									else
										__AddAnswer(&answers,1,1,0,packet_r->message);	

								}
								tmp->answers=answers;	
							}
							
							
							fprintf(stdout,GREEN_N"\n\nQuiz Found\n"RESET);
							sleep(2);
							system("clear");

							unsigned int score=0;
							while(questions)
							{
								unsigned int count=0;
								unsigned char tab[3]={0};
								fprintf(stdout,"\n\n-->Your score : %d \n\n\n",score);
								fprintf(stdout,"%s  ? \n",questions->question);
								
								while(questions->answers)
								{
									fprintf(stdout,"%d) - %s\n",++count,questions->answers->answer);	
									tab[count-1]=questions->answers->isCorrect;
									questions->answers=questions->answers->next;	
								}
								
								do
								{
									fprintf(stdout,"Reponse : ");
									tmp=scanf("%d",&choix);
									freeBuffer();
								}while(tmp!=1);
								if(choix == 1)
								{
									if(tab[0])
										score+=1;	
								}
								else if (choix == 2)
								{
									if(tab[1])
										score+=1;	
								}
								else if(choix==3)
								{
									if(tab[2])
										score+=1;	
								
								}
								else
								{
									score+=0;	
								}
								system("clear");
								questions=questions->next;						
							}
							if(score > (nbr_question/2))
								fprintf(stdout,GREEN_B"\n\nYour final score is : %d\n"RESET,score);
							else
								fprintf(stdout,RED_B"\n\nYour final score is : %d \n"RESET,score);
							
							fprintf(stdout,"Print any character to go back..\n\n");	
							getchar();
							system("clear");
							__FreeQuiz_q(&questions);
							break;
						case QUIZ_404:
							fprintf(stdout,"\n\n"RED_N"Quiz Not Found\n"RESET);
							sleep(2);
							system("clear");
							break;
						default:
							fprintf(stdout,"\n\n"RED_N"Internal error try again type %d\n"RESET,packet_r->type);
							sleep(2);
							system("clear");
							break;
					}	
				
				}		
			
			}
			else
			{
				logged=false;
				packet_s->type=LOGOUT;
				printf(RED_N"Login out ..\n"RESET);
				sleep(2);
				system("clear");
				SEND_PACKET(cln_socket,packet_s);
			}		
		
		}
		
	}
	return 0;
}




int RECEIVE_PACKET(int sock,PACKET* packet_r)
{
	Bzero(packet_r,sizeof(PACKET));
	int r_byte=(int)recv(sock,packet_r,sizeof(PACKET),0);	
	if(!r_byte)
	{
		bail("Peer shut down connection");
		close(sock);
		exit(1);	
	}
	else if(r_byte < 0)
		print_logerr("[CLIENT] Read PACKET error");	

	return r_byte;
}


int SEND_PACKET(int sock,PACKET* packet_s)
{
	int s_byte=(int)send(sock,packet_s,sizeof(PACKET),0);	
	if(!s_byte)
	{
		bail("Peer shut down connection");
		close(sock);
		exit(1);	
	}
	else if(s_byte < 0)
		print_logerr("[CLIENT] Read PACKET error");	

	return s_byte;
}





void freeBuffer()
{
	char c=0;
	while(c!='\n' && c!=EOF)c=getchar();
}


int read_str(char *buffer, int length)
{
    char *ptr= NULL;
 
    if (fgets(buffer,length, stdin) != NULL)
    {
        ptr = strchr(buffer, '\n');
        if (ptr!= NULL)
        {
            *ptr = '\0';
        }
        else
        {
            freeBuffer();
        }
        return 1;
    }
    else
    {
        freeBuffer();
        return 0;
    }
}



