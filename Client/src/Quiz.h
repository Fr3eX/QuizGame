#ifndef QUIZ_H
#define QUIZ_H


typedef struct Quiz_q Quiz_q;
typedef struct Quiz_r Quiz_r;


typedef struct Quiz
{
	unsigned int quizOwner;
	unsigned int quizId;
	unsigned int quizCode;
	unsigned int qp_number;
	unsigned int qs_number;
	unsigned int qf_number;
	
	Quiz_q* questions;

	struct Quiz* next;
}Quiz;




Quiz* __InitQuiz(unsigned int,
		 unsigned int,
		 unsigned int,
		 unsigned int,
		 unsigned int,
		 unsigned int);



Quiz* __AddQuiz(Quiz** ,unsigned int,
		unsigned int,
		unsigned int,
		unsigned int,
		unsigned int,
		unsigned int);
void __PrintQuiz(Quiz*);
void __FreeQuiz(Quiz**);

#endif
