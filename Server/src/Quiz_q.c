#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "Utility.h"
#include "Quiz_r.h"
#include "Quiz_q.h"


Quiz_q* __InitQuestion(unsigned int id,unsigned int quizId,unsigned int mark,char* question,Quiz_r* answers)
{
	
	Quiz_q* new_q=malloc(sizeof(*new_q));
	if(!new_q)
	{
		print_logerr("[MEMORY] Not enought Memory for new_q");
		return NULL;
	}

	
	new_q->question=malloc( QUESTION_BUFFER * sizeof(char) );
	if(!new_q->question)
	{
		print_logerr("[MEMORY] Not enought Memory for new_q");
		return NULL;
	}
	strcpy(new_q->question,question);
	
	new_q->q_id=id;
	new_q->quizId=quizId;
	new_q->mark=mark;
	new_q->answers=answers;
	
	new_q->next=NULL;
	
	return new_q;
}


Quiz_q* __AddQuestion(Quiz_q** questions,unsigned int id,unsigned int quizId,unsigned int mark,char* question,Quiz_r* answers)
{
	
	Quiz_q* point=*questions;

	if(!*questions)
	{
		*questions=__InitQuestion(id,quizId,mark,question,answers);
		return *questions;
	}

	Quiz_q* new_q=malloc(sizeof(*new_q));

	if(!new_q)
	{
		print_logerr("[MEMORY] Not enought Memory for new_q");
		return NULL;
	}

	new_q->question=malloc( QUESTION_BUFFER * sizeof(char) );
	if(!new_q->question)
	{
		print_logerr("[MEMORY] Not enought Memory for new_q");
		return NULL ;
	}
	strcpy(new_q->question,question);
	
	new_q->q_id=id;
	new_q->mark=mark;
	new_q->quizId=quizId;
	new_q->answers=answers;
	new_q->next=NULL;

	while(point->next)
		point=point->next;
	
	point->next=new_q;

	return new_q;
}


void __FreeQuiz_q(Quiz_q** questions)
{
	if(!*questions)
		return;

	__FreeQuiz_q(&(*questions)->next);
	__FreeQuiz_r(&(*questions)->answers);
	free((*questions)->question);
	free((*questions));	

	*questions=NULL;
}

void __PrintQuiz_q(Quiz_q* q)
{
	unsigned i=0;

	while(q)
	{
		printf("Question[%d] : \nid=%d\nMark = %d\nQuizId= %d\n%s\n",i,q->q_id,q->mark,q->quizId,q->question);
		__PrintQuiz_r(q->answers);
		i++;
		q=q->next;
	}
}













