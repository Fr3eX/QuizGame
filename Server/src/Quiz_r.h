#ifndef QUIZ_R_H
#define QUIZ_R_H

#define ANSWER_BUFFER 255



typedef struct Quiz_r
{
	unsigned int r_id;
	unsigned int quiz_qId;
	unsigned char isCorrect;
	char* answer;

	struct Quiz_r *next;
}Quiz_r;


Quiz_r* __InitAnswer(unsigned int,unsigned char,char*);


void __AddAnswer(Quiz_r**,unsigned int,unsigned char,char *);
void __FreeQuiz_r(Quiz_r**);

void __PrintQuiz_r(Quiz_r*);

#endif

