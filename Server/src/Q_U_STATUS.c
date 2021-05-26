#include <stdio.h>
#include <stdlib.h>


#include <mysql/mysql.h>

#define CLIENT
#include "User.h"

#include "Database.h"
#include "Utility.h"
#include "Q_U_STATUS.h"
#include "Quiz.h"





static char* SQL_SELECT="SELECT userId,quizId,mark from Q_U_STATUS where userId=%d";
static char* SQL_INSERT="INSERT INTO Q_U_STATUS(userId,quizId,mark) values (%d,%d,%d)";
static char* SQL_DELETE="DELETE from Q_U_STATUS where id=%d";



bool __PersistQ_U_STATUS(MYSQL* con,Q_U_status* status)
{
	if(!con)
	{	
		print_logerr("[QUSTATUS:PERSISTE] Connection has not been initialized");
		return false;	
	}

	char buffer[BUFFER_LENGTH];
	Bzero(buffer,sizeof buffer);		

	sprintf(buffer,SQL_INSERT,status->userId,status->quizId,status->mark);


	if(mysql_query(con,buffer))
	{
		__PrintDError("{ADD_QUIZ} Error while executing insert statement");
		return false;
	}	

	return true;	
}



Q_U_status* __GetQUStatus(MYSQL* con,unsigned int id)
{
	if(!con)
	{	
		print_logerr("[QUSTATUS:SELECT] Connection has not been initialized");
		return false;	
	}

	Q_U_status* status=NULL;
	char buffer[BUFFER_LENGTH];
	
	
	Bzero(buffer,sizeof buffer);		
	sprintf(buffer,SQL_SELECT,id);

	MYSQL_RES* results;
	MYSQL_ROW row;

	if(mysql_query(con,buffer))
	{
		__PrintDError("{ADD_QUIZ} Error while executing insert statement");
		return false;
	}	
	
	
	results=mysql_store_result(con);		
	if(!results)	
	{
		__PrintDError("{SELECT_QUSTATUS} Error while executing select statement");
		return NULL;
	}

	while((row=mysql_fetch_row(results)))
	{
		__AddQStatus(&status,atoi(row[0]),atoi(row[1]),atoi(row[2]));	
	}
	
	mysql_free_result(results);	
	
	return status;
}








































Q_U_status* __InitQUStatus(unsigned int userId,
			   unsigned int quizId,
			   unsigned int mark
				)
{


	Q_U_status* new_s=malloc(sizeof(Q_U_status));

	if(!new_s)
	{
		print_logerr("[MEMORY] Not Enough Memory for Q_U_status");
		return NULL;	
	}

	new_s->userId=userId;
	new_s->quizId=quizId;
	new_s->mark=mark;

	new_s->next=NULL;
	
	return new_s;
}



Q_U_status * __AddQStatus(Q_U_status **status,unsigned int userId,
					      unsigned int quizId,
					      unsigned int mark)
{
	if(!*status)
	{
		*status=__InitQUStatus(userId,quizId,mark);
		return *status;
	}

	Q_U_status* pointer=*status;
	Q_U_status* new_s=__InitQUStatus(userId,quizId,mark);

	while(pointer->next)
		pointer=pointer->next;
	
	pointer->next=new_s;

	return new_s;
}


void __FreeQUS(Q_U_status** status)
{
	if(!*status)
		return;

	__FreeQUS(&(*status)->next);
	free(*status);
	*status=NULL;
}


void __PrintQUStatus(Q_U_status* q)
{
	while(q)
	{
		printf("UserId=%d\nQuizId=%d\nMark=%d\n\n\n",q->userId,q->quizId,q->mark);
		q=q->next;
	}

	printf("\n\n\n");
}

