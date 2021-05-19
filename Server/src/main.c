#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <mysql/mysql.h>

#include "config_parser.h"
#include "Utility.h"
#include "User.h"
#include "Database.h"


int main()
{
	__ParseCfgFile("config");	
	__InitDatabaseConnection();
	
	
	Client* cl=__GetClient("omar");
	printf("%s  %d %d \n",cl->lastIp,cl->numberOfQuiz,cl->numberOfQuizzesPassed);
	return 0;
}
