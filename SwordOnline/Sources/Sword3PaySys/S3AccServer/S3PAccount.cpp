

#pragma warning(disable: 4786)
#include "S3PAccount.h"
#include "S3PDBConVBC.h"
#include "S3PResultVBC.h"
#include "COMUtil.h"
#include "GlobalFun.h"

//

S3PAccount::S3PAccount()
{
}


//

S3PAccount::~S3PAccount()
{
}


//

DWORD GetGMID();


//

int S3PAccount::Login(S3PDBConVBC* pConn, const char* strAccName, const char* strPassword, const char* strLicense, DWORD ClientID, DWORD& nLeftTime)
{
	int iRet = ACTION_FAILED;
	if (NULL == pConn)
	{
		return iRet;
	}
	
	iRet = VerifyClientLicense(pConn, strLicense);
	if(iRet != ACTION_SUCCESS)
	{
		return iRet;
	}

	if (CheckInputString(strAccName) == 0)
	{
		return iRet;
	}
	
	if (strstr("-", strAccName) != NULL || 
		strstr("'", strAccName) != NULL || 
		strstr(")", strAccName) != NULL || 
		strstr("(", strAccName) != NULL)	
		return iRet;										 // fix drop database

	if (strstr("-", strPassword) != NULL || 
		strstr("'", strPassword) != NULL || 
		strstr(")", strPassword) != NULL || 
		strstr("(", strPassword) != NULL)
		return iRet;										 // fix drop database

	if (CheckInputString(strPassword) == 0)
	{
		return iRet;
	}
	
	char strSQL[MAX_PATH];
	sprintf(strSQL, "select iClientID from Account_info where (cAccName = '%s') and (cPassword COLLATE Chinese_PRC_CS_AS = '%s')", strAccName, strPassword);
	S3PResultVBC* pResult = NULL;
	if (pConn->QuerySql(strSQL, &pResult))
	{
		if (pResult->num_rows() <= 0)
			iRet = E_ACCOUNT_OR_PASSWORD;
		else
		{
			_variant_t clientID = 0L;
			pResult->get_field_data(0, &clientID, sizeof(_variant_t));
			if (clientID.vt == VT_NULL || clientID.lVal == 0)
			{
				long iLeft = 0;
				long iExp = 0;
				long iExp2 = 0;
				if (GetLeftSecondsOfDeposit(pConn, strAccName, iLeft, iExp, iExp2) == ACTION_SUCCESS && iLeft > 1800)
				{
					sprintf(strSQL, "update Account_info set iClientID = %d, dLoginDate = null, cClientKey = '%s' where (cAccName = '%s')", ClientID, strLicense, strAccName);
					if (pConn->Do(strSQL))
					{
						iRet = ACTION_SUCCESS;
						nLeftTime = iLeft;
					}
				}
				else
				{
					iRet = E_ACCOUNT_NODEPOSIT;
				}
			}
			else
			{
				iRet = (GetGMID() == clientID.lVal) ? E_ACCOUNT_FREEZE : E_ACCOUNT_EXIST;
			}
		}
	}
	
	if (pResult)
		pResult->unuse();

	return iRet;
}


//

int S3PAccount::QueryExtPoint(S3PDBConVBC* pConn, DWORD ClientID, const char* strAccName, DWORD& nLeftTime)
{
	int iRet = ACTION_FAILED;
	if (NULL == pConn)
	{
		return iRet;
	}
	
	if (CheckInputString(strAccName) == 0)
	{
		return iRet;
	}
	
	DWORD NewClientID = 0;
	iRet = GetAccountGameID(pConn, strAccName, NewClientID);
	if (iRet == ACTION_SUCCESS)
	{
		if (NewClientID == ClientID)
		{	
			long iLeft = 0;
			long iExp = 0;
			long iExp2 = 0;
			if (GetLeftSecondsOfDeposit(pConn, strAccName, iLeft, iExp, iExp2) == ACTION_SUCCESS && iLeft > 1800)
			{	
				if (iExp > 0)
				{	
					char strSQL[MAX_PATH];
					sprintf(strSQL, "update View_AccountMoney set nExtPoint = 0 where (iClientID = %d) and (cAccName = '%s')", ClientID, strAccName);
					if(!pConn->Do(strSQL))
					{
						iRet = E_ACCOUNT_ACCESSDENIED;
						nLeftTime = iLeft;
						return iRet;
					}
				}
				if (iExp2 > 0)
				{
					char strSQL[MAX_PATH];
					sprintf(strSQL, "update View_AccountMoney set nExtPoint2 = 0 where (iClientID = %d) and (cAccName = '%s')", ClientID, strAccName);
					if(!pConn->Do(strSQL))
					{
						iRet = E_ACCOUNT_ACCESSDENIED;
						nLeftTime = iLeft;
						return iRet;
					}
				}

				iRet = ACTION_SUCCESS;
				nLeftTime = iLeft;
			}
			else
			{
				iRet = E_ACCOUNT_NODEPOSIT;
			}
		}
		else
			iRet = E_ACCOUNT_ACCESSDENIED;
	}
	return iRet;
}


//

int S3PAccount::LoginGame(S3PDBConVBC* pConn, DWORD ClientID, const char* strAccName)
{
	int iRet = ACTION_FAILED;
	if (NULL == pConn)
	{
		return iRet;
	}
	
	if (CheckInputString(strAccName) == 0)
	{
		return iRet;
	}
	
	DWORD NewClientID = 0;
	iRet = GetAccountGameID(pConn, strAccName, NewClientID);
	if (iRet == ACTION_SUCCESS)
	{
		if (NewClientID == ClientID)
		{	
			char strSQL[MAX_PATH];
			sprintf(strSQL, "update Account_info set dLoginDate = getdate() where (cAccName = '%s') and (iClientID = %d) and (dLoginDate is null)", strAccName, ClientID);
			if (pConn->Do(strSQL))
			{
				iRet = ACTION_SUCCESS;
			}
			else
				iRet = ACTION_FAILED;
		}
		else
			iRet = E_ACCOUNT_ACCESSDENIED;
	}
	return iRet;
}


//

int S3PAccount::Logout(S3PDBConVBC* pConn, DWORD ClientID, const char* strAccName)
{
	int iRet = ACTION_FAILED;
	if (NULL == pConn)
	{
		return iRet;
	}
	
	if (CheckInputString(strAccName) == 0)
	{
		return iRet;
	}
	
	char strSQL[2*MAX_PATH];
	sprintf(strSQL, "update View_AccountMoney set iLeftSecond = iLeftSecond - (datediff(second, dLoginDate, getdate())), iUseSecond = iUseSecond + (datediff(second, dLoginDate, getdate())) where (datediff(second, getdate(), dEndDate) > 0) and (iClientID = %d or iClientID = %d) and (cAccName = '%s') and (dLoginDate is not null)",
		ClientID, GetGMID(), strAccName);
	pConn->Do(strSQL);
	

	DWORD NewClientID = 0;
	iRet = GetAccountGameID(pConn, strAccName, NewClientID);
	if (iRet == ACTION_SUCCESS)
	{
		if (NewClientID == ClientID)
		{
			sprintf(strSQL, "update Account_info set iClientID = 0, dLogoutDate = getdate(), cClientKey = null where (cAccName = '%s') and (iClientID = %d)",
				strAccName, NewClientID);
			if (pConn->Do(strSQL))
			{	
				iRet = ACTION_SUCCESS;
			}
			else
			{
				iRet = ACTION_FAILED;
			}
		}
		else if (NewClientID == GetGMID())
			iRet = ACTION_SUCCESS;
		else
			iRet = E_ACCOUNT_ACCESSDENIED;
	}

	return iRet;
}


//

int S3PAccount::ElapseTime(S3PDBConVBC* pConn, DWORD ClientID, const char* strAccName, const DWORD dwDecSecond)
{
	int iRet = ACTION_FAILED;
	if (NULL == pConn)
	{
		return iRet;
	}
	
	if (CheckInputString(strAccName) == 0)
	{
		return iRet;
	}
	
	DWORD dwSecord = 0;
	iRet = QueryTime(pConn, ClientID, strAccName, dwSecord);
	if (iRet == ACTION_SUCCESS)
	{
		dwSecord = min(dwDecSecond, dwSecord);
		char strSQL[MAX_PATH];
		sprintf(strSQL, "update View_AccountMoney set iLeftSecond = iLeftSecond - %d, iUseSecond = iUseSecond + %d,  where (cAccName = '%s') and (datediff(second, getdate(), dEndDate) > 0) and (iClientID = %d)", dwSecord, dwSecord, strAccName, ClientID);
		if (pConn->Do(strSQL))
		{
			iRet = ACTION_SUCCESS;
		}
		else
			iRet = ACTION_FAILED;
	}
	return iRet;
}


//

int S3PAccount::QueryTime(S3PDBConVBC* pConn, DWORD ClientID, const char* strAccName, DWORD& dwSecond)
{
	dwSecond = 0;
	int iRet = ACTION_FAILED;
	if (NULL == pConn)
	{
		return iRet;
	}
	
	if (CheckInputString(strAccName) == 0)
	{
		return iRet;
	}
	
	DWORD NewClientID = 0;
	iRet = GetAccountGameID(pConn, strAccName, NewClientID);
	if (iRet == ACTION_SUCCESS)
	{
		if (NewClientID == ClientID)
		{
			long iLeftS = 0;
			long iExp = 0;
			long iExp2 = 0;
			iRet = GetLeftSecondsOfDeposit(pConn, strAccName, iLeftS, iExp, iExp2);
			if (iRet == ACTION_SUCCESS)
				dwSecond = max(0, iLeftS);
		}
		else
			iRet = E_ACCOUNT_ACCESSDENIED;
	}
	return iRet;
}


//

int S3PAccount::ServerLogin(S3PDBConVBC* pConn, const char* strAccName, const char* strPassword, const DWORD Address, const short Port, const BYTE Mac[6], unsigned long& nGameID)
{
	nGameID = 0;
	int iRet = ACTION_FAILED;
	if (NULL == pConn)
	{
		return iRet;
	}
	
	if (CheckInputString(strAccName) == 0)
	{
		return iRet;
	}
	if (CheckInputString(strPassword) == 0)
	{
		return iRet;
	}
	
	char strSQL[MAX_PATH];
	sprintf(strSQL, "select cIP, iPort, iid, cMemo from ServerList where (cServerName = '%s') and (cPassword COLLATE Chinese_PRC_CS_AS = '%s')", strAccName, strPassword);
	S3PResultVBC* pResult = NULL;
	if (pConn->QuerySql(strSQL, &pResult))
	{
		if (pResult->num_rows() <= 0)
			iRet = E_ACCOUNT_OR_PASSWORD;
		else
		{
			iRet = E_ADDRESS_OR_PORT;
			if ((Address & 0x0000FFFF) == 0x0000a8c0)
			{
				_variant_t clientid = 0L;
				pResult->get_field_data(2, &clientid, sizeof(_variant_t));
				nGameID = clientid.lVal;
				iRet = ACTION_SUCCESS;	//Local network not check ip
			}
			else
			{
				_variant_t vaddr;
				_variant_t vaMac;
				if (pResult->get_field_data(0, &vaddr, sizeof(_variant_t)) &&
					pResult->get_field_data(3, &vaMac, sizeof(_variant_t)) &&
					vaddr.vt == VT_BSTR && vaMac.vt == VT_BSTR)
				{
					DWORD addr = inet_addr((const char *)(_bstr_t)vaddr);
					char szmac[15];
					sprintf(szmac, "%02X%02X-%02X%02X-%02X%02X", Mac[0], Mac[1], Mac[2], Mac[3], Mac[4], Mac[5]);
					szmac[14] = 0;
					if (addr == Address &&
						strcmpi((const char *)(_bstr_t)vaMac, szmac) == 0)
					{
						_variant_t gameid = 0L;
						pResult->get_field_data(2, &gameid, sizeof(_variant_t));
						nGameID = gameid.lVal;
						iRet = ACTION_SUCCESS;
					}
				}
			}
		}
	}
	
	if (pResult)
		pResult->unuse();

	return iRet;
}


//

int S3PAccount::GetServerID(S3PDBConVBC* pConn, const char* strAccName, unsigned long& nGameID)
{
	nGameID = 0;
	int iRet = ACTION_FAILED;
	if (NULL == pConn)
	{
		return iRet;
	}
	
	if (CheckInputString(strAccName) == 0)
	{
		return iRet;
	}
	
	char strSQL[MAX_PATH];
	sprintf(strSQL, "select iid from ServerList where (cServerName = '%s')", strAccName);
	S3PResultVBC* pResult = NULL;
	if (pConn->QuerySql(strSQL, &pResult))
	{
		if (pResult->num_rows() <= 0)
			iRet = E_ACCOUNT_OR_PASSWORD;
		else
		{
			_variant_t clientid = 0L;
			pResult->get_field_data(0, &clientid, sizeof(_variant_t));
			nGameID = clientid.lVal;
			iRet = ACTION_SUCCESS;	//Local network not check ip
		}
	}
	
	if (pResult)
		pResult->unuse();

	return iRet;
}


//

int S3PAccount::CheckAddress(S3PDBConVBC* pConn, const DWORD Address, const short Port)
{
	int iRet = ACTION_FAILED;
	if (NULL == pConn)
	{
		return iRet;
	}
	if ((Address & 0x0000FFFF) == 0x0000a8c0)
		iRet = ACTION_SUCCESS;	//Local network not check ip
	else
	{
		char strSQL[MAX_PATH];
		in_addr add;
		add.s_addr = Address;
		sprintf(strSQL, "select iid from ServerList where (cIP = '%s')", inet_ntoa(add));
		S3PResultVBC* pResult = NULL;
		if (pConn->QuerySql(strSQL, &pResult))
		{
			if (pResult->num_rows() <= 0)
				iRet = E_ADDRESS_OR_PORT;
			else
				iRet = ACTION_SUCCESS;
		}
		if (pResult)
			pResult->unuse();
	}

	return iRet;
}


//

int S3PAccount::UnlockAll(S3PDBConVBC* pConn, DWORD ClientID)
{
	int iRet = ACTION_FAILED;
	if (NULL == pConn)
	{
		return iRet;
	}
	char strSQL[MAX_PATH];
	sprintf(strSQL, "update Account_info set iClientID = 0, dLogoutDate = getdate() where (iClientID = %d)", ClientID);
	pConn->Do(strSQL);
	
	return ACTION_SUCCESS;
}


//

int S3PAccount::ElapseAll(S3PDBConVBC* pConn, DWORD ClientID)
{
	int iRet = ACTION_FAILED;
	/*if (NULL == pConn)
	{
		return iRet;
	}
	char strSQL[MAX_PATH];
	sprintf(strSQL, "update View_AccountMoney set iLeftSecond = iLeftSecond - (datediff(second, dLoginDate, getdate())), iUseSecond = iUseSecond + (datediff(second, dLoginDate, getdate())) where (datediff(second, getdate(), dEndDate) > 0) and (iClientID = %d) and (dLoginDate is not null)", ClientID);
	if(pConn->Do(strSQL))
	{
		iRet = ACTION_SUCCESS;
	}*/
	
	return iRet;
}


//

int S3PAccount::GetAccountGameID(S3PDBConVBC* pConn, const char* strAccName, DWORD& ClientID)
{
	int iRet = ACTION_FAILED;
	if (NULL == pConn)
	{
		return iRet;
	}
	
	if (CheckInputString(strAccName) == 0)
	{
		return iRet;
	}
	
	char strSQL[MAX_PATH];
	sprintf(strSQL, "select iClientID from Account_info where (cAccName = '%s')", strAccName);
	S3PResultVBC* pResult = NULL;
	if (pConn->QuerySql(strSQL, &pResult))
	{
		if (pResult->num_rows() <= 0)
			iRet = E_ACCOUNT_OR_PASSWORD;
		else
		{
			_variant_t lID = 0L;
			pResult->get_field_data(0, &lID, sizeof(_variant_t));
			ClientID = lID.lVal;
			iRet = ACTION_SUCCESS;
		}
	}
	
	if (pResult)
		pResult->unuse();

	return iRet;
}

int S3PAccount::GetLeftSecondsOfDeposit(S3PDBConVBC* pConn,
										const char* strAccName,
										long& liLeft,
										long& liExp,
										long& liExp2)
{
	int iRet = ACTION_FAILED;
	if (NULL == pConn)
	{
		return iRet;
	}
	
	if (CheckInputString(strAccName) == 0)
	{
		return iRet;
	}
	
	char strSQL[MAX_PATH];
	sprintf(strSQL, "select datediff(second, getdate(), dEndDate), iLeftSecond, nExtPoint, nExtPoint2 from View_AccountMoney where (cAccname = '%s')", strAccName);
	S3PResultVBC* pResult = NULL;
	if (pConn->QuerySql(strSQL, &pResult))
	{
		if (pResult->num_rows() <= 0)
			iRet = E_ACCOUNT_NODEPOSIT;
		else
		{
			_variant_t left = 0L;
			_variant_t diffDate;
			_variant_t extPoint;
			_variant_t extPoint2;
			pResult->get_field_data(0, &diffDate, sizeof(_variant_t));
			pResult->get_field_data(1, &left, sizeof(_variant_t));
			pResult->get_field_data(2, &extPoint, sizeof(_variant_t));
			pResult->get_field_data(3, &extPoint2, sizeof(_variant_t));
			liLeft = left.lVal;

			if (diffDate.vt == VT_I4 && diffDate.lVal > 0)
			{
				liLeft += diffDate.lVal;
			}

			liExp = extPoint.lVal;
			liExp2 = extPoint2.lVal;

			iRet = ACTION_SUCCESS;
		}
	}
	
	if (pResult)
		pResult->unuse();

	return iRet;
}

int S3PAccount::VerifyUserModifyPassword(S3PDBConVBC* pConn, DWORD ClientID, const char* strAccName, const char* strPassword)
{
	int iRet = ACTION_FAILED;
	if (NULL == pConn)
	{
		return iRet;
	}
	
	if (CheckInputString(strAccName) == 0)
	{
		return iRet;
	}
	if (CheckInputString(strPassword) == 0)
	{
		return iRet;
	}
	
	DWORD NewClientID = 0;
	iRet = GetAccountGameID(pConn, strAccName, NewClientID);
	if (iRet == ACTION_SUCCESS)
	{
		if (NewClientID == ClientID)
		{
			char strSQL[MAX_PATH];
			S3PResultVBC* pResult = NULL;
			sprintf(strSQL, "select cAccName from Account_info where (cAccName = '%s') and (cSecPassword COLLATE Chinese_PRC_CS_AS = '%s')", strAccName, strPassword);
			if (pConn->QuerySql(strSQL, &pResult))
			{
				if (pResult->num_rows() <= 0)
					iRet = E_ACCOUNT_OR_PASSWORD;
				else
					iRet = ACTION_SUCCESS;
			}
			else
				iRet = ACTION_FAILED;
			
			if (pResult)
				pResult->unuse();
		}
		else
			iRet = E_ACCOUNT_ACCESSDENIED;
	}
	return iRet;
}


//

int S3PAccount::CheckInputString(const char* strInput)
{	
	if(strchr(strInput,32) != NULL) 
		return 0;

	if(strchr(strInput,39) != NULL) 
		return 0;

	if(strchr(strInput,45) != NULL) 
		return 0;

	return 1; 
}


//

int S3PAccount::VerifyClientLicense(S3PDBConVBC* pConn, const char* strLicense)
{
	int iRet = ACTION_FAILED;
	if (NULL == pConn)
	{
		return iRet;
	}
	char strSQL[MAX_PATH];
	S3PResultVBC* pResult = NULL;

	sprintf(strSQL, "SELECT cBandKey FROM View_AccountLicense WHERE cBandKey = '%s'", strLicense);
	if(pConn->QuerySql(strSQL, &pResult))
	{
		if(pResult->num_rows() > 0)
		{
			if (pResult)
				pResult->unuse();

			return E_CONNECT_BANED;
		}

		if (pResult)
			pResult->unuse();
	}

	sprintf(strSQL, "SELECT cClientKey FROM View_AccountLicense WHERE cClientKey = '%s'", strLicense);
	if (pConn->QuerySql(strSQL, &pResult))
	{
		if (pResult->num_rows() >= MAX_CLIENT_IN_PC)
		{		
			iRet = E_CONNECT_LOCKED;
		}
		else
		{
			iRet = ACTION_SUCCESS;
		}
	}
	else
	{
		iRet = ACTION_FAILED;
	}
	
	if (pResult)
		pResult->unuse();

	return iRet;
}