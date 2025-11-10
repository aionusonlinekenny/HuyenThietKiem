--迭代函数，用于计算技能熟练度
--具体方法：
--根据1级熟练度，升级加速度，级数，重复伤害次数，范围，计算出相应等级熟练度
-- SkillExp(i) = Exp1*a^(i-1)*time*range
EXP_PER = 60
function SkillExpFunc(Exp0,a,Level,Time,Range)
	return floor(Exp0*(a^(Level-1))*Time*Range/2)
end

SKILLS={
	--翠烟
	fenghua_xueyue={ --Phong Hoa Tuyet Nguyet
		physicsenhance_p={{{1,5},{20,85}}},
		seriesdamage_p={{{1,1},{20,10}}},
		addskilldamage1={
			[1]={{1,336},{2,336}},
			[3]={{1,1},{20,55}}
		},
		addskilldamage2={
			[1]={{1,108},{2,108}},
			[3]={{1,1},{20,35}}
		},
		addskilldamage3={
			[1]={{1,1063},{2,1063}},
			[3]={{1,1},{20,45}}
		},
		addskilldamage4={
			[1]={{1,1064},{2,1064}},
			[3]={{1,1},{20,45}}
		},
		colddamage_v={
			[1]={{1,5},{20,80}},
			[3]={{1,5},{20,150}}
		},
		missle_speed_v={{{1,20},{20,24},{21,24}}},
		skill_attackradius={{{1,320},{20,384}}},
		skill_cost_v={{{1,10},{20,10}}}
	},
	cuiyan_daofa={ --Thuy Yen Dao phap
		addphysicsdamage_p={{{1,45},{20,215}},{{1,-1},{2,-1}},{{1,1},{2,1}}},
		deadlystrikeenhance_p={{{1,6},{20,35,Conic}},{{1,-1},{2,-1}}}
	},
	cuiyan_shuangdao={ --Thuy yen Song dao
		addcoldmagic_v={{{1,20},{20,215}},{{1,-1},{2,-1}}}
	},
	huti_hanbing={ --Ho the Han bang
		meleedamagereturn_p={{{1,5},{20,20}},{{1,18*120},{20,18*120}}},
		rangedamagereturn_p={{{1,5},{20,20}},{{1,18*120},{20,18*120}}},
		skill_cost_v={{{1,40},{20,60}}}
	},
	fengjuan_canxue={ --Phong Quyen tan Tuyet
		seriesdamage_p={{{1,1},{20,10}}},
		colddamage_v={
			[1]={{1,10},{20,75}},--plus
			[3]={{1,10},{20,125}}--plus
		},
		physicsdamage_v={
			[1]={{1,25},{20,235}},
			[3]={{1,25},{20,375}},
		},
		addskilldamage1={
			[1]={{1,337},{2,337}},
			[3]={{1,1},{20,115}}
		},
		addskilldamage2={
			[1]={{1,111},{2,111}},
			[3]={{1,1},{20,35}}
		},
		addskilldamage3={
			[1]={{1,1065},{2,1065}},
			[3]={{1,1},{20,95}}
		},
		addskilldamage4={
			[1]={{1,1093},{2,1093}},
			[3]={{1,1},{20,95}}
		},
		missle_speed_v={{{1,20},{20,24},{21,24}}},
		skill_attackradius={{{1,320},{20,384}}},
		skill_cost_v={{{1,20},{20,20}}}
	},
	bingxin_qianying={ --Bang tam Trai anh
		lifereplenish_v={{{1,130},{20,700}},{{1,8},{2,8}}},
		skill_cost_v={{{1,21},{20,40}}}
	},
	yuda_lihua={ --Vu Da Le Hoa
		physicsenhance_p={{{1,10},{20,140}}},
		seriesdamage_p={{{1,5},{20,30}}},
		addskilldamage1={
			[1]={{1,382},{2,382}},
			[3]={{1,15},{20,50}}
		},
		addskilldamage2={
			[1]={{1,1064},{2,1064}},
			[3]={{1,1},{20,42}}
		},
		colddamage_v={
			[1]={{1,10},{20,100}},
			[3]={{1,10},{20,250}}
		},
		skill_attackradius={{{1,384},{20,448}}},
		skill_cost_v={{{1,20},{20,30}}}
	},
	xueying={ --Tuyet Anh
		attackspeed_v={{{1,12},{20,65}},{{1,18*120},{20,18*180}}},
		castspeed_v={{{1,12},{20,65}},{{1,18*120},{20,18*180}}},
		fastwalkrun_p={{{1,17},{20,55}},{{1,18*120},{20,18*180}}},
		skill_cost_v={{{1,40},{20,140}}}
	},
	taxue_wuhen={ --Dap Tuyet vo Ngan - ko su dung
--		fastwalkrun_p={{{1,17},{20,55}},{{1,18*120},{20,18*180}}},
--		skill_cost_v={{{1,24},{20,100}}}
	},
	muye_liuxing={ --Muc Da luu tinh
		seriesdamage_p={{{1,10},{20,50},{21,52}}},
		physicsenhance_p={{{1,30},{20,271}}},
		colddamage_v={
			[1]={{1,20},{20,246}},
			[3]={{1,20},{20,426}}
		},
		addskilldamage1={
			[1]={{1,336},{2,336}},
			[3]={{1,1},{20,60}}
		},
		addskilldamage2={
			[1]={{1,1063},{2,1063}},
			[3]={{1,1},{20,50}}
		},
		addskilldamage3={
			[1]={{1,1064},{2,1064}},
			[3]={{1,1},{20,50}}
		},
		skill_attackradius={{{1,448},{20,480},{21,480}}},
		skill_cost_v={{{1,30},{20,40}}}
	},
	fuyun_sanxue={ --Phu van Tan tuyet
		colddamage_v={
			[1]={{1,40},{20,375}},
			[3]={{1,40},{20,575}}
		},
		physicsdamage_v={
			[1]={{1,10},{20,135}},--plus
			[3]={{1,10},{20,135}},--plus
		},
		seriesdamage_p={{{1,5},{20,30}}},
		addskilldamage1={
			[1]={{1,338},{2,338}},
			[3]={{1,1},{20,100}}
		},
		addskilldamage2={
			[1]={{1,1065},{2,1065}},
			[3]={{1,1},{20,83}}
		},
		addskilldamage3={
			[1]={{1,1093},{2,1093}},
			[3]={{1,1},{20,83}}
		},
		skill_attackradius={{{1,384},{20,416}}},
		skill_cost_v={{{1,50},{20,50}}}
	},
	bihai_chaosheng={ --Bich hai Trieu Sinh
		seriesdamage_p={{{1,10},{20,50},{21,52}}},
		physicsdamage_v={
			[1]={{1,20},{20,200}},
			[3]={{1,20},{20,200}},
		},
		colddamage_v={
			[1]={{1,43},{20,704}},
			[3]={{1,43},{20,1214}}
		},
		addskilldamage1={
			[1]={{1,337},{2,337}},
			[3]={{1,2},{20,70}}
		},
				--addskilldamage2={
				--	[1]={{1,338},{2,338}},
				--	[3]={{1,2},{20,65}}
				--},
		addskilldamage3={
			[1]={{1,1065},{2,1065}},
			[3]={{1,1},{20,58}}
		},
		addskilldamage4={
			[1]={{1,1093},{2,1093}},
			[3]={{1,1},{20,58}}
		},
		skill_cost_v={{{1,65},{20,65}}}
	},
	binggu_xuexin={ --Bang cot tuyet tam
		addcoldmagic_v={{{1,60},{30,315}},{{1,-1},{2,-1}}},
		addcolddamage_v={{{1,30},{30,275}},{{1,-1},{2,-1}}},
		addphysicsmagic_v={{{1,30},{30,275}},{{1,-1},{2,-1}}},
		deadlystrikeenhance_p={{{1,5},{30,45,Conic}},{{1,-1},{2,-1}}},
		coldenhance_p={{{1,8},{30,80}},{{1,-1},{2,-1}}}
	},
	bingzong_wuying={ --Bang Tung vo anh
		physicsenhance_p={{{1,15},{15,100},{20,146}}},
		seriesdamage_p={{{1,20},{15,20},{20,60},{21,62}}},
		colddamage_v={
			[1]={{1,10},{15,140},{20,173}},
			[3]={{1,50},{15,200},{20,276}}
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
			[1]={{1,1063},{2,1063}},
			[3]={{1,1},{20,120}}
		},
		addskilldamage2={
			[1]={{1,1064},{2,1064}},
			[3]={{1,1},{20,120}}
		},
		skill_showevent={{{1,0},{10,0},{10,4},{20,4}}},
		addskillexp1={{{1,336},{2,336}},{{1,EXP_PER},{20,EXP_PER}},{{1,0},{2,0}}},
		skill_misslenum_v={{{1,1},{5,1},{20,5},{21,5}}},
		skill_skillexp_v={{	{1,180},
							{2,460},
							{3,920},
							{4,1840},
							{5,2680},
							{6,5360},
							{7,7040},
							{8,9640},
							{9,11160},
							{10,12200},
							{11,23213},
							{12,38231},
							{13,57867},
							{14,79324},
							{15,124231},
							{16,186754},
							{17,278453},
							{18,397564},
							{19,457631},
							{20,789321},
							{21,1910800},
							{22,1910800}},
							{{1,336},{20,336}},
							{{1,191},{2,191}
						}},
	},
	bingxin_yuling={ --Bang tam Ngoc lang - ko su dung
		seriesdamage_p={{{1,20},{20,60},{21,62}}},
		physicsenhance_p={{{1,15},{20,115}}},
		colddamage_v={
			[1]={{1,10},{20,440}},
			[3]={{1,10},{20,440}}
		},
		skill_eventskilllevel={{{1,1},{20,20}}},
		skill_collideevent={
			[1]={{1,0},{15,0},{15,1},{20,1}},
			[3]={{1,382},{20,382}}
		},
		skill_showevent={{{1,0},{15,0},{15,4},{20,4}}},
	},
	bingxin_xuelian={ --Bang tam Tuyet lien
		seriesdamage_p={{{1,20},{20,60},{21,62}}},
		--physicsenhance_p={{{1,15},{20,115}}},
		colddamage_v={
			[1]={{1,10},{20,220}},
			[3]={{1,10},{20,220}}
		},
	},
	bingxin_xianzi={ --Bang Tam tien tu
		seriesdamage_p={{{1,20},{15,20},{20,60},{21,62}}},
		physicsdamage_v={
			[1]={{1,5},{15,100},{20,358}},
			[3]={{1,5},{15,100},{20,358}},
		},
		colddamage_v={
			[1]={{1,15},{15,240},{20,581}},
			[3]={{1,15},{15,240},{20,581}}
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
		addskilldamage1={
			[1]={{1,1065},{2,1065}},
			[3]={{1,1},{20,45}}
		},
		addskilldamage2={
			[1]={{1,1093},{2,1093}},
			[3]={{1,1},{20,45}}
		},
		skill_showevent={{{1,0},{10,0},{10,2},{20,2}}},
		addskillexp1={{{1,337},{2,337}},{{1,EXP_PER},{20,EXP_PER}},{{1,0},{2,0}}},
		skill_skillexp_v={{	{1,180},
							{2,460},
							{3,920},
							{4,1840},
							{5,2680},
							{6,5360},
							{7,7040},
							{8,9640},
							{9,11160},
							{10,12200},
							{11,23213},
							{12,38231},
							{13,57867},
							{14,79324},
							{15,124231},
							{16,186754},
							{17,278453},
							{18,397564},
							{19,457631},
							{20,789321},
							{21,1910800},
							{22,1910800}},
							{{1,337},{20,337}},
							{{1,192},{2,192}
						}},
	},
	fengxue_bingtian={ --Phong Tuyet Bang Thien
		seriesdamage_p={{{1,20},{20,60},{21,62}}},
		colddamage_v={
			[1]={{1,45},{20,400}},
			[3]={{1,45},{20,400}}
		},
	},
	cuiyan120={ --Ngu tuyet An 120
		skill_cost_v={{{1,35},{20,80},{21,80}}},
		hide={{{1,1},{20,1}},{{1,5*18},{15,25*18},{20,30*18},{21,30*18}}},
		skill_mintimepercast_v={{{1,60*18},{15,45*18},{20,40*18},{21,40*18}}},
		skill_mintimepercastonhorse_v={{{1,60*18},{15,45*18},{20,40*18},{21,40*18}}},
		skill_desc=
			function(level)
				return "Th阨 gian kh玦 ph鬰 chi猽 th鴆: <color=orange>"..floor(Link(level,SKILLS.cuiyan120.skill_mintimepercast_v[1]) / 18).." gi﹜<color>\n"
			end,
		skill_skillexp_v={{ {1,500},
							{2,3460},
							{3,5920},
							{4,8840},
							{5,14680},
							{6,25360},
							{7,37040},
							{8,49640},
							{9,71160},
							{10,112200},
							{11,146920},
							{12,186240},
							{13,227400},
							{14,380520},
							{15,552040},
							{16,764480},
							{17,978440},
							{18,1171040},
							{19,1520000},
							{20,789321},
							{21,1910800},
							{22,1910800}},
							{{1,0},{20,0}},
							{{1,194},{2,194}
						}},
	},
	daocuiyan150={ --Bang Tuoc Hoat ky -150
		physicsenhance_p={{{1,90},{15,600},{20,900},{23,1260},{26,1440}}},
		seriesdamage_p={{{1,40},{15,40},{20,80},{21,82}}},
		colddamage_v={
			[1]={{1,60},{15,850},{20,1050},{23,1290},{26,1410}},
			[3]={{1,300},{15,1200},{20,1655},{23,2201},{26,2474}}
		},
		missle_speed_v={{{1,24},{20,24},{21,24}}},
		skill_attackradius={{{1,448},{20,512},{21,512}}},
		skill_cost_v={{{1,48},{20,72},{23,79}}},
		skill_eventskilllevel={{{1,1},{20,20}}},
		skill_collideevent={
			[1]={{1,0},{10,0},{10,1},{20,1}},
			[3]={{1,1064},{20,1064}}
		},
		skill_showevent={{{1,0},{10,0},{10,4},{20,4}}},
		addskillexp1={{{1,1063},{2,1063}},{{1,EXP_PER},{20,EXP_PER}},{{1,0},{2,0}}},
		skill_skillexp_v={{	{1,180},
							{2,460},
							{3,920},
							{4,1840},
							{5,2680},
							{6,5360},
							{7,7040},
							{8,9640},
							{9,11160},
							{10,12200},
							{11,23213},
							{12,38231},
							{13,57867},
							{14,79324},
							{15,124231},
							{16,186754},
							{17,278453},
							{18,397564},
							{19,457631},
							{20,789321},
							{21,1910800},
							{22,1910800}},
							{{1,1063},{20,1063}},
							{{1,195},{2,195}
						}},
	},
	daocuiyan150_2={ --Bang Ngung Han Yen - 150 tang 2
		physicsenhance_p={{{1,18},{15,120},{20,175},{23,241},{26,274}}},
		seriesdamage_p={{{1,40},{15,40},{20,80},{21,82}}},
		colddamage_v={
			[1]={{1,12},{15,168},{20,210},{23,260},{26,285}},
			[3]={{1,60},{15,240},{20,331},{23,440},{26,494}}
		},
		missle_speed_v={{{1,28},{20,32},{21,32}}},--note
		skill_misslenum_v={{{1,1},{5,1},{20,5},{21,5}}},
	},
	neicuiyan150={ --Thuy Anh Man Tu -150
		seriesdamage_p={{{1,40},{15,40},{20,80},{21,82}}},
		physicsdamage_v={
			[1]={{1,6},{15,120},{20,430},{23,802},{26,988}},
			[3]={{1,6},{15,120},{20,430},{23,802},{26,988}},
		},
		colddamage_v={
			[1]={{1,18},{15,290},{20,700},{23,1192},{26,1438}},
			[3]={{1,18},{15,290},{20,700},{23,1192},{26,1438}}
		},
		missle_speed_v={{{1,20},{20,24},{21,24}}},
		skill_attackradius={{{1,448},{20,512},{21,512}}},
		skill_cost_v={{{1,55},{20,90},{23,101}}},
		skill_eventskilllevel={{{1,1},{20,20}}},
		skill_flyevent={
			[1]={{1,0},{10,0},{10,1},{20,1}},
			[2]={{1,2},{2,2}},
			[3]={{1,1093},{20,1093}}
		},
		skill_showevent={{{1,0},{10,0},{10,2},{20,2}}},
		addskillexp1={{{1,1065},{2,1065}},{{1,EXP_PER},{20,EXP_PER}},{{1,0},{2,0}}},
		skill_skillexp_v={{	{1,180},
							{2,460},
							{3,920},
							{4,1840},
							{5,2680},
							{6,5360},
							{7,7040},
							{8,9640},
							{9,11160},
							{10,12200},
							{11,23213},
							{12,38231},
							{13,57867},
							{14,79324},
							{15,124231},
							{16,186754},
							{17,278453},
							{18,397564},
							{19,457631},
							{20,789321},
							{21,1910800},
							{22,1910800}},
							{{1,1065},{20,1065}},
							{{1,196},{2,196}
						}},
	},
	--startevent: 1102 missle:362 neicuiyan150
	neicuiyan150_2={ --Khinh Sa Phu Thuy -150 tang 2
		seriesdamage_p={{{1,40},{20,80},{21,82}}},
		colddamage_v={
			[1]={{1,55},{20,720},{23,930},{26,1035}},
			[3]={{1,55},{20,720},{23,930},{26,1035}}
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

