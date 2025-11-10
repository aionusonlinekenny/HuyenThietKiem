



SKILLS=
{
	
	Block ={ --¸ñµ²
		block_rate={{{1,100},{2,100}},{{1,18*3},{2,18*3},{3,18*3},{4,18*3},{20,18*3}}},
		autorescueskill=
		{
			{
				{1,993*256 + 1},{20,993*256 + 20},
			},
			{
				{1,1},{20,1}
			},
			{
				{1,90*18*256 + 20},
				{2,80*18*256 + 25},
				{3,70*18*256 + 30},
				{4,60*18*256 + 35},
			}
		},
		
		skill_desc=
			function(nLevel)
				
				local nPercent = 20+(nLevel-1) * 5	
				return format("Sinh lùc thÊp h¬n so víi thêi gian dÞch chuyÓn tøc thêi 25%% <color=orange>%d%%<color>cã sö dông x¸c suÊt tù ®éng<color=orange>%d<color>cÊp[<color=orange>%s<color>]\n",
					nPercent,
					nLevel,
					"X¸c suÊt hãa gi¶I s¸t th­¬ng hoµn toµn")
				 
			end,	
		
	},	
	TeleportTemp = 
	{
		
		autorescueskill=
		{
			{
				{1,997*256 + 1},{20,997*256 + 20},
			},
			{
				{1,1},{20,1}
			},
			{
				{1,90*18*256 + 20},
				{2,80*18*256 + 25},
				{3,70*18*256 + 30},
				{4,60*18*256 + 35},
			}
		},
	},
	Teleport_StateTemp =
	{
		allskill_v = 
		{
			{
				{1,1},{2,1},
			},
			{
				{1,4*18},{2,4*18},{3,4*18},{4,4*18},{20,4*18}
			},
			{
				{1,995},
				{2,995},
				
			}
		},
		skill_desc=
			function(nLevel)
				
				local nPercent = 20+(nLevel-1) * 5	
				return format("Sinh lùc thÊp h¬n so víi thêi gian dÞch chuyÓn tøc thêi 25%% <color=orange>%d%%<color>cã sö dông x¸c suÊt tù ®éng<color=orange>%d<color>cÊp[<color=orange>%s<color>]\n",
					nPercent,
					nLevel,
					"DÞch chuyÓn tøc thêi v« h¹n")
				 
			end,
	},
	EnhanceHit=
	{
		autorescueskill=
		{
			{
				{1,998*256 + 1},{20,998*256 + 20},
			},
			{
				{1,1},{20,1}
			},
			{
				{1,90*18*256 + 20},
				{2,80*18*256 + 25},
				{3,70*18*256 + 30},
				{4,60*18*256 + 35},
				
			}
		},
		enhancehit_rate={{{1,100},{2,100}},{{1,18*4},{2,18*4},{3,18*4},{4,18*4},{20,18*4}}},
		skill_desc = function(nLevel)
				
				local nPercent = 20+(nLevel-1) * 5	
				return format("Sinh lùc thÊp h¬n so víi thêi gian dÞch chuyÓn tøc thêi 25%% <color=orange>%d%%<color>cã sö dông x¸c suÊt tù ®éng<color=orange>%d<color>cÊp[<color=orange>%s<color>]\n",
					nPercent,
					nLevel,
					"Träng kÝch tuyÖt ®èi")
				 
			end,
	},
	Common=
	{
		skill_mintimepercast_v={{{1,90*18},{2,80*18},{3,70*18},{4,60*18},{15,60*18},}},
		skill_mintimepercastonhorse_v={{{1,90*18},{2,80*18},{3,70*18},{4,60*18},{15,60*18},}},
	}
}

function SkillExpFunc(Exp0,a,Level,Time,Range)
	return floor(Exp0*(a^(Level-1))*Time*Range/2)
end

function Line(x,x1,y1,x2,y2)
	if(x2==x1) then
		return y2
	end
	return (y2-y1)*(x-x1)/(x2-x1)+y1
end


function Conic(x,x1,y1,x2,y2)
	if((x1 < 0) or (x2<0))then 
		return 0
	end
	if(x2==x1) then
		return y2
	end
	return (y2-y1)*x*x/(x2*x2-x1*x1)-(y2-y1)*x1*x1/(x2*x2-x1*x1)+y1
end

function Extrac(x,x1,y1,x2,y2)
	if((x1 < 0) or (x2<0))then 
		return 0
	end
	if(x2==x1) then
		return y2
	end
	return (y2-y1)*(x-x1)/(x2-x1)+y1
end

function Link(x,points)
	num = getn(points)
	if(num<2) then
		return -1
	end
	for i=1,num do
		if(points[i][3]==nil) then
			points[i][3]=Line
		end
	end
	if(x < points[1][1]) then
		return points[1][3](x,points[1][1],points[1][2],points[2][1],points[2][2])
	end
	if(x > points[num][1]) then
		return points[num][3](x,points[num-1][1],points[num-1][2],points[num][1],points[num][2])
	end
	
	c = 2
	for i=2,num do
		if((x >= points[i-1][1]) and (x <= points[i][1])) then
			c = i
			break
		end
	end
	return points[c][3](x,points[c-1][1],points[c-1][2],points[c][1],points[c][2])
end

function Param2String(Param1, Param2, Param3)
	return Param1..","..Param2..","..Param3
end;


function GetSkillLevelData(levelname, data, level)
	if(data==nil) then
		return ""
	end
	if(data == "") then
		return ""
	end
	if(SKILLS[data]==nil) then
		return ""
	end
	if(SKILLS[data][levelname]==nil) then
		return ""
	end
	if(type(SKILLS[data][levelname]) == "function") then
		return SKILLS[data][levelname](level)
	end
	if(SKILLS[data][levelname][1]==nil) then
		SKILLS[data][levelname][1]={{0,0},{20,0}}
	end
	if(SKILLS[data][levelname][2]==nil) then
		SKILLS[data][levelname][2]={{0,0},{20,0}}
	end
	if(SKILLS[data][levelname][3]==nil) then
		SKILLS[data][levelname][3]={{0,0},{20,0}}
	end
	p1=floor(Link(level,SKILLS[data][levelname][1]))
	p2=floor(Link(level,SKILLS[data][levelname][2]))
	p3=floor(Link(level,SKILLS[data][levelname][3]))
	return Param2String(p1,p2,p3)
end;




