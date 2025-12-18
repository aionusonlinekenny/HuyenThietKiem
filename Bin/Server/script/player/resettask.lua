Include("\\script\\lib\\TaskLib.lua")
Include("\\script\\global\\luanpcfunctions\\shopvip.lua")
--*******************************************RESETTASK*********************************************--
nYr,nMo,nDy,nHr,nMi,nSe,nDyfW = 0,0,0,0,0,0,0;
function CheckTaskReset()
	nYr,nMo,nDy,nHr,nMi,nSe,nDyfW = GetTimeNow();
	if nYr ~= GetTask(T_YEAR) or nMo ~= GetTask(T_MONTH) or nDy ~= GetTask(T_DAY) then
		----------PHONG LANG DO
		SetTask(600,0);
		----------VUOT AI
		SetTask(602,0);
		----------TONG KIM
		
		----------LIEN DAU
		
		----------DA TAU
		SetTask(TASK_COUNT_QUEST,0);
		SetTask(TASK_CANCEL_QUEST,0);
		SetTask(TASK_DT_KEY,0);
		----------HUY HOANG
		SetTask(T_LIMIT_HOANGKIM,0);
		SetTask(T_LIMIT_HUYHOANG,0);
		----------HOA DANG
		SetTask(T_REPCAUHOI,0);
		SetTask(T_RIGHTQUESTION,0);
		----------SAT THU

		SetTask(803,0); -- 8 lan/ngay
		----------LOAN CHIEN
		if (GetRank() == 193 or GetRank() == 194 or GetRank() == 195) then
			--SetRank(0);
		end;		
		----------SERVER
		SetTask(T_DAY,				nDy);
		SetTask(T_MONTH,			nMo);	
		SetTask(T_YEAR,				nYr);		
		--VIP
		ResetVIP();	
		----------TIN SU THIEN BAO KHO
		SetTask(T_TIN_SU,0);
		SetTask(T_BK_KEY_1,0);
		SetTask(T_BK_KEY_2,0);
		SetTask(T_BK_KEY_3,0);
		SetTask(T_BK_KEY_4,0);
		SetTask(T_BK_KEY_5,0);
		SetTask(T_BK_KEY_DONE_1,0);
		SetTask(T_BK_KEY_DONE_2,0);
		SetTask(T_BK_KEY_DONE_3,0);
		SetTask(T_BK_KEY_DONE_4,0);
		SetTask(T_BK_KEY_DONE_5,0);
		SetTask(T_TIN_SU_LB,0);
	end
end