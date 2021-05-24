#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <mysql/mysql.h>
#include "Utility.h"
#include "Database.h"
#include <time.h>
#include "Quiz.h"

#define CLIENT
#include "User.h"

static unsigned __GetClientId(char const*,MYSQL*);
static Client* __BuildClient(char*,char*,char*,char*);
static bool __IsExist(char const*,MYSQL *);


static char* SQL_INSERT="INSERT INTO Quiz (quizOwner,quizCode,startTime,deadTime) values(%d,%d,'%d-%d-%d','%d-%d-%d')";
static char* SQL_DELETE="DELETE from Quiz where id=%d"; 
static char* SQL_UPDATE="UPDATE Quiz SET %s='%s' where id=%d";
static char* SQL_SELECT="SELECT * from Quiz where id='%s'"; 
static char* SQL_SELECTID="SELECT ID from Quiz where quizCode='%s'"; 



/*
unsigned int __AddQuiz(unsigned int quizOwner,Quiz_q* questions,Quiz_r* response)
{
	
}*/
