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
#include "Q_U_STATUS.h"

int main()
{
	__ParseCfgFile("config");
	__InitDatabaseConnection();		
	
	MYSQL *con=__GetMysqlCon();
	
	Q_U_status* alja=__GetQUStatus(con,3);
	__PrintQUStatus(alja);	
	Q_U_status* omar=__GetQUStatus(con,2);
	__PrintQUStatus(omar);	
	Q_U_status* inc=__GetQUStatus(con,1);
	__PrintQUStatus(inc);	
		
	
	__FreeDatabaseConnection();
	__FreePData();
	return 0;

}
