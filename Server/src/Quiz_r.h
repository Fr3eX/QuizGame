#ifndef QUIZ_H
#define QUIZ_H

#define ANSWER_BUFFER 255



typedef struct Quiz_r
{
	unsigned int r_id;
	unsigned int quiz_qId;
	unsigned char isCorrect;
	char* answer;

	struct Quiz_r *next;
}Quiz_r;



#endif

