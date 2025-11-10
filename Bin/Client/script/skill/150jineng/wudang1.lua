function SkillExpFunc(Exp0,a,Level,Time,Range)
	return floor(Exp0*(a^(Level-1))*Time*Range/2)
end

SKILLS={
	--武当
		renjian_heyi={ --人剑合一
		physicsenhance_p={{{1,25},{15,400},{20,500}}},
		seriesdamage_p={{{1,20},{15,20},{20,60},{21,62}}},
		lightingdamage_v={
			[1]={{1,1},{20,20}},
			[3]={{1,9},{15,185},{20,230}}
		},
		skill_eventskilllevel={{{1,1},{20,20}}},
		attackrating_p={{{1,65},{20,345}}},
		stealmana_p={{{1,1},{20,5}}},
		deadlystrike_p={{{1,16},{20,25}}},
		stun_p={{{1,1},{20,20},{21,20},{22,20}},{{1,1},{19,9},{20,10}}},
		missle_speed_v={{{1,30},{20,30}}},
		missle_lifetime_v={{{1,4},{20,4}}},
		skill_attackradius={{{1,90},{20,90}}},
		skill_cost_v={{{1,35},{20,60}}},
		skill_eventskilllevel={{{1,1},{20,20}}},
		skill_collideevent={
			[1]={{1,0},{15,0},{15,1},{20,1}},
			[3]={{1,162},{20,162}}
		},
		skill_startevent={
			[1]={{1,0},{10,0},{10,1},{20,1}},
			[3]={{1,371},{20,371}}
		},
                addskilldamage1={
			[1]={{1,242},{20,242}},
			[3]={{1,1},{20,20}}
		},
                addskilldamage3={
			[1]={{1,15},{20,180}},
			[3]={{1,1},{20,20}}
		},
                addskilldamage2={
			[1]={{1,243},{20,243}},
			[3]={{1,0},{15,0},{16,16},{20,20}}
		},
                addskilldamage4={
			[1]={{1,10},{20,100}},
			[3]={{1,1},{20,20}}
		},
		skill_showevent={{{1,0},{10,0},{10,1},{15,1},{15,5},{20,5}}},
		addskillexp1={{{1,0},{2,0}},{{1,1},{20,1}},{{1,0},{2,0}}},
--		skill_skillexp_v={{	{1,SkillExpFunc(7200,1.15,1,3,1)},
--							{2,SkillExpFunc(7200,1.15,2,3,1)},
--							{3,SkillExpFunc(7200,1.16,3,3,1)},
--							{4,SkillExpFunc(7200,1.17,4,3,1)},
--							{5,SkillExpFunc(7200,1.18,5,3,1)},
--							{6,SkillExpFunc(7200,1.19,6,3,1)},
--							{7,SkillExpFunc(7200,1.20,7,3,1)},
--							{8,SkillExpFunc(7200,1.21,8,3,1)},
--							{9,SkillExpFunc(7200,1.22,9,3,1)},
--							{10,SkillExpFunc(7200,1.23,10,3,1)},
--							{11,SkillExpFunc(7200,1.24,11,3,1)},
--							{12,SkillExpFunc(7200,1.23,12,3,1)},
--							{13,SkillExpFunc(7200,1.22,13,3,1)},
--							{14,SkillExpFunc(7200,1.21,14,3,1)},
--							{15,SkillExpFunc(7200,1.20,15,3,1)},
--							{16,SkillExpFunc(7200,1.19,16,3,1)},
--							{17,SkillExpFunc(7200,1.18,17,3,1)},
--							{18,SkillExpFunc(7200,1.17,18,3,1)},
--							{19,SkillExpFunc(7200,1.16,19,3,1)},
--							{20,SkillExpFunc(7200,1.15,20,3,1)},
--							}},
	},
	xuanyi_wuxiang={ --玄一无象
		stun_p={{{1,1},{20,1},{21,1},{22,1}},{{1,1},{19,9},{20,10}}},
		skill_misslesform_v={{{1,1},{15,1},{15,6},{20,6},{21,6}}},
		seriesdamage_p={{{1,20},{20,60},{21,62}}},
		lightingdamage_v={
			[1]={{1,1},{20,50}},
			[3]={{1,1},{20,100}}
		},
	},
	taiji_jianyi={ --太极剑意
		stun_p={{{1,1},{20,1},{21,1},{22,1}},{{1,1},{19,9},{20,10}}},
--		seriesdamage_p={{{1,20},{20,60}}},
--		physicsenhance_p={{{1,15},{20,180}}},
	},
	wudang120={ --武当120级技能
		skill_cost_v={{{1,1},{20,1}}},
		--clearnegativestate={{{1,15},{15,75},{20,85},{21,85}}},
		missle_missrate={{{1,85},{15,25},{20,15},{21,15}}},
		ignorenegativestate_p={
			{{1,1},{15,1},{20,1},{21,1}},
			{{1,18},{20,18},{21,18}},
		},
		skill_mintimepercastonhorse_v={{{1,35*18},{15,18*18},{20,15*18},{21,15*18}}},
		skill_mintimepercast_v={{{1,35*18},{15,18*18},{20,15*18},{21,15*18}}},
		skill_desc=
			function(level)
				return "冷却时间：<color=orange>"..floor(Link(level,SKILLS.wudang120.skill_mintimepercast_v[1]) / 18).."秒<color>\n"..
				"骑马施展冷却时间：<color=orange>"..floor(Link(level,SKILLS.wudang120.skill_mintimepercastonhorse_v[1]) / 18).."秒<color>\n" ..
				"有<color=orange>"..floor(100 - Link(level,SKILLS.wudang120.missle_missrate[1])).."%<color>的概率<color>清除周围一定范围内友方玩家的异常状态\n"..
				"有<color=orange>"..floor(100 - Link(level,SKILLS.wudang120_child.missle_missrate[1])).."%<color>的概率<color>使自身对各种负面状态和技能免疫，持续<color=orange>"..
				floor(Link(level,SKILLS.wudang120_child.ignorenegativestate_p[2]) / 18).."秒<color>\n"
			end,
		skill_skillexp_v={{	{1,17851239},
							{2,19487603},
							{3,22760330},
							{4,27669421},
							{5,34214875},
							{6,42396694},
							{7,52214875},
							{8,63669421},
							{9,76760330},
							{10,91487603},
							{11,107851239},
							{12,135669421},
							{13,174942148},
							{14,225669421},
							{15,274418181},
							{16,344618181},
							{17,425738181},
							{18,517778181},
							{19,620738181},
							{20,620738181},
							}},	
	},
	wudang120_child={ --武当120级技能子技能
		missle_missrate={{{1,85},{15,20},{20,15},{21,15}}},
		ignorenegativestate_p={
			{{1,100},{15,100},{20,100},{21,100}},
			{{1,1*18},{20,3*18},{21,3*18}},
		},
	},
}
-----------------------------------------------
--Create by yfeng 2004-05-20
-----------------------------------------------

-----------------------------------------------
--根据2个点，求线形函数f(x)=k*x+b
--y= (y2-y1)*(x-x1)/(x2-x1)+y1
--当x2=x1, 有x=c,该直线是一条垂直于x轴的直线
--这是可以取得y=任意值
--因此，如果已知两点(x1,y1),(x2,y2)可求得过此2点的
--函数为：
function Line(x,x1,y1,x2,y2)
	if(x2==x1) then
		return y2
	end
	return (y2-y1)*(x-x1)/(x2-x1)+y1
end

-----------------------------------------------
--根据2个点，求2次形函数f(x)=a*x2+c
--y= (y2-y1)*x*x/(x2*x2-x1*x1)-(y2-y1)*x1*x1/(x2*x2-x1*x1)+y1
--当x1或者x2 < 0 ,y =0
--当x2=x1, 有x=c,是一条垂直于x轴的直线
--这是可以取得y=任意值
--因此，如果已知两点(x1,y1),(x2,y2)可求得过此2点的
--函数为：extrac
function Conic(x,x1,y1,x2,y2)
	if((x1 < 0) or (x2<0))then 
		return 0
	end
	if(x2==x1) then
		return y2
	end
	return (y2-y1)*x*x/(x2*x2-x1*x1)-(y2-y1)*x1*x1/(x2*x2-x1*x1)+y1
end

-----------------------------------------------
--根据2个点，求-2次形函数f(x)=a*sqrt(x2)+c
--y=(y2-y1)*x/(sqrt(x2)-sqrt(x1))+y1-(y2-y1)/((sqrt(x2)-sqrt(x1))
--当x2或者x1<0, y=0,
--当x1=x2,有x=c,是一条垂直于x轴的直线
--这是可以取得y=任意值
--因此，如果已知两点(x1,y1),(x2,y2)可求得过此2点的
--函数为：extrac
function Extrac(x,x1,y1,x2,y2)
	if((x1 < 0) or (x2<0))then 
		return 0
	end
	if(x2==x1) then
		return y2
	end
	return (y2-y1)*(x-x1)/(x2-x1)+y1
end

-----------------------------------------------
--描绘连接线:Link(x,points)
--根据points提供的一系列点，用相邻的两个点描绘曲线
--return y值
--x 输入值
--points 点集合
--形如：points是形如{{x1,y1,func=xxx},{x2,y2,func=xxx},...{xn,yn,func=xxx}}的映射
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

------------------------------------------------------
--技能设定格式如下：
--SKILLS={
--	技能名称=	{
--		魔法属性=	{
--			[1]={{级别,数值，曲线}，{级别，数值，曲线}，。。。。}，
--			[2]={{级别,数值，曲线}，{级别，数值，曲线}，。。。。}，
--			[3]={{级别,数值，曲线}，{级别，数值，曲线}，。。。。}，	
--		}，
--		魔法属性=	{
--			[1]={{级别,数值，曲线}，{级别，数值，曲线}，。。。。}，
--			[2]={{级别,数值，曲线}，{级别，数值，曲线}，。。。。}，
--			[3]={{级别,数值，曲线}，{级别，数值，曲线}，。。。。}，	
--		}，
--		。。。。。
--	}，
--	技能名称=	{
--		魔法属性=	{
--			[1]={{级别,数值，曲线}，{级别，数值，曲线}，。。。。}，
--			[2]={{级别,数值，曲线}，{级别，数值，曲线}，。。。。}，
--			[3]={{级别,数值，曲线}，{级别，数值，曲线}，。。。。}，	
--		}，
--		魔法属性=	{
--			[1]={{级别,数值，曲线}，{级别，数值，曲线}，。。。。}，
--			[2]={{级别,数值，曲线}，{级别，数值，曲线}，。。。。}，
--			[3]={{级别,数值，曲线}，{级别，数值，曲线}，。。。。}，	
--		}，
--		。。。。。
--	}，
--	。。。。。
--}
--如：
--SKILLS={
--	Sanhuan-taoyue={
--		physicsenhance_p={
--			[1]={{1,50},{20,335}},--魔法属性physicsenhance_p参数1，1级时为35，20级时为335，曲线不填，默认线形
--			[2]={{1,0},{20,0}},
--		},--没有[3]，表示魔法属性physicsenhance_p参数2，默认为任何时候都是0
--		lightingdamage_v={
--			[1]={{1,65},{20,350}},
--			[3]={{1,65},{20,350}},
--		}
--	}
--}
--以上描述技能“三环套月”的魔法属性和数值
-----------------------------------------------------------
--函数GetSkillLevelData(levelname, data, level)
--levelname：魔法属性名称
--data：技能名称
--level：技能等级
--return：当技能名称为data，技能等级为level
--			时的魔法属性levelname所需求的三个参数的具体值
-----------------------------------------------------------
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


function Param2String(Param1, Param2, Param3)
return Param1..","..Param2..","..Param3
end;
