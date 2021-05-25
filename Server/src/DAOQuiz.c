#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <mysql/mysql.h>
#include "Utility.h"
#include "Database.h"

#include "Quiz.h"
#include "Quiz_r.h"
#include "Quiz_q.h"
#include "DAOQuiz_q.h"
#include "DAOQuiz_r.h"
#include "DAOQuiz.h"


#define CLIENT
#include "User.h"


static char* SQL_INSERT="INSERT INTO Quiz (quizOwner,quizCode,NBR_QP,NBR_QPS,NBR_QPF) values(%d,%d,%d,%d,%d)";
static char* SQL_DELETE="DELETE from Quiz where id=%d"; 
static char* SQL_UPDATE="UPDATE Quiz SET %s=%d where id=%d";
static char* SQL_SELECT_CODE="SELECT id,quizOwner,quizCode,NBR_QP,NBR_QPS,NBR_QPF from Quiz where quizCode=%d"; 

static char* SQL_SELECT_OWNER="SELECT id,quizOwner,quizCode,NBR_QP,NBR_QPS,NBR_QPF from Quiz where quizOwner=%d"; 



bool __PersistQuiz(MYSQL* con,Quiz* quiz)
{
	if(!con)
	{	
		print_logerr("[QUIZ:PERSISTE] Connection has not been initialized");
		return false;	
	}

	char buffer[BUFFER_LENGTH];
	
	while(quiz)
	{
		Bzero(buffer,sizeof buffer);	
		sprintf(buffer,SQL_INSERT,quiz->quizOwner,quiz->quizCode,quiz->qp_number,
				quiz->qs_number,
				quiz->qf_number);			
	
		if((quiz=__GetQuiz(con,quiz->quizCode)))
		{
			print_log("[ADD_QUIZ] : DUPLICATE QUIZECODE");
			__FreeQuiz(&quiz);
				return false;
		}
	
		if(mysql_query(con,buffer))
		{
			__PrintDError("{ADD_QUIZ} Error while executing insert statement");
			return false;
		}
		quiz=quiz->next;
	}
	return true;
}

bool __RemoveQuiz(MYSQL* con,unsigned id)
{
	if(!con)
	{
		print_logerr("[DAOQUIZ_Q:DELETE] Connection has not benn initialized");	
		return false;	
	}	
	
	char buffer[QBUFFER_LENGTH];
	Bzero(buffer,sizeof buffer);
	
	sprintf(buffer,SQL_DELETE,id);


	if(mysql_query(con,buffer))
	{
		__PrintDError("{ADD_Q} Error while executing insert statement");
		return false;
	}

	return true;
}


bool __UpdateQuiz(MYSQL* con,Field_quiz field,unsigned int val,Quiz* quiz)
{
	if(!con)
	{	
		print_logerr("[QUIZ:GETALL] Connection has not been initialized");
		return NULL;	
	}


	char buffer[BUFFER_LENGTH];
	Bzero(buffer,sizeof buffer);

	switch(field)
	{
		case QNBR_QP:
			sprintf(buffer,SQL_UPDATE,"NBR_QP",val,quiz->quizId);
		break;
		case QNBR_QPS:
			sprintf(buffer,SQL_UPDATE,"NBR_QPS",val,quiz->quizId);
		break;
		case QNBR_QPF:
			sprintf(buffer,SQL_UPDATE,"NBR_QPF",val,quiz->quizId);
		break;
		default:
			print_logerr("Field out of value");
			return false;
		break;	
	}

	
	if(mysql_query(con,buffer))	
	{
		__PrintDError("{MOD}Error while executing update statement");
		return false;
	}
	else
	{
	
		switch(field)
		{
			case QNBR_QP:
				quiz->qp_number=val;
			break;
			case QNBR_QPS:
				quiz->qs_number=val;
			break;
			case QNBR_QPF:
				quiz->qf_number=val;
			break;
			default:
				print_logerr("Field out of value");
				return false;
			break;	
		}
	
	}

	return true;
}

Quiz* __GetQuiz(MYSQL* con,unsigned int quizCode)
{
	if(!con)
	{	
		print_logerr("[QUIZ:GETALL] Connection has not been initialized");
		return NULL;	
	}
	
	MYSQL_RES* results=NULL;
	MYSQL_ROW row;
	Quiz* quiz=NULL;
	char buffer[BUFFER_LENGTH];
	Bzero(buffer,sizeof buffer);

	sprintf(buffer,SQL_SELECT_CODE,quizCode);

	if(mysql_query(con,buffer))
	{
		__PrintDError("{GET_QUIZ} Error while executing insert statement");
		return NULL;
	}
	

	results=mysql_store_result(con);		
	
	if(!results)	
	{
		__PrintDError("{SELECT_QUIZ} Error while executing select statement");
		return NULL;
	}

	if(row=mysql_fetch_row(results))
	{	
		int id=atoi(row[0]);
		int qO=atoi(row[1]);
		int qC=atoi(row[2]);
		int qp=atoi(row[3]);
		int qps=atoi(row[4]);
		int qpf=atoi(row[5]);	
		
		quiz=__InitQuiz(id,qO,qC,qp,qps,qpf);
	}

	return quiz;	
}

Quiz* __GetAllUserQuiz(MYSQL* con,unsigned int  user_id)
{
		if(!con)
	{	
		print_logerr("[QUIZ:GETALL] Connection has not been initialized");
		return NULL;	
	}
	
	MYSQL_RES* results=NULL;
	MYSQL_ROW row;
	Quiz* quiz=NULL;
	char buffer[BUFFER_LENGTH];
	Bzero(buffer,sizeof buffer);

	sprintf(buffer,SQL_SELECT_OWNER,user_id);

	if(mysql_query(con,buffer))
	{
		__PrintDError("{GET_QUIZ} Error while executing insert statement");
		return NULL;
	}
	

	results=mysql_store_result(con);		
	
	if(!results)	
	{
		__PrintDError("{SELECT_QUIZ} Error while executing select statement");
		return NULL;
	}

	while((row=mysql_fetch_row(results)))
	{	
		int id=atoi(row[0]);
		int qO=atoi(row[1]);
		int qC=atoi(row[2]);
		int qp=atoi(row[3]);
		int qps=atoi(row[4]);
		int qpf=atoi(row[5]);	
		__AddQuiz(&quiz,id,qO,qC,qp,qps,qpf);
	}

	return quiz;
}



