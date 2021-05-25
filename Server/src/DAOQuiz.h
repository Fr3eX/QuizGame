#ifndef DAOQUIZ_H
#define DAOQUIZ_H


typedef enum
{
	QNBR_QP,
	QNBR_QPS,
	QNBR_QPF
}Field_quiz;

bool __PersistQuiz(MYSQL*,Quiz*);
bool __RemoveQuiz(MYSQL*,unsigned int);
bool __UpdateQuiz(MYSQL*,Field_quiz,unsigned int,Quiz*);


Quiz* __GetQuiz(MYSQL* con,unsigned int);
Quiz* __GetAllUserQuiz(MYSQL*,unsigned int);


#endif

