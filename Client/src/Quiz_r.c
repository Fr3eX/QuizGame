#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "Utility.h"
#include "Quiz_r.h"


Quiz_r* __InitAnswer(unsigned int id,unsigned int quiz_qId,unsigned char isCorrect,char *answer)
{
	Quiz_r* new_r=malloc(sizeof(*new_r));
	
	if(!new_r)
	{
		print_log("[MEMORY] Not Enough Memory for Quiz_r");
		return NULL;
	}

	new_r->r_id=id;
	new_r->quiz_qId=quiz_qId;
	new_r->isCorrect=isCorrect;
	
	new_r->answer=malloc(ANSWER_BUFFER* sizeof(char));
	if(!new_r->answer)
	{
		print_log("[MEMORY] Not Enough Memory for Quiz_r");
		__FreeQuiz_r(&new_r);
		return NULL;
	}
	strcpy(new_r->answer,answer);
	new_r->next=NULL;
	
	return new_r;
}

void __AddAnswer(Quiz_r** answers,unsigned int id,unsigned int quiz_qId,unsigned char isCorrect,char *answer)
{
	if(!*answers)
	{
		*answers=__InitAnswer(id,quiz_qId,isCorrect,answer);
		return;
	}

	Quiz_r* new_r=malloc(sizeof(*new_r));
	if(!new_r)
	{
		print_log("[MEMORY] Not Enough Memory for Quiz_r");
		return; 
	}
	
	new_r->r_id=id;
	new_r->quiz_qId=quiz_qId;
	new_r->isCorrect=isCorrect;
	new_r->answer=malloc(ANSWER_BUFFER* sizeof(char));
	
	if(!new_r->answer)
	{
		print_log("[MEMORY] Not Enough Memory for Quiz_r");
		__FreeQuiz_r(&new_r);
		return;
	}
	strcpy(new_r->answer,answer);
	
	new_r->next=*answers;
	*answers=new_r;
}




void __FreeQuiz_r(Quiz_r** answer)
{
	if(!*answer)
		return;
	__FreeQuiz_r(&(*answer)->next);
	free((*answer)->answer);
	free(*answer);
	*answer=NULL;
}

void __PrintQuiz_r(Quiz_r *r)
{
	
	unsigned i=0;
	while(r)
	{
		printf("Resp[%d] : id=%d\nquiz_qid=%d\nisCorrect=%d\nanswer=%s\n",++i,r->r_id,r->quiz_qId,r->isCorrect,r->answer);
		r=r->next;
	}

}


unsigned int AnswrCount(Quiz_r* answers)
{
	if(!answers)
		return 0;

	else
		return 1+AnswrCount(answers->next);
}

