--迭代函数，用于计算技能熟练度
--具体方法：
--根据1级熟练度，升级加速度，级数，重复伤害次数，范围，计算出相应等级熟练度
-- SkillExp(i) = Exp1*a^(i-1)*time*range
function SkillExpFunc(Exp0,a,Level,Time,Range)
	return floor(Exp0*(a^(Level-1))*Time*Range/2)
end


SKILLS={
	--翠烟
		bingzong_wuying={ --冰踪无影
		physicsenhance_p={{{1,45},{15,1060},{20,1246}}},
		seriesdamage_p={{{1,20},{15,20},{20,60},{21,62}}},
		colddamage_v={
			[1]={{1,10},{15,640},{20,773}},
			[3]={{1,50},{15,750},{20,876}}
		},
		missle_speed_v={{{1,20},{20,24},{21,24}}},
		skill_attackradius={{{1,448},{20,512},{21,512}}},
		skill_cost_v={{{1,40},{20,60}}},
		skill_eventskilllevel={{{1,1},{20,20}}},
		skill_collideevent={
			[1]={{1,0},{10,0},{10,1},{20,1}},
			[3]={{1,382},{20,382}}
		},
                addskilldamage1={
			[1]={{1,171},{20,171}},
			[3]={{1,0},{4,0},{5,5},{20,20}}
		},
                addskilldamage3={
			[1]={{1,15},{20,115}},
			[3]={{1,1},{20,20}}
		},
		skill_showevent={{{1,0},{10,0},{10,4},{20,4}}},
		addskillexp1={{{1,0},{2,0}},{{1,1},{20,1}},{{1,0},{2,0}}},
		skill_misslenum_v={{{1,1},{5,1},{20,5},{21,5}}},
		skill_skillexp_v={{	{1,SkillExpFunc(8000,1.25,1,1,1)},
							{2,SkillExpFunc(8000,1.15,2,1,1)},
							{3,SkillExpFunc(8000,1.16,3,1,1)},
							{4,SkillExpFunc(8000,1.17,4,1,1)},
							{5,SkillExpFunc(8000,1.18,5,1,1.5)},
							{6,SkillExpFunc(8000,1.19,6,1,1.5)},
							{7,SkillExpFunc(8000,1.20,7,1,1.5)},
							{8,SkillExpFunc(8000,1.21,8,1,1.5)},
							{9,SkillExpFunc(8000,1.22,9,1,1.5)},
							{10,SkillExpFunc(8000,1.23,10,1,2)},
							{11,SkillExpFunc(8000,1.24,11,1,2)},
							{12,SkillExpFunc(8000,1.23,12,1,2)},
							{13,SkillExpFunc(8000,1.22,13,1,2)},
							{14,SkillExpFunc(8000,1.21,14,1,2)},
							{15,SkillExpFunc(8000,1.20,15,1,3)},
							{16,SkillExpFunc(8000,1.19,16,1,3)},
							{17,SkillExpFunc(8000,1.18,17,1,3)},
							{18,SkillExpFunc(8000,1.17,18,1,3)},
							{19,SkillExpFunc(8000,1.16,19,1,3)},
							{20,SkillExpFunc(8000,1.15,20,1,4)},
							}},
	},
	bingxin_yuling={ --冰心玉凌
		seriesdamage_p={{{1,20},{20,60},{21,62}}},
		physicsenhance_p={{{1,15},{20,115}}},
		colddamage_v={
			[1]={{1,10},{20,490}},
			[3]={{1,10},{20,490}}
		},
		skill_eventskilllevel={{{1,1},{20,20}}},
		skill_collideevent={
			[1]={{1,0},{15,0},{15,1},{20,1}},
			[3]={{1,382},{20,382}}
		},
		skill_showevent={{{1,0},{15,0},{15,4},{20,4}}},
	},
	bingxin_xuelian={ --冰心雪莲
		skill_misslenum_v={{{1,0},{5,1},{11,1},{20,1},{21,1}}},
		seriesdamage_p={{{1,20},{20,60},{21,62}}},
		physicsenhance_p={{{1,15},{20,115}}},
		colddamage_v={
			[1]={{1,10},{20,170}},
			[3]={{1,10},{20,170}}
		},
	},
	bingxin_xianzi={ --冰心仙子
		seriesdamage_p={{{1,20},{15,20},{20,60},{21,62}}},
		physicsdamage_v={
			[1]={{1,50},{15,650},{20,850}},
			[3]={{1,50},{15,650},{20,850}},
		},
		colddamage_v={
			[1]={{1,15},{15,1050},{20,1210}},
			[3]={{1,15},{15,1050},{20,1210}}
		},
                addskilldamage1={
			[1]={{1,173},{20,173}},
			[3]={{1,0},{10,0},{11,11},{20,20}}
		},
                addskilldamage5={
			[1]={{1,0},{10,0},{20,600}},
			[3]={{1,1},{20,20}}
		},
		missle_speed_v={{{1,28},{20,32},{21,32}}},
		skill_attackradius={{{1,448},{20,512},{21,512}}},
		skill_cost_v={{{1,45},{20,75}}},
		skill_eventskilllevel={{{1,1},{20,20}}},
		skill_flyevent={
			[1]={{1,0},{10,0},{10,1},{20,1}},
			[2]={{1,3},{2,3}},
			[3]={{1,338},{20,338}}
		},
		skill_showevent={{{1,0},{10,0},{10,2},{20,2}}},
		addskillexp1={{{1,0},{2,0}},{{1,1},{20,1}},{{1,0},{2,0}}},
		skill_skillexp_v={{	{1,SkillExpFunc(13000,1.25,1,1,1)},
							{2,SkillExpFunc(13000,1.15,2,1,1)},
							{3,SkillExpFunc(13000,1.16,3,1,1)},
							{4,SkillExpFunc(13000,1.17,4,1,1)},
							{5,SkillExpFunc(13000,1.18,5,1,1)},
							{6,SkillExpFunc(13000,1.19,6,1,1)},
							{7,SkillExpFunc(13000,1.20,7,1,1)},
							{8,SkillExpFunc(13000,1.21,8,1,1)},
							{9,SkillExpFunc(13000,1.22,9,1,1)},
							{10,SkillExpFunc(13000,1.23,10,1,1)},
							{11,SkillExpFunc(13000,1.24,11,1,1)},
							{12,SkillExpFunc(13000,1.23,12,1,1)},
							{13,SkillExpFunc(13000,1.22,13,1,1)},
							{14,SkillExpFunc(13000,1.21,14,1,1)},
							{15,SkillExpFunc(13000,1.20,15,1,1)},
							{16,SkillExpFunc(13000,1.19,16,1,1)},
							{17,SkillExpFunc(13000,1.18,17,1,1)},
							{18,SkillExpFunc(13000,1.17,18,1,1)},
							{19,SkillExpFunc(13000,1.16,19,1,1)},
							{20,SkillExpFunc(13000,1.15,20,1,1)},
							}},
	},
	fengxue_bingtian={ --风雪冰天
		skill_misslenum_v={{{1,0},{10,0},{11,1},{20,1},{21,1}}},
		seriesdamage_p={{{1,20},{20,60},{21,62}}},
		colddamage_v={
			[1]={{1,45},{20,500}},
			[3]={{1,45},{20,500}}
		},
	},
	cuiyan120={ --翠烟120级技能
		skill_cost_v={{{1,1},{20,1}}},
		hide={{{1,1},{20,1}},{{1,5*18},{15,25*18},{20,30*18},{21,30*18}}},
		skill_mintimepercast_v={{{1,60*18},{15,45*18},{20,40*18},{21,40*18}}},
		skill_mintimepercastonhorse_v={{{1,60*18},{15,45*18},{20,40*18},{21,40*18}}},
		skill_desc=
			function(level)
				return "冷却时间：<color=orange>"..floor(Link(level,SKILLS.cuiyan120.skill_mintimepercast_v[1]) / 18).."秒<color>\n"
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

