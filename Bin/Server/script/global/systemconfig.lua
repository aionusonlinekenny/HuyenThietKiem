--------------------------------------
-- ϵͳ���ã����ء�״̬��

---------------------------------------------------------------
-- Stub for GetProductRegion if not available (C++ function)
if not GetProductRegion then
	function GetProductRegion()
		-- Default to Vietnam version for HuyenThietKiem
		return "vn", 4;
	end
end

---------------------------------------------------------------
-- ����汾����
DEF_PRODUCT_REGION_CN		= 0;		-- �й���½�汾
DEF_PRODUCT_REGION_CN_IB	= 1;		-- �й���½��Ѱ汾
DEF_PRODUCT_REGION_TW		= 2;		-- ̨��汾
DEF_PRODUCT_REGION_MY		= 3;		-- Խ�ϰ汾
DEF_PRODUCT_REGION_VN		= 4;		-- Խ�ϰ汾

SYSCFG_PRODUCT_REGION_NAME, SYSCFG_PRODUCT_REGION_ID = GetProductRegion();	-- ��ǰ�汾

---------------------------------------------------------------
-- Ǯׯ�������� (1 - ������nil - �ر�)
SYSCFG_GAMEBANK_GOLDSILVER_OPEN = 1; -- ����Ԫ�����ܿ���
SYSCFG_EXTPOINTID1_TYPEPAY = 1; -- H� th�ng ti�n n�p th� t�i Ti�n Trang. 0:Ti�n ��ng, 1: KNB
SYSCFG_GAMEBANK_GOLD_GET 		= 1; -- ��Ԫ����ȡ
SYSCFG_GAMEBANK_GOLD_PAY 		= 1; -- ��Ԫ����ֵ
SYSCFG_GAMEBANK_GOLD_COIN 	= 1; -- ��Ԫ����ͭǮ
SYSCFG_GAMEBANK_GOLD_USE 		= 1; -- ��Ԫ��������������;

SYSCFG_GAMEBANK_SILVER_GET 	= 1; -- ��Ԫ����ȡ
SYSCFG_GAMEBANK_SILVER_PAY 	= 1; -- ��Ԫ����ֵ
SYSCFG_GAMEBANK_SILVER_COIN = 1; -- ��Ԫ����ͭǮ
SYSCFG_GAMEBANK_SILVER_USE 	= 1; -- ��Ԫ��������������;

SYSCFG_GAMEBANK_TICKET_OPEN = 1; -- ��Ʊ���ܿ���
SYSCFG_GAMEBANK_TICKET_GET 	= 1; -- ��Ʊ��ȡ
SYSCFG_GAMEBANK_TICKET_PAY 	= 1; -- ��Ʊ��ֵ
SYSCFG_GAMEBANK_TICKET_COIN = 1; -- ��Ʊ��ͭǮ
SYSCFG_GAMEBANK_TICKET_USE 	= 1; -- ��Ʊ������������;
---------------------------------------------------------------
-- LLG_ALLINONE_TODO_20070802
--��չ���ʹ��
SYSCFG_EXTPOINTID7_LOGINMSG		= 1;	--������Ϸʱ��������չ��״̬������Ϣ��ʾ

---------------------------------------------------------------
-- ����������� (1 - ������nil - �ر�)
SYSCFG_SHOP_OPEN            = 1;
---------------------------------------------------------------

---------------------------------------------------------------
-- �һ����������� (1 - ������nil - �ر�)
SYSCFG_TAOHUAISLAND_OPEN    = 1;
---------------------------------------------------------------

---------------------------------------------------------------
-- ÿ����ȡ������������ (1 - ������nil - �ر�)
SYSCFG_AWARDPERDAY_OPEN     = nil;
if (SYSCFG_PRODUCT_REGION_ID == DEF_PRODUCT_REGION_TW or SYSCFG_PRODUCT_REGION_ID == DEF_PRODUCT_REGION_VN) then
	SYSCFG_AWARDPERDAY_OPEN = 1;
end
---------------------------------------------------------------

---------------------------------------------------------------
-- ת�����û���ȡ������������ (1 - ������nil - �ر�)
SYSCFG_PAYMONTHAWARD_OPEN     = nil;
if (SYSCFG_PRODUCT_REGION_ID == DEF_PRODUCT_REGION_TW or SYSCFG_PRODUCT_REGION_ID == DEF_PRODUCT_REGION_VN) then
	SYSCFG_PAYMONTHAWARD_OPEN = 1;
end
---------------------------------------------------------------

---------------------------------------------------------------
-- ͬ�鹦������ (1 - ������nil - �ر�)
SYSCFG_PARTNER_OPEN     = nil;
if (SYSCFG_PRODUCT_REGION_ID == DEF_PRODUCT_REGION_CN or SYSCFG_PRODUCT_REGION_ID == DEF_PRODUCT_REGION_TW or SYSCFG_PRODUCT_REGION_ID == DEF_PRODUCT_REGION_VN) then
	SYSCFG_PARTNER_OPEN = 1;
end
---------------------------------------------------------------

---------------------------------------------------------------
-- �°�Ṧ������ (1 - ������nil - �ر�)
SYSCFG_NEWTONG_OPEN     = nil;
if (SYSCFG_PRODUCT_REGION_ID == DEF_PRODUCT_REGION_CN or SYSCFG_PRODUCT_REGION_ID == DEF_PRODUCT_REGION_CN_IB or SYSCFG_PRODUCT_REGION_ID == DEF_PRODUCT_REGION_VN) then
	SYSCFG_NEWTONG_OPEN = 1;
end
---------------------------------------------------------------

---------------------------------------------------------------
function IncludeForRegionVer(strPath, strLuaFileName)
	
	local strFullName = strPath;
		
	if(SYSCFG_PRODUCT_REGION_NAME == nil) then	
		print("region_version error!!!\n");
		return
	end
	
	strFullName = strFullName..SYSCFG_PRODUCT_REGION_NAME.."\\"..strLuaFileName;
	print(strFullName);	
	Include(strFullName);
end

-- �ж�����Ƿ� VIP
function IsVip()
	if (GetAccLeftTime() > 0) then 
		return 1;
	end
	return 0
end;