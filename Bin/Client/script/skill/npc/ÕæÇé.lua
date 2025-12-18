function GetSkillLevelData(levelname, data, level)

if (levelname == "allres_p") then
return Getallres_p(level)
end;

if (levelname == "fastwalkrun_p") then
return Getfastwalkrun_p(level)
end;

if (levelname == "magicenhance_p") then
return Getmagicenhance_p(level)
end;



str1 = ""
return str1
end;

function Param2String(Param1, Param2, Param3)
return Param1..","..Param2..","..Param3
end;

function Getallres_p(level)
result = 10
return Param2String(result,-1,0)
end;

function Getfastwalkrun_p(level)
result = 100
return Param2String(result,-1,0)
end;

function Getmagicenhance_p(level)
result = 10
return Param2String(result,-1,0)
end;

