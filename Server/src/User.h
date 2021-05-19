#ifndef CLIENT_H
#define CLIENT_H

#ifdef CLIENT

#define BUFFER_LENGTH 300

#define NICKNAME 20
#define PASSWORD 20
#define EMAIL	 20 
#define RANK	 20

#endif

typedef struct Client 
{
	unsigned id;
	char *userName;
	char *password;
	char *lastIp;
	unsigned int numberOfQuiz;
	unsigned int numberOfQuizzesPassed;
	unsigned int numberOfQuizzesPassed_S;
	unsigned int numberOfQuizzesPassed_F;
}Client;

typedef enum
{
	LAST_IP,
	NBR_Q,
	NBR_QP,
	NBR_QPS,
	NBR_QPF
}Field;

typedef union
{
	char* value_s;
	unsigned int value_i;
}FieldVal;


Client* __AddClient(char*,char*,char*,unsigned int,unsigned int,unsigned int,unsigned int);
Client* __GetClient(char const*);

void __FreeClient(Client**);

bool __ModClient(unsigned,Field field,FieldVal* val);
bool __RemoveClient(unsigned);

#endif

