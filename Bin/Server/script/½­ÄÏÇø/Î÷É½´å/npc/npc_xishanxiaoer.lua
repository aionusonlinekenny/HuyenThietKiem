-- Î÷É½´å Î÷É½Ğ¡¶ş ¶Ô»°½Å±¾
-- ÀîĞÀ 2005-01-25
IncludeLib("SETTING")
Include("\\script\\task\\newtask\\newtask_head.lua")
Include("\\script\\task\\newtask\\xishancun\\xishancun_head.lua")
Include("\\script\\task\\system\\task_string.lua"); 
Include("\\script\\activitysys\\g_activity.lua")
Include("\\script\\dailogsys\\g_dialog.lua")
Include("\\script\\activitysys\\playerfunlib.lua")
Include("\\script\\global\\titlefuncs.lua")
Include("\\script\\lib\\awardtemplet.lua")
Include("\\script\\global\\fuyuan.lua")
Include("\\script\\missions\\leaguematch\\npc\\officer.lua")
Include("\\script\\lib\\log.lua")
Include("\\script\\global\\vldoxanh\\vsd\\fucmain.lua")

function main()
--	Uworld1064 = nt_getTask(1064)
--	local name = GetName()
--	if	( Uworld1064 < 2) then  -- ÅĞ¶ÏÃ»ÓĞÍê³ÉÎ÷É½ÓìÈÎÎñ
--		Talk(1,"","Î÷É½Ğ¡¶ş£ºÕâ¼¸ÈÕÌìÆø·´¸´ÎŞ³££¬ÎÒ¶¼Ã»ĞÄË¼³öº£´òÓãÁË¡£")
--	elseif ( Uworld1064 == 2 ) then   -- ÅĞ¶ÏÒÑ¾­Íê³ÉÁËÎ÷É½ÓìÈÎÎñ£¬µ«ÉĞÎ´È¥Î÷É½Óì£¬²¢ÇÒÎ´ºÍÎ÷É½Ğ¡¶ş¶Ô»°¹ı
--		-- Say("Î÷É½Ğ¡¶ş£ºÄãÒªÈ¥Î÷É½Óì£¿¿ÉÏ§Î÷É½Óì»¹Ã»¿ª·Å£¬ÔİÊ±²»ÄÜ¹ıÈ¥¡£²»¹ıÄã·ÅĞÄ£¬ÎÒÒÑ¾­¼Ç×¡ÄãÁË£¬¹ıĞ©ÈÕ×ÓÄãÖ±½ÓÀ´ÕÒÎÒºÃÁË£¬ÎÒ´øÄãÉÏÎ÷É½Óì¡£",0)
--		Describe(DescLink_XiShanXiaoEr.."<#>£ºÕâÎ»¿Í¹Ù£¬ÕÒÎÒÓĞºÎ¹ó¸É£¿ÊÇ²»ÊÇÒªÂòÓãÄØ£¿<enter>"
--		..name.."<#>£º²»ÊÇµÄ£¬ÊÇ²»ĞÑÈËÊ¿ÈÃÎÒÀ´ÕÒÄãµÄ¡£<enter>Î÷É½Ğ¡¶ş£ºÊÇËû£¬ÎÒÖªµÀÁË¡£ÄãÒ²ÊÇÈ¥Î÷É½Óì°É£¬×ß°É£¬ÎÒÕâ¾Í´øÄãÈ¥¡£²»¹ıÎ÷É½ÓìÕâ¸öµØ·½À´È¥¿É²»ÊÇÄÇÃ´·½±ã£¬¶øÇÒĞ×ÏÕÒì³££¬ÄãÒª¶à¼ÓĞ¡ĞÄÓ´¡£<enter>",
--		2,"ÎÒÃÇÆô³Ì°É/task007","ÎÒ»¹ÊÇ¹ı»áÔÙÈ¥°É/task006")
--	elseif ( Uworld1064 == 3) then   -- ÅĞ¶ÏÒÑ¾­Íê³ÉÁËÎ÷É½ÓìÈÎÎñ£¬ÇÒºÍÎ÷É½Ğ¡¶ş¶Ô»°¹ıÒ»´Î£¬µ«ÉĞÎ´È¥Î÷É½Óì
--		Say("Î÷É½Ğ¡¶ş£ºÄãÏÖÔÚÒÑ¾­×¼±¸ºÃÈ¥Î÷É½ÓìÁËÂğ£¿",2,"ÎÒÃÇÆô³Ì°É/task007","ÎÒ»¹ÊÇ¹ı»áÔÙÈ¥°É/task006");
--	 end;
	
	local nNpcIndex = GetLastDiagNpc();
	local szNpcName = GetNpcName(nNpcIndex);
	if NpcName2Replace then szNpcName = NpcName2Replace(szNpcName) end
	local tbDailog = DailogClass:new(szNpcName);
	tbDailog.szTitleMsg = "T©y S¬n tiÓu nhŞ: §¹i hiÖp muèn ®i ®Õn Vi S¬n ®¶o? Tr­íc tiªn h·y giao nép LÖnh Bµi Vi S¥n §¶o míi cã thÓ ®i ®Õn Vi S¬n ®¶o ®­îc."
	

	tbDailog:AddOptEntry("§ång ı! Ta sÏ t×m ngay.", task007, {}); 
	tbDailog:AddOptEntry("NhËn nhiÖm vô Vi S¬n §¶o", VLMC_main2, {}); 
	
	G_ACTIVITY:OnMessage("ClickNpc", tbDailog, nNpcIndex);
	
	tbDailog:Show();
	
end;
function task006()	
	Uworld1064 = nt_getTask(1064)
	nt_setTask(1064,3)
end;
	
function task007()
	if (GetLevel() < 100) then
		Say("T©y S¬n tiÓu nhŞ: D­íi cÊp 100 kh«ng thÓ ®i Vi S¬n ®¶o ®­îc!",0);
		return
	end
	
	GiveItemUI("Giao diÖn tr¶ phİ.","Xin h·y bá lÖnh bµi vµo « bªn d­íi.", "task008", "no")
--	Uworld1064 = nt_getTask(1064)
--	nt_setTask(1064,0)
--	SetFightState(1);
--	NewWorld(342,1177,2410);
--	DisabledUseTownP(1)	--ÏŞÖÆÆäÔÚÎ÷É½ÓìÄÚÊ¹ÓÃ»Ø³Ç·û
--	SetRevPos(175,1);		--ÉèÖÃÖØÉúµã
end;

function task008(ncount)
	local scrollarray = {}
	local scrollcount = 0
	local scrollidx = {}
	local y = 0
	for i=1, ncount do
		local nItemIdx = GetGiveItemUnit(i);
		itemgenre, detailtype, parttype = GetItemProp(nItemIdx)
		if (itemgenre == 6 and detailtype == 1 and parttype ==2432) then	
			y = y + 1
			scrollidx[y] = nItemIdx;
			scrollarray[i] = GetItemStackCount(nItemIdx)
			scrollcount = scrollcount + scrollarray[i]
		end
	end
	if (y ~= ncount) then
		Talk(1,"","LÖnh bµi Vi S¬n §·o ch­a ®ñ, ng­¬i h·y kiÓm tra l¹i xem!")
		return
	end
	if (scrollcount > 1) then
		Talk(1,"","Ta chØ cÇn 1 lÖnh bµi Vi S¬n §¶o, ng­¬i ®õng ®­a cho ta nhiÒu nh­ vËy")
		return
	end
	if (scrollcount < 1) then
		Talk(1,"","LÖnh bµi Vi S¬n §¶o ng­¬i giao cho ta kh«ng ®ñ th× ph¶i, kiÓm tra l¹i xem!")
		return
	end
	if (scrollcount == 1) then
		for i = 1, y do
			RemoveItemByIndex(scrollidx[i])
		end
	end;
	--SetFightState(0);
	NewWorld(342,1178,2412)
	SetFightState(1);
end

function no()
end
