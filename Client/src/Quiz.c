#include <stdio.h>
#include <stdlib.h>

#include "Utility.h"
#include "Quiz.h"
#include "Quiz_q.h"


Quiz* __InitQuiz(unsigned int id,
		 unsigned int quizOwner,
		 unsigned int quizCode,
		 unsigned int NBR_QP,
		 unsigned int NBR_QPS,
		 unsigned int NBR_QPF)
{

	Quiz* quiz=malloc(sizeof(Quiz));
	
	if(!quiz)
	{
		print_logerr("[MEMORT] Not enough memory for quiz Structure ");
		return NULL;
	}

	quiz->quizId=id;
	quiz->quizOwner=quizOwner;
	quiz->quizCode=quizCode;
	quiz->qp_number=NBR_QP;
	quiz->qs_number=NBR_QPS;
	quiz->qf_number=NBR_QPF;


	quiz->questions=NULL;
	quiz->next=NULL;


	return quiz;
}




Quiz* __AddQuiz(Quiz** quiz,unsigned int id,
		unsigned int quizOwner,
		unsigned int quizCode,
		unsigned int NBR_QP,
		unsigned int NBR_QPS,
		unsigned int NBR_QPF)
{

	if(!*quiz)
	{
		*quiz=__InitQuiz(id,
				 quizOwner,
				 quizCode,
				 NBR_QP,
				 NBR_QPS,
				 NBR_QPF);
		return *quiz;	
	}
	
	Quiz* pointer=*quiz;
	Quiz* new_q=__InitQuiz(id,quizOwner,quizCode,NBR_QP,NBR_QPS,NBR_QPF);

	if(!new_q)
		return NULL;

	while(pointer->next)
		pointer=pointer->next;
		
	pointer->next=new_q;	


	return new_q;
}
	

void __PrintQuiz(Quiz* q)
{
	while(q)
	{
		printf("Quiz_id=%d\n"
		       "Quiz_owner=%d\n"
		       "Quiz_code=%d\n"
		       "Quiz_qp=%d\n"
		       "Quiz_qs=%d\n"
		       "Quiz_qd=%d\n",q->quizId,
		       q->quizOwner,
		       q->quizCode,
		       q->qp_number,
		       q->qs_number,
		       q->qf_number);
		__PrintQuiz_q(q->questions);	
		
		
		q=q->next;
	}
}


void __FreeQuiz(Quiz** quiz)
{
	if(!*quiz)
		return;

	__FreeQuiz(&(*quiz)->next);
	__FreeQuiz_q(&(*quiz)->questions);
	free(*quiz);

	*quiz=NULL;
}
