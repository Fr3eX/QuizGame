#ifndef SERVER_H
#define SERVER_H

#pragma pack(1)

typedef enum
{
	USER_EXIST,
	

	LOG_SUCC,

	REG_SUCC,
	REG_UNCC,
	
	QUIZ_CREATED,
	QUIZ_EXIST,	
	QUIZ_404,

	STATUS_OK,
	STATUS_NOTOK

}PACKET_TYPE;

typedef struct PACKET
{
	PACKET_TYPE type;			
}PACKET;


unsigned int __InitServer();




#endif
