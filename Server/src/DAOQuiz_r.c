#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "Utility.h"
#include "Database.h"
#include "Quiz_r.h"
#include "Quiz_q.h"
#include "DAOQuiz_r.h"


static char* SQL_INSERT="INSERT INTO Quiz_r (quiz_qId,isCorrect,answer) values(%d,%d,'%s')"; 
static char* SQL_DELETE="DELETE from Quiz_r where id=%d";
static char* SQL_UPDATE="UPDATE Quiz_r SET %s='%s' where id=%d";
static char* SQL_UPDATE_I="UPDATE Quiz_r SET %s=%d where id=%d";
static char* SQL_SELECTQUIZ_ID="SELECT id,quiz_qId,isCorrect,answer from Quiz_r where quiz_qId=%d";

bool __PersistQuiz_r(MYSQL* con,Quiz_r *r)
{
	if(!con)
	{	
		print_logerr("[DAOQUIZ_R:PERSISTE] Connection has not been initialized");
		return false;	
	}
	char buffer[RBUFFER_LENGTH];
	Bzero(buffer,sizeof buffer);

	while(r)
	{
	
		Bzero(buffer,sizeof buffer);
		sprintf(buffer,SQL_INSERT,r->quiz_qId,r->isCorrect,r->answer);
		
		if(mysql_query(con,buffer))
		{
			__PrintDError("{ADD_R} Erro while executing insert statement");
			return false;
		}
		r=r->next;
	}
	return true;
}

bool __DeleteQuiz_r(MYSQL* con,unsigned int id)
{
	if(!con)
	{
		print_logerr("[DAOQUIZ_R:DELETE] Connection has not benn initialized");	
		return false;	
	}	
	
	char buffer[RBUFFER_LENGTH];
	Bzero(buffer,sizeof buffer);
	
	sprintf(buffer,SQL_DELETE,id);


	if(mysql_query(con,buffer))
	{
		__PrintDError("{ADD_R} Erro while executing insert statement");
		return false;
	}

	return true;
}

bool __UpdateQuiz_r(MYSQL* con,Field_qr field,Field_qrVal *val,Quiz_r* answer)
{
	if(!con | !answer)
	{
		print_logerr("[DAOQUIZ_R:DELETE] Connection has not benn initialized or answer is NULL");	
		return false;	
	}	
	
	char buffer[RBUFFER_LENGTH];
	Bzero(buffer,sizeof buffer);
	
	
	switch(field)
	{
		case ISCORRECT:
			sprintf(buffer,SQL_UPDATE_I,"isCorrect",val->val_i,answer->r_id);
			break;
		case ANSWER:
			sprintf(buffer,SQL_UPDATE,"answer",val->val_str,answer->answer);
			break;
		default:
			print_logerr("Field_qr out of value");
			return false;
	}

	if(mysql_query(con,buffer))
	{
		__PrintDError("{UPDATE_R} Error while executing update statement");
		return false;
	}
	else
	{
		switch(field)
		{
			case ISCORRECT:
				answer->isCorrect=val->val_i;	
				break;
			case ANSWER:
				Bzero(answer->answer,sizeof(answer->answer));
				strcpy(answer->answer,val->val_str);
				break;
			default:
				print_logerr("Field_qr out of value");
				return false;
		}
	}
	
	
	return true;	
}



void __GetQuiz_r(MYSQL* con,Quiz_q* question)
{
	if(!con || !question)
 	{
		print_logerr("[DAOQUIZ_R:GET] Connection has not benn initialized or question is NULL");	
		return;	
	}	

	char buffer[RBUFFER_LENGTH];
	Quiz_r* answers=NULL;	
	MYSQL_RES* results=NULL;		
	MYSQL_ROW row;
	
	question->answers=answers;
	
	Bzero(buffer,sizeof buffer);
	sprintf(buffer,SQL_SELECTQUIZ_ID,question->q_id);
	
	if(mysql_query(con,buffer))
	{
		__PrintDError("{SELECT_R} Erro while executing select statement");
		return;
	}
	
	results=mysql_store_result(con);		
	if(!results)
	{
		__PrintDError("{SELECT_R} Error while storing results");
		return;
	}

	while((row=mysql_fetch_row(results)))
	{
		__AddAnswer(&answers,atoi(row[0]),atoi(row[1]),atoi(row[2]),row[3]);	
	}
	
	question->answers=answers;

	mysql_free_result(results);		
}
