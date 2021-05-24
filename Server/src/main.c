#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <mysql/mysql.h>


#include "config_parser.h"
#include "Utility.h"
#include "User.h"
#include "Database.h"
#include "Quiz_r.h"
#include "Quiz.h"
#include "Quiz_q.h"
#include "DAOQuiz_r.h"
#include "DAOQuiz_q.h"

int main()
{
	__ParseCfgFile("config");
	__InitDatabaseConnection();		
	
	MYSQL *con=__GetMysqlCon();
	Quiz quiz;
	quiz.quizId=1;	

	Quiz_q* questions=__GetQuiz_q(con,&quiz);
	
	if(questions == NULL)
		printf("NULL\n");	
	__PrintQuiz_q(questions);

	__FreeDatabaseConnection();
	__FreePData();
	return 0;

}
