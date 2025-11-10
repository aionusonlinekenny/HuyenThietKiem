----------------------------------
--	Copyright: JX Legend by Legend Team
--	Author: Kinnox
--	Date: 23/10/2013
--	Desc: Script level Npc ®èi tho¹i
----------------------------------
Include("\\script\\npclevelscript\\property.lua")

----------------------------------
--
----------------------------------
function GetNpcLevelData(Series, Level, StyleName, ParamStr)
	if(StyleName=="PasstSkillLevel") then
		param1=GetParam(ParamStr,1);
		param2=GetParam(ParamStr,2);
		return SetPasstSkillLevel(Level,param1,param2);
	end
	
	return ParamStr
end

----------------------------------
--
----------------------------------
function GetNpcKeyData(Series, Level, StyleName, Param1, Param2, Param3)
	result = Param1 * Level * Level + Param2 * Level + Param3;
	return result;
end;