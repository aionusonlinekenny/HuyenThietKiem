/*****************************************************************************************
//	界面// --login窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-13
// --*/

#pragma once
#pragma pack(push, 1)

#include "KProtocol.h"

#define	LOGIN_ACCOUNT_MIN_LEN	6
#define LOGIN_ACCOUNT_MAX_LEN	16
#define LOGIN_PASSWORD_MIN_LEN	8
#define LOGIN_PASSWORD_MAX_LEN	16
#define LOGIN_ROLE_NAME_MIN_LEN	6
#define	LOGIN_ROLE_NAME_MAX_LEN	16

//login action return value
//the folloin' value is used in login connect operaion
#define	LOGIN_ACTION_FILTER					0xff0000	//登陆过程中的操作类型过滤数值
#define LOGIN_A_CONNECT						0x010000	//连接
#define	LOGIN_A_NEWACCOUNT					0x020000	//新建账号（账号服务器）
#define	LOGIN_A_SERVERLIST					0x030000	//获取服务器列表（账号服务器）
#define	LOGIN_A_REPORT						0x040000	//通知服务器在线(定期向服务器发送)
#define	LOGIN_A_LOGIN						0x050000	//登陆服务器（账号、游戏服务器）
#define	LOGIN_A_LOGOUT						0x060000	//注销
#define	LOGIN_A_CHARACTERLIST				0x070000	//获取角色列表（游戏服务器）


#define	LOGIN_R_REQUEST						0	//when the login request is send form client to server
#define	LOGIN_R_SUCCESS						1
#define	LOGIN_R_FAILED						2
#define	LOGIN_R_ACCOUNT_OR_PASSWORD_ERROR	3
#define	LOGIN_R_ACCOUNT_EXIST				4
#define	LOGIN_R_TIMEOUT						5
#define	LOGIN_R_IN_PROGRESS					6
#define	LOGIN_R_NO_IN_PROGRESS				7
#define	LOGIN_R_VALID						8   
#define	LOGIN_R_INVALID						9  
#define LOGIN_R_INVALID_PROTOCOLVERSION     10
#define LOGIN_R_FREEZE						11
#define LOGIN_R_LOCKED						12
#define LOGIN_R_DENIED						13
#define LOGIN_R_BANED						14

struct KLoginStructHead
{
	unsigned short	Size;
	int				Param;
};


struct KLoginAccountInfo : KLoginStructHead
{
	char	        Account[32];
	KSG_PASSWORD    Password;
	char			License[32];
	unsigned long   nLeftTime;
    
    #ifdef USE_KPROTOCOL_VERSION
    unsigned int    ProtocolVersion;
    #endif
};

#pragma pack(pop)