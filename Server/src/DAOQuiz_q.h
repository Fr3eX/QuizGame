#ifndef DAOQUIZ_Q_H
#define DAOQUIZ_Q_H


#define QBUFFER_LENGTH 555


typedef enum
{
	MARK,
	QUESTION
}Field_qq;


typedef union
{
	char* val_str;
	unsigned int val_i;
}Field_qqVal;


bool __PersistQuiz_q(MYSQL*,Quiz_q*,unsigned int);
bool __DeleteQuiz_q(MYSQL*,unsigned int);
bool __UpdateQuiz_q(MYSQL*,Quiz_q*,Field_qq field,Field_qqVal *);

Quiz_q * __GetQuiz_q(MYSQL*,Quiz*);


#endif

