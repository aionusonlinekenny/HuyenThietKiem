

#ifndef _K_MSSQL_DATABASE_H_
#define _K_MSSQL_DATABASE_H_

#ifdef _SERVER

BOOL	MSSQL_GetExtPoint(IN char* cAccount, IN OUT int *gExtPoint);
BOOL	MSSQL_SetExtPoint(IN char* cAccount, IN int nExtPoint);
BOOL	MSSQL_PayExtPoint(IN char* cAccount, IN int nExtPoint);

BOOL	MSSQL_GetExtPoint2(IN char* cAccount, IN OUT int *gExtPoint);
BOOL	MSSQL_SetExtPoint2(IN char* cAccount, IN int nExtPoint);
BOOL	MSSQL_PayExtPoint2(IN char* cAccount, IN int nExtPoint);

BOOL	MSSQL_SetBandPC(IN char* cAccount);
BOOL	MSSQL_UnBandPC(IN char* cAccount);

BOOL	MSSQL_SetSongJin(IN char* cAccount,BOOL nLock);
BOOL	MSSQL_GetSongJin(IN char* cAccount);
#endif

#endif
