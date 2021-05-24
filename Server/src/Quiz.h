#ifndef QUIZ_H
#define QUIZ_H



#define QUESTION_BUFFER 255
#define ANSWER_BUFFER 255
#include <time.h>


typedef struct Quiz_q Quiz_q;
typedef struct Quiz_r Quiz_r;

typedef struct
{
	unsigned int quizOwner;
	unsigned int quizId;
	unsigned int quizCode;
	unsigned int p_number;
	unsigned int qs_number;
	unsigned int qf_number;
	
	struct tm startTime;
	struct tm deadTime;
}Quiz;


typedef struct
{
	unsigned int userId;
	unsigned int quizId;
	unsigned int mark;
	unsigned int n_correctAnswers;
	unsigned int n_wrongAnswers;
}Q_U_status;



unsigned int __AddQuiz(unsigned int,unsigned int,Quiz_q*);
bool __RemoveQuiz(unsigned int);


Quiz __GetQuiz(unsigned int quizCode);
void __FreeQuiz(Quiz** quiz);



#endif

