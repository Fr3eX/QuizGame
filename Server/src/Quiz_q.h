#ifndef QUIZ_H
#define QUIZ_H



#define QUESTION_BUFFER 255
#define ANSWER_BUFFER 255


typedef struct
{
	unsigned int quizId;
	unsigned int q_id;
	unsigned int mark;
	char* question;
	Quiz_r* responses,
	
	Quiz_q* next;
}Quiz_q;


Quiz_q* __InitQuestion(

#endif

