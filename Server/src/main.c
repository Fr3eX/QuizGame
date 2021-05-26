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
#include "Server.h"

int main()
{
	__ParseCfgFile("config");
	
	__InitServer();	
	
	__FreePData();
	return 0;

}
