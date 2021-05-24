#include <criterion/criterion.h>

#include <stdio.h>
#include <stdbool.h>
#include <mysql/mysql.h>
#include "../../src/config_parser.h"
#include "../../src/Database.h"
#include "../../src/Quiz_r.h"
#include "../../src/Quiz_q.h"
#include "../../src/Quiz.h"
#include "../../src/DAOQuiz_r.h"
#include "../../src/DAOQuiz_q.h"

MYSQL *con=NULL;
Quiz_r* answers=NULL;


Test(quiz_r,insert)
{
	__AddAnswer(&answers,1,1,10,"TEST ANSWER 1! ");
	cr_expect(answers,"Error while inserting first answer element");	
	__AddAnswer(&answers,2,1,20,"TEST ANSWER 2! ");
	cr_expect(answers,"Error while inserting seconde answer element");	
	__AddAnswer(&answers,3,1,10,"TEST ANSWER 3! ");
	cr_expect(answers,"Error while inserting third answer element");	
}

Test(quiz_rdao,persist)
{
	__AddAnswer(&answers,1,1,10,"TEST ANSWER 1! ");
	__AddAnswer(&answers,2,1,20,"TEST ANSWER 2! ");
	__AddAnswer(&answers,3,1,10,"TEST ANSWER 3! ");
	
	__ParseCfgFile("config");		
	__InitDatabaseConnection();
	con=__GetMysqlCon();
	
	cr_expect((__PersistQuiz_r(con,answers)==true),"Error while persisting answers");
	cr_expect((__DeleteQuiz_r(con,4)==true),"Error while deleting answers");
	cr_expect((__DeleteQuiz_r(con,5)==true),"Error while deleting answers");
	cr_expect((__DeleteQuiz_r(con,6)==true),"Error while deleting answers");
	
	__FreeDatabaseConnection();
	__FreePData();
}

