#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "Utility.h"
#include "Quiz_r.h"




Quiz_r* InitResponse(unsigned int quiz_qId,unsigned char isCorrect,char *answer)
{
	Quiz_r* new_r=malloc(sizeof(*new_r));
	
	if(!new_r)
	{
		print_log("[MEMORY] Not Enough Memory for Quiz_r");
		return NULL;
	}

	
	new_r->quiz_qId=quiz_qId;
	new_r->isCorrect=isCorrect;
	
	new_r->answer=malloc(ANSWER_BUFFER* sizeof(char));
	if(!new_r->answer)
	{
		print_log("[MEMORY] Not Enough Memory for Quiz_r");
		return NULL;
	}
	
	new_r->next=NULL;
	
	return new_r;
}

