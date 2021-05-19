#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <mysql/mysql.h>
#include <stddef.h>

#include "Utility.h"
#include "Database.h"

#define CLIENT
#include "User.h"

static unsigned __GetClientId(char const*,MYSQL*);
static Client* __BuildClient(char*,char*,char*,unsigned int,unsigned int,unsigned int,unsigned int);
static bool __IsExist(char const*,MYSQL *);


static char* SQL_INSERT="INSERT INTO User (username,password,lastIp,NBR_Q,NBR_QP,NBR_QPS,NBR_QPF) values('%s','%s','%s','%d',%d,%d,%d)";
static char* SQL_DELETE="DELETE from User where id=%d"; 
static char* SQL_UPDATE="UPDATE User SET %s='%s' where id=%d";
static char* SQL_UPDATE_I="UPDATE User SET %s=%d where id=%d";
static char* SQL_SELECT="SELECT * from User where username='%s'"; 
static char* SQL_SELECTID="SELECT id from User where username='%s'"; 


Client* __AddClient(char* userName,char* password,char* lastIp,unsigned numberOfQuiz,unsigned int numberOfQuizzesPassed,unsigned int numberOfQuizzesPassed_S,unsigned int numberOfQuizzesPassed_F)
{
	MYSQL *connection = __GetMysqlCon();
	Client* client=NULL;
	char buffer[BUFFER_LENGTH];
	Bzero(buffer,sizeof buffer);

	if(!connection)
	{	
		print_logerr("{ADD}Connection has not been initialized");	
		return NULL;
	}
	
	if(__IsExist(userName,connection))
	{
		print_logerr("Client Exist");
		return NULL;
	}
	
	client=__BuildClient(userName,password,lastIp,numberOfQuiz,numberOfQuizzesPassed,numberOfQuizzesPassed_S,numberOfQuizzesPassed_F);
	if(!client)
		return NULL;
	
	/*Zeroing buffer*/
	Bzero(buffer,sizeof buffer);
	/*Writing the query on the buffer*/
	sprintf(buffer,SQL_INSERT,client->userName,client->password,client->lastIp,client->numberOfQuiz,client->numberOfQuizzesPassed,client->numberOfQuizzesPassed_S,client->numberOfQuizzesPassed_F);		
				
	if(mysql_query(connection,buffer))	
	{
		__PrintDError("{ADD}Error while executing insert statement");
		__FreeClient(&client);
		return NULL;
	}

	
	client->id=mysql_insert_id(connection); // Get last inserted id row work only if we used auto-increment opt
		
	return client;
}



Client* __GetClient(char const* userName)
{
	Client* client=NULL;
	MYSQL *connection = __GetMysqlCon();
	char buffer[BUFFER_LENGTH];
	
	Bzero(buffer,sizeof buffer);
	
	sprintf(buffer,SQL_SELECT,userName);		
	
	if(!connection)
	{	
		print_logerr("{GET}Connection has not been initialized");	
		return NULL;
	}
	
	if(mysql_query(connection,buffer))	
	{
		__PrintDError("{GET}Error while executing select statement");
		return NULL;
	}
		
	MYSQL_RES *result=mysql_store_result(connection);
	if(!result)
	{
		__PrintDError("{GET}Error while storing results ");
		return NULL;
	}

	MYSQL_ROW row;
	int num_field=mysql_num_fields(result);
	row=mysql_fetch_row(result);
		
	
	if(row)
	{	
		client=__BuildClient(row[1],row[2],row[3],atoi(!row[4]?"0":row[4]),atoi(!row[5]?"0":row[5]),atoi(!row[6]?"0":row[6]),atoi(!row[7]?"0":row[7]));
		client->id=atoi(row[0]);
	}
	
	mysql_free_result(result);	

	return client;
}

bool __ModClient(unsigned id,Field field,FieldVal* value)
{
	MYSQL *connection = __GetMysqlCon();
	char buffer[BUFFER_LENGTH];
	
	Bzero(buffer,sizeof buffer);
	
	switch(field)
	{
		case LAST_IP:
			sprintf(buffer,SQL_UPDATE,"lastIp",value->value_s,id);
			break;
		case NBR_Q:
			sprintf(buffer,SQL_UPDATE_I,"NBR_Q",value->value_i,id);
			break;
		case NBR_QP:
			sprintf(buffer,SQL_UPDATE_I,"NBR_QP",value->value_i,id);
			break;
		case NBR_QPS:
			sprintf(buffer,SQL_UPDATE_I,"NBR_QPS",value->value_i,id);
			break;
		case NBR_QPF:
			sprintf(buffer,SQL_UPDATE_I,"NBR_QPF",value->value_i,id);
			break;
		default:
			print_logerr("Field out of value");
			return false;
	}

	if(!connection)
	{	
		print_logerr("{MOD}Connection has not been initialized");	
		return false;
	}
	
	if(mysql_query(connection,buffer))	
	{
		__PrintDError("{MOD}Error while executing update statement");
		return false;
	}
	else
	{
		if(mysql_affected_rows(connection)!=1)
			return false;
				
	}
	return true;		
}


bool __RemoveClient(unsigned id)
{
	MYSQL *connection = __GetMysqlCon();
	char buffer[BUFFER_LENGTH];
	
	Bzero(buffer,sizeof buffer);
	
	sprintf(buffer,SQL_DELETE,id);

	if(!connection)
	{	
		print_logerr("{REM}Connection has not been initialized");	
		return false;
	}
	
	if(mysql_query(connection,buffer))	
	{
		__PrintDError("{REM}Error while executing delete statement");
		return false;
	}
	else
	{
		if(mysql_affected_rows(connection)!=1)
			return false;
				
	}
	return true;
}



void __FreeClient(Client** cl)
{
	if(!(*cl))
		return;
	free((*cl)->userName);
	free((*cl)->password);
	free((*cl)->lastIp);
	free(*cl);

	*cl=NULL;
}


static Client* __BuildClient(char* userName,char* password,char* lastIp,unsigned numberOfQuiz,unsigned int numberOfQuizzesPassed,unsigned int numberOfQuizzesPassed_S,unsigned int numberOfQuizzesPassed_F)
{
	Client* new=malloc(sizeof(Client));
	
	if(!new)	
	{
		print_logerr("There is no enough memory for a new client");
		return NULL;
	}
	
	
	new->userName=malloc(NICKNAME*sizeof(char));
	new->password=malloc(PASSWORD*sizeof(char));
	new->lastIp=malloc(LAST_IP*sizeof(char));
	

	strcpy(new->userName,userName);	
	strcpy(new->password,password);	
	strcpy(new->lastIp,(lastIp) ? lastIp : "NULL");	
	new->numberOfQuiz=numberOfQuiz;
	new->numberOfQuizzesPassed=numberOfQuizzesPassed;
	new->numberOfQuizzesPassed_S=numberOfQuizzesPassed_S;
	new->numberOfQuizzesPassed_F=numberOfQuizzesPassed_F;

	return new;
}



static bool __IsExist(char const* userName,MYSQL *connection)
{
	char buffer[BUFFER_LENGTH];
	Bzero(buffer,sizeof buffer);

	sprintf(buffer,SQL_SELECTID,userName);		
	
	if(mysql_query(connection,buffer))	
	{
		print_logerr("{ISE}Connection has not been initialized");	
		return true;
	}

	MYSQL_RES *result=mysql_store_result(connection);
	if(!result)
	{
		__PrintDError("{ISE}rror while storing results ");
		return true;
	}
	MYSQL_ROW row;
		
	if((row=mysql_fetch_row(result)))
		return true;

	mysql_free_result(result);	
	
	return false;
}


static unsigned __GetClientId(char const* userName,MYSQL* connection)
{
	unsigned id=-1;	
	char buffer[BUFFER_LENGTH];
	Bzero(buffer,sizeof buffer);
	sprintf(buffer,SQL_SELECTID,userName);		

	if(mysql_query(connection,buffer))
	{
		__PrintDError("Error while executing select statement");
		return -1;
	}	
	
	MYSQL_RES *result=mysql_store_result(connection);
	if(!result)
	{
		__PrintDError("Error while storing results ");
		return -1;
	}
	MYSQL_ROW row;
	
	int num_field=mysql_num_fields(result);

	if((row=mysql_fetch_row(result)))
	{
		id=(unsigned)atoi(row[0]);			
	}

	mysql_free_result(result);	

	return id;	
}
