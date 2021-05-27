#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>


#include "Utility.h"
#include "Database.h"
#include "Quiz.h"
#include "DAOQuiz_q.h"
#include "DAOQuiz_r.h"
#include "Quiz_q.h"
#include "Quiz_r.h"



static char* SQL_INSERT="INSERT INTO Quiz_q (quizId,mark,question) values(%d,%d,'%s')"; 
static char* SQL_DELETE="DELETE from Quiz_q where id=%d";
static char* SQL_UPDATE="UPDATE Quiz_q SET %s='%s' where id=%d";
static char* SQL_UPDATE_I="UPDATE Quiz_q SET %s=%d where id=%d";
static char* SQL_SELECTQUIZ_ID="SELECT id,quizId,mark,question from Quiz_q where quizId=%d";





bool __PersistQuiz_q(MYSQL* con,Quiz_q *r,unsigned int id)
{
	if(!con)
	{	
		print_logerr("[DAOQUIZ_R:PERSISTE] Connection has not been initialized");
		return false;	
	}
	char buffer[QBUFFER_LENGTH];
	Bzero(buffer,sizeof buffer);

	while(r)
	{
	
		Bzero(buffer,sizeof buffer);
		sprintf(buffer,SQL_INSERT,id,r->mark,r->question);
		
		if(mysql_query(con,buffer))
		{
			__PrintDError("{ADD_Q} Erro while executing insert statement");
			return false;
		}
		
		unsigned id=mysql_insert_id(con);	
		__PersistQuiz_r(con,r->answers,id);	
		r=r->next;
	}
	return true;
}


bool __DeleteQuiz_q(MYSQL* con,unsigned int id)
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

bool __UpdateQuiz_q(MYSQL* con,Quiz_q* question,Field_qq field,Field_qqVal *val)
{
	if(!con || !question)
	{
		print_logerr("[DAOQUIZ_Q:DELETE] Connection has not benn initialized or question is NULL");	
		return false;	
	}	
	
	char buffer[QBUFFER_LENGTH];
	Bzero(buffer,sizeof buffer);
	
	
	switch(field)
	{
		case MARK:
			sprintf(buffer,SQL_UPDATE_I,"mark",val->val_i,question->q_id);
			break;
		case QUESTION:
			sprintf(buffer,SQL_UPDATE,"question",val->val_str,question->q_id);
			break;
		default:
			print_logerr("Field_qq out of value");
			return false;
	}

	if(mysql_query(con,buffer))
	{
		__PrintDError("{UPDATE_Q} Erro while executing update statement");
		return false;
	}
	else
	{
		switch(field)
		{
			case MARK:
				question->mark=val->val_i;
				break;
			case QUESTION:
				Bzero(question->question,sizeof question->question);
				strcpy(question->question,val->val_str);
				break;
			default:
				print_logerr("Field_qq out of value");
				return false;
		}
	}


	return true;	
}


Quiz_q * __GetQuiz_q(MYSQL* con,Quiz* quiz)
{
	if(!con || !quiz)
	{
		print_logerr("[DAOQUIZ_R:GET] Connection has not benn initialized or quiz is NULL");	
		return NULL;	
	}	

	char buffer[QBUFFER_LENGTH];
	Quiz_q* questions=NULL;	
	Quiz_q* tmp=NULL;
	MYSQL_RES* results=NULL;		
	MYSQL_ROW row;
	
			
	Bzero(buffer,sizeof buffer);
	
	sprintf(buffer,SQL_SELECTQUIZ_ID,quiz->quizId);

	if(mysql_query(con,buffer))
	{
		__PrintDError("{SELECT_Q} Erro while executing select statement");
		return NULL;
	}
	
	results=mysql_store_result(con);		
	if(!results)
	{
		__PrintDError("{SELECT_R} Error while storing results");
		return NULL;
	}

	while((row=mysql_fetch_row(results)))
	{
		tmp=__AddQuestion(&questions,atoi(row[0]),atoi(row[1]),atoi(row[2]),row[3],NULL);						
		__GetQuiz_r(con,tmp);
	}
	
	mysql_free_result(results);	
	
	return questions; 
}
