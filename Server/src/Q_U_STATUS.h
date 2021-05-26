#ifndef Q_U_STATUS_H
#define Q_U_STATUS_H




typedef struct Q_U_status
{
	unsigned int userId;
	unsigned int quizId;
	unsigned int mark;

	struct Q_U_status* next;
}Q_U_status;


bool __PersistQ_U_STATUS(MYSQL*,Q_U_status*);
Q_U_status* __GetQUStatus(MYSQL*,unsigned int);

	
	
	
Q_U_status* __InitQUStatus(unsigned int,
			   unsigned int,
			   unsigned int );

Q_U_status * __AddQStatus(Q_U_status **status,unsigned int,
					      unsigned int,
					      unsigned int);

void __FreeQUS(Q_U_status**);
void __PrintQUStatus(Q_U_status*);


#endif
