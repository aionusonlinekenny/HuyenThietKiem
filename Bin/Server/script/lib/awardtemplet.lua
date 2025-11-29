

-- �ļ�������awardtemplet.lua
-- �����ߡ���zhongchaolong
-- ����ʱ�䣺2008-03-20 18:55:45

-- Stub for IncludeLib if not available
if not IncludeLib then
	function IncludeLib(szLibName)
		-- Stub function for C++ library loading
	end
end

if IncludeLib then
	IncludeLib("ITEM")
end

tbAwardTemplet = {}

tbAwardTemplet.TYPE = {}

function tbAwardTemplet:RegType(szKey, pClass)
	self.TYPE[szKey] = pClass
end

function tbAwardTemplet:GivByRandom(tbItem, nAwardCount, tbLogTitle)
	if tbItem == nil then
		return 0
	end
	local rtotal = 10000000
	local rcur=random(1,rtotal);
	local rstep=0;
	for i=1,getn(tbItem) do
		rstep=rstep+floor(tbItem[i].nRate*rtotal/100);
		if(rcur <= rstep) then
			return self:Give(tbItem[i], nAwardCount, tbLogTitle)
		end
	end
end

function tbAwardTemplet:Give(tbItem, nAwardCount, tbLogTitle)
	if not tbItem then --��Ʒ��Ϊ��
		return 0
	end
	nAwardCount = nAwardCount or 1
	if type(tbItem[1]) == "table" then -- ����Ƕ����Ʒ
		if tbItem[1].nRate then --�����ʸ�ĳһ��
			for i = 1, nAwardCount do
				self:GivByRandom(tbItem, 1, tbLogTitle)
			end
			return 1
		else --��˳���ȫ��
			for i = 1,  getn(tbItem) do
				self:Give(tbItem[i], nAwardCount, tbLogTitle)
			end	
			return 1;
		end
	else
		for k, v in self.TYPE do
			if tbItem[k] then
				v:Give(tbItem, nAwardCount, tbLogTitle)
				return 1
			end
		end
	end
end

function tbAwardTemplet:GiveAwardByList(tbItem, szLogTitle, nAwardCount)
	return self:Give(tbItem, nAwardCount, {szLogTitle})
end