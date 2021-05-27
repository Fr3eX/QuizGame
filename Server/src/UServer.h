#ifndef USERVER_H
#define USERVER_H

#define MSG_SIZE 700


typedef enum
{
	WELC=100,//WELCOME MESSAGE

	
	LOG=99,	// CLIENT WANTS LOGIN
	LOG_SUCC=102, // LOGIN SUCCESSFULLY
	LOG_UNCC=103, // LOGIN UNSUCCESSFULLY
	LOGOUT=200,

	REG=98, // CLIENT WANTS REG
	REG_SUCC=104, //REG SUCCESSFULLY
	REG_UNCC=105, //REG UNSUCCESSFULLY
	USER_EXIST=101, //REG ERROR USER EXIST
	
	QUIZ_RU=91,//CLIENT REQUEST HIS QUIZZES CODES
	QUIZ_R=92, // CLIENT REQUEST FOR PASSING QUIZ
	QUIZ_C=97, // CLIENT WANTS CREATE QUIZ
	QUIZ_404=108, // QUIZ NOT FOUND
	QUIZ_OK=109, // QUIZ CREATED
	QUIZ_ANS=200, //CLIENT SEND QUIZ RESULTS
	QUIZ_CORR=220,	
	QUIZ_UNCORR=210,	
	
	STATUS=112, // CLIENT WANT SEE HIS STATUS (QUIZZE THAT HE PASSES)
	STATUS_OK=110, // STATUS FOUND
	STATUS_NOTOK=111, // SATUS EMPTY
	ERROR=150 // OTHER ERRORS
}PACKET_TYPE;

typedef struct PACKET
{
	PACKET_TYPE type;
	char message[MSG_SIZE];	
}PACKET;


unsigned int __InitServer();

void __SendWELCM(int);
void __HandleMessage(int,PACKET*);
int RECEIVE_PACKET(int,PACKET*);
#endif