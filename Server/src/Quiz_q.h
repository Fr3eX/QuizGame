#ifndef QUIZ_Q_H
#define QUIZ_Q_H

#define QUESTION_BUFFER 255

typedef struct Quiz_q
{
	unsigned int quizId;
	unsigned int q_id;
	unsigned int mark;
	char* question;
	Quiz_r* answers;
	
	struct Quiz_q* next;
}Quiz_q;



Quiz_q*  __InitQuestion(unsigned int,unsigned int,char*,Quiz_r*);

void __AddQuestion(Quiz_q**,unsigned int,unsigned int,char*,Quiz_r*);
void __PrintQuiz_q(Quiz_q*);
void __FreeQuiz_q(Quiz_q**);
#endif

