#ifndef DAOQUIZ_R_H
#define DAOQUIZ_R_H

#define RBUFFER_LENGTH 555

typedef enum
{
	ISCORRECT,
	ANSWER
}Field_qr;

typedef union
{
	unsigned int val_i;
	char* val_str;

}Field_qrVal;

bool __PersistQuiz_r(MYSQL*,Quiz_r*,unsigned int);
bool __DeleteQuiz_r(MYSQL*,unsigned int);
bool __UpdateQuiz_r(MYSQL*,Field_qr,Field_qrVal*,Quiz_r*);

	
void __GetQuiz_r(MYSQL*,Quiz_q*);


#endif

