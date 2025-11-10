----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Háa Chi Hé Gi¸p
----------------------------------
Include("\\script\\item\\battles\\checkmapid.lua")

InDist=4; 

function OnUse(nIndex)
	W,X,Y=GetWorldPos();
	X = floor(X / 8);
	Y = floor(Y / 16);

	X1 = floor(GetTaskTemp(201) / (32*8))
	Y1 = floor(GetTaskTemp(202) /(32*16))
	
	if (X1 == 0 or Y1 == 0) then
		Msg2Player("<color=metal>Ng­¬i ch­a ®o¹t ®­îc cê nªn kh«ng thÓ sö dông kú tiªu!") 
		return 0
	end
	strmsg = "Ng­¬i cÇn vËn chuyÓn so¸i kú tíi : <color=yellow>"..X1..","..Y1.."<color>";
	Dist=floor(sqrt((X-X1)*(X-X1)+(Y-Y1)*(Y-Y1)));
	
	if(Dist>=InDist) then
		Msg2Player(strmsg)
		return 0
	else 
		if (GetCurCamp() == 1) then
			SetMissionV(MS_TOTALFLAG_S, GetMissionV(MS_TOTALFLAG_S) + 1)
		else
			SetMissionV(MS_TOTALFLAG_J, GetMissionV(MS_TOTALFLAG_J) + 1)
		end 
		local s_area = GetMissionV(BTHead.Mission.MS_AREASONG)
		local j_area = GetMissionV(BTHead.Mission.MS_AREAJIN)
		local area_section1 = "Area_"..s_area
		local area_section2 = "Area_"..j_area 
		if (GetMissionV(MS_FLAGMODE) == 1) then
			if (GetCurCamp() == 1 and  GetMissionV(MS_TOTALFLAG_S) < TOTALFLAG_FIX) then
				sf_createflag(1, area_section1, area_section2)
			elseif(GetCurCamp() == 2 and  GetMissionV(MS_TOTALFLAG_J) < TOTALFLAG_FIX) then
				sf_createflag(2, area_section2, area_section1)
			end 
		end; 
		
		ResetMask();
		AddSkillState(460, 1, 1, 0 ) 
		AddSkillState(656, 30, 30, 0) 
		AddSkillState(461, 1, 1, 0)  
		SetTaskTemp(201,0)
		SetTaskTemp(202,0)
		
		local award_snapflag = 50 
		
		local TOTAL_FLAG_S = GetMissionV(MS_TOTALFLAG_S)
		local TOTAL_FLAG_J = GetMissionV(MS_TOTALFLAG_J)
		if (GetCurCamp()==1) then 
			Msg2MSAll(MISSIONID, "<color=metal>Phe tèng <color=yellow>"..GetName().."<color> ®· c¾m thµnh c«ng mét so¸i kú. HiÖn t¹i tû lÖ c¾m cê lµ: <color=LightSalmon>Tèng "..TOTAL_FLAG_S..", Kim: "..TOTAL_FLAG_J.."."); 
			Msg2MSAll(MISSIONID, "<color=metal>Mçi binh sü phe tèng nhËn ®­îc <color=yellow>"..award_snapflag.."<color> ®iÓm tÝch lòy");
			nPointFlag = 500
			SetTask(BTHead.Task.T_SNAPFLAG, GetTask(BTHead.Task.T_SNAPFLAG) + 1)	 
			SetTask(BTHead.Task.T_SNAPFLAGPOINT, GetTask(BTHead.Task.T_SNAPFLAGPOINT) + nPointFlag)	 
			SetTask(BTHead.Task.T_SONGJINPOINT, GetTask(BTHead.Task.T_SONGJINPOINT) + nPointFlag)
			local nPoint = GetTask(BTHead.Task.T_TOTALPOINT) + nPointFlag 
			SetTask(BTHead.Task.T_TOTALPOINT, nPoint) 
			SetMissionV(BTHead.Mission.MS_SONGPOINT, GetMissionV(BTHead.Mission.MS_SONGPOINT) + nPointFlag) 
			SeniorHead:AwardFlagObj(5,award_snapflag) 
		else 
			Msg2MSAll(MISSIONID, "<color=metal>Phe kim <color=yellow>"..GetName().."<color> ®· c¾m thµnh c«ng mét so¸i kú. HiÖn t¹i tû lÖ c¾m cê lµ: <color=LightSalmon>Kim "..TOTAL_FLAG_J..", Kim: "..TOTAL_FLAG_S..".");
			Msg2MSAll(MISSIONID, "<color=metal>Mçi binh sü phe kim nhËn ®­îc <color=yellow>"..award_snapflag.."<color> ®iÓm tÝch lòy");
			nPointFlag = 500
			SetTask(BTHead.Task.T_SNAPFLAG, GetTask(BTHead.Task.T_SNAPFLAG) + 1)	 
			SetTask(BTHead.Task.T_SNAPFLAGPOINT, GetTask(BTHead.Task.T_SNAPFLAGPOINT) + nPointFlag)	 
			SetTask(BTHead.Task.T_SONGJINPOINT, GetTask(BTHead.Task.T_SONGJINPOINT) + nPointFlag)
			local nPoint = GetTask(BTHead.Task.T_TOTALPOINT) + nPointFlag 
			SetTask(BTHead.Task.T_TOTALPOINT, nPoint) 
			SetMissionV(BTHead.Mission.MS_JINPOINT, GetMissionV(BTHead.Mission.MS_JINPOINT) + nPointFlag) 
			SeniorHead:AwardFlagObj(6,award_snapflag) 
		end 
		return 1
	end
end
	
	