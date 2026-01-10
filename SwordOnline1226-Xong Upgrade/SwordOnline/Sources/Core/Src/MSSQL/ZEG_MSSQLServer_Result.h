#ifndef MSSQLSERVER_RESULT_H
#define MSSQLSERVER_RESULT_H

#ifdef _SERVER

#pragma once
#include "msado15.tlh"

class ZEG_MSSQLServer_Result
{
public:
	_RecordsetPtr m_pResult;
	ZEG_MSSQLServer_Result()
	{
		m_pResult = NULL;
	}
	virtual ~ZEG_MSSQLServer_Result()
	{
		m_pResult = NULL;
	}

	void AttachResult(_RecordsetPtr& pResult)
	{
		m_pResult = pResult;
	}

	int num_rows() const;
	int num_fields() const;
	void data_seek (unsigned int offset, int nType) const;
	bool get_field_data(unsigned int nfieldindex, void* pData, unsigned long nsize);
	int unuse();

private:
	enum
	{
		begin,
		next,
		previous,
		end
	};
};

#endif	// _SERVER
#endif	// MSSQLSERVER_RESULT_H
