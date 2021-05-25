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
#include "DAOQuiz.h"

int main()
{
	__ParseCfgFile("config");
	__InitDatabaseConnection();		
	
	MYSQL *con=__GetMysqlCon();
	
	Quiz* q=__GetQuiz(con,500);	
	__PrintQuiz(q);

	__UpdateQuiz(con,QNBR_QP,20,q);	
	__UpdateQuiz(con,QNBR_QPS,20,q);	
	__UpdateQuiz(con,QNBR_QPF,20,q);	
	
	
	__PrintQuiz(q);
	
	
	__FreeDatabaseConnection();
	__FreePData();
	return 0;

}
