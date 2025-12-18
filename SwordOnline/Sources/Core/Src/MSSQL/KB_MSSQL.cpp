


#include  "KCore.h"
#ifdef _SERVER

#pragma once
#include  "KB_MSSQL.h"
#include  "ZEG_MSSQLServer_Connection.h"

extern	tag_DATABASEINFO tagSQLInfo;	

BOOL MSSQL_GetExtPoint(IN char* cAccount, IN OUT int *gExtPoint)
{	
	ZEG_MSSQLServer_Connection*	pConn = new ZEG_MSSQLServer_Connection;
	
	if( !pConn->OpenConnect(&tagSQLInfo) )
	{
		*gExtPoint = 0;
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}

	char strSQL[MAX_PATH];
	sprintf(strSQL, "SELECT nExtPoint FROM View_AccountMoney WHERE (cAccName = '%s')", cAccount);
	ZEG_MSSQLServer_Result* pResult = NULL;
	if( pConn->QuerySql(strSQL, &pResult) )
	{
		if (pResult->num_rows() > 0)
		{
			_variant_t iExt;
			pResult->get_field_data(0, &iExt, sizeof(_variant_t));
			
			*gExtPoint = iExt.lVal;
		}
		else
		{	
			if(pResult)
				pResult->unuse();
			*gExtPoint = 0;
			pConn->CloseConnect();
			if(pConn)
				delete pConn;
			pConn = NULL;
			return FALSE;
		}
	}
	else
	{
		if(pResult)
			pResult->unuse();
		*gExtPoint = 0;
		pConn->CloseConnect();
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}
	
	if(pResult)
		pResult->unuse();
	pConn->CloseConnect();
	if(pConn)
		delete pConn;
	pConn = NULL;
	return TRUE;
}


BOOL	MSSQL_SetExtPoint(IN char* cAccount, IN int nExtPoint)
{
	ZEG_MSSQLServer_Connection*	pConn = new ZEG_MSSQLServer_Connection;
	
	if( !pConn->OpenConnect(&tagSQLInfo) )
	{
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}

	char strSQL[MAX_PATH];
	sprintf(strSQL, "UPDATE View_AccountMoney SET nExtPoint = '%d' WHERE (cAccName = '%s')", nExtPoint, cAccount);
	if( !pConn->Do(strSQL) )
	{
		pConn->CloseConnect();
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}
	
	pConn->CloseConnect();
	if(pConn)
		delete pConn;
	pConn = NULL;
	return TRUE;
}


BOOL	MSSQL_PayExtPoint(IN char* cAccount, IN int nExtPoint)
{
	ZEG_MSSQLServer_Connection*	pConn = new ZEG_MSSQLServer_Connection;
	
	if( !pConn->OpenConnect(&tagSQLInfo) )
	{
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}

	char strSQL[MAX_PATH];
	sprintf(strSQL, "UPDATE View_AccountMoney SET nExtPoint = nExtPoint - '%d' WHERE (cAccName = '%s')", nExtPoint, cAccount);
	if( !pConn->Do(strSQL) )
	{
		pConn->CloseConnect();
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}
	
	pConn->CloseConnect();
	if(pConn)
		delete pConn;
	pConn = NULL;
	return TRUE;
}

BOOL MSSQL_GetExtPoint2(IN char* cAccount, IN OUT int *gExtPoint)
{	
	ZEG_MSSQLServer_Connection*	pConn = new ZEG_MSSQLServer_Connection;
	
	if( !pConn->OpenConnect(&tagSQLInfo) )
	{
		*gExtPoint = 0;
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}

	char strSQL[MAX_PATH];
	sprintf(strSQL, "SELECT nExtPoint2 FROM View_AccountMoney WHERE (cAccName = '%s')", cAccount);
	ZEG_MSSQLServer_Result* pResult = NULL;
	if( pConn->QuerySql(strSQL, &pResult) )
	{
		if (pResult->num_rows() > 0)
		{
			_variant_t iExt;
			pResult->get_field_data(0, &iExt, sizeof(_variant_t));
			
			*gExtPoint = iExt.lVal;
		}
		else
		{	
			if(pResult)
				pResult->unuse();
			*gExtPoint = 0;
			pConn->CloseConnect();
			if(pConn)
				delete pConn;
			pConn = NULL;
			return FALSE;
		}
	}
	else
	{	
		if(pResult)
			pResult->unuse();
		*gExtPoint = 0;
		pConn->CloseConnect();
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}
	if(pResult)
		pResult->unuse();
	pConn->CloseConnect();
	if(pConn)
		delete pConn;
	pConn = NULL;
	return TRUE;
}


BOOL	MSSQL_SetExtPoint2(IN char* cAccount, IN int nExtPoint)
{
	ZEG_MSSQLServer_Connection*	pConn = new ZEG_MSSQLServer_Connection;
	
	if( !pConn->OpenConnect(&tagSQLInfo) )
	{
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}

	char strSQL[MAX_PATH];
	sprintf(strSQL, "UPDATE View_AccountMoney SET nExtPoint2 = '%d' WHERE (cAccName = '%s')", nExtPoint, cAccount);
	if( !pConn->Do(strSQL) )
	{
		pConn->CloseConnect();
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}
	
	pConn->CloseConnect();
	if(pConn)
		delete pConn;
	pConn = NULL;
	return TRUE;
}


BOOL	MSSQL_PayExtPoint2(IN char* cAccount, IN int nExtPoint)
{
	ZEG_MSSQLServer_Connection*	pConn = new ZEG_MSSQLServer_Connection;
	
	if( !pConn->OpenConnect(&tagSQLInfo) )
	{
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}

	char strSQL[MAX_PATH];
	sprintf(strSQL, "UPDATE View_AccountMoney SET nExtPoint2 = nExtPoint2 - '%d' WHERE (cAccName = '%s')", nExtPoint, cAccount);
	if( !pConn->Do(strSQL) )
	{
		pConn->CloseConnect();
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}
	
	pConn->CloseConnect();
	if(pConn)
		delete pConn;
	pConn = NULL;
	return TRUE;
}


BOOL	MSSQL_SetBandPC(IN char* cAccount)
{
	ZEG_MSSQLServer_Connection*	pConn = new ZEG_MSSQLServer_Connection;
	
	if( !pConn->OpenConnect(&tagSQLInfo) )
	{
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}
	
	char strSQL[MAX_PATH];
	ZEG_MSSQLServer_Result* pResult = NULL;
	char szKey[64];
	sprintf(strSQL, "SELECT cClientKey FROM View_AccountLicense WHERE (cAccName = '%s')", cAccount);
	if( pConn->QuerySql(strSQL, &pResult) )
	{
		if(pResult->num_rows() > 0)
		{
			_variant_t iKey;
			pResult->get_field_data(0, &iKey, sizeof(_variant_t));
			
			sprintf(szKey, "%s", (const char *)(_bstr_t)iKey);
		}
		else
		{	
			if (pResult)
				pResult->unuse();
			pConn->CloseConnect();
			if(pConn)
				delete pConn;
			pConn = NULL;
			return FALSE;
		}
		if(pResult)
			pResult->unuse();
	}

	sprintf(strSQL, "UPDATE View_AccountLicense SET cBandKey = '%s' WHERE (cAccName = '%s')", szKey, cAccount);
	if( !pConn->Do(strSQL) )
	{
		pConn->CloseConnect();
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}
	
	pConn->CloseConnect();
	if(pConn)
		delete pConn;
	pConn = NULL;
	return TRUE;
}

BOOL	MSSQL_UnBandPC(IN char* cAccount)
{
	ZEG_MSSQLServer_Connection*	pConn = new ZEG_MSSQLServer_Connection;
	
	if( !pConn->OpenConnect(&tagSQLInfo) )
	{
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}
	
	char strSQL[MAX_PATH];
	ZEG_MSSQLServer_Result* pResult = NULL;
	char szKey[64];
	sprintf(strSQL, "SELECT cClientKey FROM View_AccountLicense WHERE (cAccName = '%s')", cAccount);
	if( pConn->QuerySql(strSQL, &pResult) )
	{
		if(pResult->num_rows() > 0)
		{
			_variant_t iKey;
			pResult->get_field_data(0, &iKey, sizeof(_variant_t));
			
			sprintf(szKey, "%s", (const char *)(_bstr_t)iKey);
		}
		else
		{	
			if (pResult)
				pResult->unuse();
			pConn->CloseConnect();
			if(pConn)
				delete pConn;
			pConn = NULL;
			return FALSE;
		}
		if(pResult)
			pResult->unuse();
	}

	sprintf(strSQL, "UPDATE View_AccountLicense SET cBandKey = '%s' WHERE (cAccName = '%s')", NULL, cAccount);
	if( !pConn->Do(strSQL) )
	{
		pConn->CloseConnect();
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}
	
	pConn->CloseConnect();
	if(pConn)
		delete pConn;
	pConn = NULL;
	return TRUE;
}

BOOL	MSSQL_SetSongJin(IN char* cAccount,BOOL nLock)
{
	ZEG_MSSQLServer_Connection*	pConn = new ZEG_MSSQLServer_Connection;
	
	if( !pConn->OpenConnect(&tagSQLInfo) )
	{
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}

	char strSQL[MAX_PATH];
	sprintf(strSQL, "UPDATE Account_info SET iSongJin = '%d' WHERE (cAccName = '%s')", nLock, cAccount);
	if( !pConn->Do(strSQL) )
	{
		pConn->CloseConnect();
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}
	
	pConn->CloseConnect();
	if(pConn)
		delete pConn;
	pConn = NULL;
	return TRUE;
}

BOOL	MSSQL_GetSongJin(IN char* cAccount)
{
	ZEG_MSSQLServer_Connection*	pConn = new ZEG_MSSQLServer_Connection;
	
	if( !pConn->OpenConnect(&tagSQLInfo) )
	{
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}

	char strSQL[MAX_PATH];
	ZEG_MSSQLServer_Result* pResult = NULL;

	char szKey[64];
	sprintf(strSQL, "SELECT cClientKey FROM View_AccountLicense WHERE (cAccName = '%s')", cAccount);
	if( pConn->QuerySql(strSQL, &pResult) )
	{
		if(pResult->num_rows() > 0)
		{
			_variant_t iKey;
			pResult->get_field_data(0, &iKey, sizeof(_variant_t));
			
			sprintf(szKey, "%s", (const char *)(_bstr_t)iKey);
		}
		else
		{	
			if (pResult)
				pResult->unuse();
			pConn->CloseConnect();
			if(pConn)
				delete pConn;
			pConn = NULL;
			return FALSE;
		}
		if(pResult)
			pResult->unuse();
	}

	
	BYTE iSongJin = 0;
	sprintf(strSQL, "SELECT cClientKey,iSongJin FROM View_AccountLicense WHERE (cClientKey = '%s') AND (iSongJin > '%d')", szKey,iSongJin);
	if( pConn->QuerySql(strSQL, &pResult) )
	{
		if(pResult->num_rows() > 0)
		{
			if (pResult)
				pResult->unuse();
			pConn->CloseConnect();
			if(pConn)
				delete pConn;
			pConn = NULL;
			return TRUE;
		}
		else
		{	
			if (pResult)
				pResult->unuse();
			pConn->CloseConnect();
			if(pConn)
				delete pConn;
			pConn = NULL;
			return FALSE;
		}
		if(pResult)
			pResult->unuse();
	}

	if( !pConn->Do(strSQL) )
	{
		pConn->CloseConnect();
		if(pConn)
			delete pConn;
		pConn = NULL;
		return FALSE;
	}
	
	pConn->CloseConnect();
	if(pConn)
		delete pConn;
	pConn = NULL;
	return FALSE;
}
#endif