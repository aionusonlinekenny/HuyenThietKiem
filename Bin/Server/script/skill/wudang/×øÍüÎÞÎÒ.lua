function GetSkillLevelData(levelname, data, level)

if (levelname == "manashield_p") then
return Getmanashield_p(level)
end;

str1 = ""
return str1
end;

function Param2String(Param1, Param2, Param3)
return Param1..","..Param2..","..Param3
end;

function Getmanashield_p(level)
result = 19+level*4
return Param2String(result,-1,0)
end;
