#ifndef ZEG_ZEG_MSSQLServer_Connection_H
#define ZEG_ZEG_MSSQLServer_Connection_H

#ifdef _SERVER

#pragma once
#include "../GameDataDef.h"
#include "ZEG_MSSQLServer_Result.h"
#include "msado15.tlh"

class ZEG_MSSQLServer_Connection
{
public:
	ZEG_MSSQLServer_Connection();
	~ZEG_MSSQLServer_Connection();
	bool OpenConnect(_LPDATABASEINFO);
	void CloseConnect();
	bool QuerySql(const char* lpszSql, ZEG_MSSQLServer_Result** ppResult);
	bool Do(const char* lpszSql);
	
	
private:
	_ConnectionPtr m_pCon;
	_CommandPtr m_pCmd;
	ZEG_MSSQLServer_Result m_Result;
	ZEG_MSSQLServer_Result m_Result2;
	bool GetFreeResult(ZEG_MSSQLServer_Result** ppResult);

	bool QueryResult(const char* lpszSql, ZEG_MSSQLServer_Result* pRes);
	bool DoResult(const char* lpszSql);

	bool QueryCmd(const char* lpszSql, ZEG_MSSQLServer_Result* pRes);
	bool DoCmd(const char* lpszSql);
};

#endif	// _SERVER
#endif	// ZEG_ZEG_MSSQLServer_Connection_H