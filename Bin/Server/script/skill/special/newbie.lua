--µü´úº¯Êý£¬ÓÃÓÚ¼ÆËã¼¼ÄÜÊìÁ·¶È
--¾ßÌå·½·¨£º
--¸ù¾Ý1¼¶ÊìÁ·¶È£¬Éý¼¶¼ÓËÙ¶È£¬¼¶Êý£¬ÖØ¸´ÉËº¦´ÎÊý£¬·¶Î§£¬¼ÆËã³öÏàÓ¦µÈ¼¶ÊìÁ·¶È
-- SkillExp(i) = Exp1*a^(i-1)*time*range
EXP_PER = 60
function SkillExpFunc(Exp0,a,Level,Time,Range)
	return floor(Exp0*(a^(Level-1))*Time*Range/2)
end

SKILLS={
	--¶ëáÒ
	piaoyun_chuanxue={ --Phieu tuyet xuyen van
		seriesdamage_p={{{1,1},{20,10}}},
		colddamage_v={
			[1]={{1,15},{20,275}},
			[3]={{1,25},{20,415}}
		},
		addskilldamage2={
			[1]={{1,380},{2,380}},
			[3]={{1,1},{20,75}}
		},
		addskilldamage1={
			[1]={{1,91},{2,91}},
			[3]={{1,1},{20,35}}
		},
		addskilldamage3={
			[1]={{1,1062},{2,1062}},
			[3]={{1,1},{20,63}}
		},
		missle_speed_v={{{1,20},{20,24},{21,24}}},
		skill_attackradius={{{1,320},{20,384}}},
		skill_cost_v={{{1,10},{20,10}}}
	},
	emei_jianfa={ --Nga Mi Kiem Phap
		addphysicsdamage_p={{{1,15},{20,215}},{{1,-1},{2,-1}},{{1,0},{2,0}}},
		deadlystrikeenhance_p={{{1,6},{20,36}},{{1,-1},{2,-1}}}
	},
	emei_zhangfa={ --Nga Mi Chuong phap
		addcoldmagic_v={{{1,15},{20,515}},{{1,-1},{2,-1}}}
	},
	sixiang_tonggui={ --Tu tuong dong quy
		seriesdamage_p={{{1,5},{20,30}}},
		colddamage_v={
			[1]={{1,35},{20,315}},
			[3]={{1,45},{20,450}}
		},
		addskilldamage1={
			[1]={{1,331},{2,331}},
			[3]={{1,1},{20,76}}
		},
		addskilldamage2={
			[1]={{1,1062},{2,1062}},
			[3]={{1,1},{20,20}}
		},
		skill_attackradius={{{1,384},{20,416}}},
		skill_cost_v={{{1,25},{20,35}}}
	},
	yiye_zhiqiu={ --Nhat diep Tri thu
		seriesdamage_p={{{1,1},{20,10}}},
		physicsenhance_p={{{1,30},{20,75}}},
		colddamage_v={
			[1]={{1,10},{20,80}},
			[3]={{1,10},{20,80}}
		},
		deadlystrike_p={{{1,10},{20,20}}},
		addskilldamage1={
			[1]={{1,328},{2,328}},
			[3]={{1,1},{20,60}}
		},
		addskilldamage2={
			[1]={{1,88},{2,88}},
			[3]={{1,1},{20,35}}
		},
		addskilldamage3={
			[1]={{1,1061},{2,1061}},
			[3]={{1,1},{20,50}}
		},
		addskilldamage4={
			[1]={{1,1091},{2,1091}},
			[3]={{1,50},{20,50}}
		},
		missle_speed_v={{{1,20},{20,24},{21,24}}},
		skill_attackradius={{{1,320},{20,384}}},
		skill_cost_v={{{1,25},{20,25}}}
	},
	liushui={--Luu Thuy
		fastwalkrun_p={{{1,9},{20,66}},{{1,18},{2,18}}}
	},
	bumie_bujue={ --Bat Diet Bat tuyet
		physicsenhance_p={{{1,80},{20,385}}},
		seriesdamage_p={{{1,10},{20,50},{21,52}}},
		colddamage_v={
			[1]={{1,10},{20,282}},
			[3]={{1,10},{20,282}}
		},
		deadlystrike_p={{{1,15},{20,54}}},
		addskilldamage1={
			[1]={{1,328},{2,328}},
			[3]={{1,1},{20,70}}
		},
		addskilldamage2={
			[1]={{1,1061},{2,1061}},
			[3]={{1,1},{20,60}}
		},
		missle_speed_v={{{1,28},{20,32},{21,32}}},
		skill_attackradius={{{1,448},{20,512},{21,512}}},
		skill_cost_v={{{1,30},{20,35}}}
	},
	mengdie={ --Mong diep
		lifereplenish_v={{{1,1},{20,50}},{{1,18},{2,18}}},
		manareplenish_v={{{1,1},{20,50}},{{1,18},{2,18}}},
	},
	foguang_puzhao={ --Phat Quang pho chieu
		colddamage_v={
			[1]={{1,70},{20,787}},
			[3]={{1,80},{20,1287}}
		},
		seriesdamage_p={{{1,10},{20,50},{21,52}}},
		addskilldamage2={
			[1]={{1,380},{2,380}},
			[3]={{1,1},{20,80}}
		},
		addskilldamage3={
			[1]={{1,1062},{2,1062}},
			[3]={{1,1},{20,65}}
		},
		skill_cost_v={{{1,30},{20,60}}}
	},
	cihang_pudu={ --Tu hang pho do
		lifereplenish_v={{{1,275},{20,750}},{{1,36},{2,36}}},
		skill_cost_v={{{1,100},{20,100}}}
	},
	fofa_wubian={ --Phat Phap vo bien
		addcoldmagic_v={{{1,30},{30,315}},{{1,-1},{2,-1}}},
		addcolddamage_v={{{1,70},{30,315}},{{1,-1},{2,-1}}},
		attackspeed_v={{{1,12},{30,65}},{{1,-1},{2,-1}}},
		castspeed_v={{{1,12},{30,65}},{{1,-1},{2,-1}}},
		coldenhance_p={{{1,8},{30,37}},{{1,-1},{2,-1}}}
	},
	foxin_ciyou={ --Phat tam Tu Huu
		lifemax_p={{{1,30},{20,125}},{{1,18*8},{2,18*8}}},
	},
	tuichuang_wangyue={ --Thoi song Vong nguyet
		physicsenhance_p={{{1,40},{20,175}}},
		seriesdamage_p={{{1,5},{20,30}}},
		colddamage_v={
			[1]={{1,10},{20,120}},
			[3]={{1,10},{20,120}}
		},
		deadlystrike_p={{{1,10},{20,30}}},
		addskilldamage1={
			[1]={{1,329},{2,329}},
			[3]={{1,1},{20,49}}
		},
		addskilldamage2={
			[1]={{1,1091},{2,1091}},
			[3]={{1,1},{20,40}}
		},
		missle_speed_v={{{1,24},{20,28},{21,28}}},
		skill_attackradius={{{1,384},{20,448}}},
		skill_cost_v={{{1,20},{20,20}}}
	},
	qingyin_fanchang={ --Thanh Am phan xuong
		fasthitrecover_v={{{1,1},{20,20}},{{1,18},{2,18}}},
		fatallystrikeres_p={{{1,1},{20,20}},{{1,18},{2,18}}},
		freezetimereduce_p={{{1,1},{20,30}},{{1,18},{2,18}}},
		poisontimereduce_p={{{1,1},{20,30}},{{1,18},{2,18}}},
		stuntimereduce_p={{{1,1},{20,30}},{{1,18},{2,18}}}
	},
	pudu_zhongsheng={ --Pho Do chung sinh
		lifereplenish_v={{{1,1},{20,15}},{{1,18},{2,18}}},
		manareplenish_v={{{1,1},{20,10}},{{1,18},{2,18}}},
		allres_p={{{1,1},{20,40}},{{1,18},{2,18}}},
	},
	sane_jixue={ --Tam Nga Te tuyet
		physicsenhance_p={{{1,10},{15,100},{20,237}}},
		seriesdamage_p={{{1,20},{15,20},{20,60},{21,62}}},
		colddamage_v={
			[1]={{1,10},{20,111}},
			[3]={{1,10},{20,111}}
		},
		deadlystrike_p={{{1,10},{20,54}}},
		missle_speed_v={{{1,28},{20,32},{21,32}}},
		skill_attackradius={{{1,448},{20,512},{21,512}}},
		skill_cost_v={{{1,35},{20,35}}},
		skill_eventskilllevel={{{1,1},{20,20}}},
		skill_startevent={
			[1]={{1,0},{10,0},{10,1},{20,1}},
			[3]={{1,329},{20,329}}
		},
		addskilldamage1={
			[1]={{1,1061},{2,1061}},
			[3]={{1,1},{20,25}}
		},
		skill_showevent={{{1,0},{10,0},{10,1},{20,1}}},
		addskillexp1={{{1,328},{2,328}},{{1,EXP_PER},{20,EXP_PER}},{{1,0},{2,0}}},
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
							{{1,328},{20,328}},
							{{1,191},{2,191}
						}},
	},
	yuquan_xichen={ --Ngoc Tuyen Tay tran
		physicsenhance_p={{{1,30},{20,148}}},
		seriesdamage_p={{{1,20},{20,60},{21,62}}},
		deadlystrike_p={{{1,10},{20,20}}},
	},
	qianfo_qianye={ --Thien Phat Thien Diep - ko su dung
		colddamage_v={
			[1]={{1,45},{20,100}},
			[3]={{1,45},{20,100}}
		},
		seriesdamage_p={{{1,20},{20,60},{21,62}}},
		skill_attackradius={{{1,448},{20,512}}},
		skill_cost_v={{{1,30},{20,65}}},
		skill_eventskilllevel={{{1,1},{20,20}}},
		skill_startevent={
			[1]={{1,0},{10,0},{10,1},{20,1}},
			[3]={{1,380},{20,380}}
		},
		skill_showevent={{{1,0},{10,0},{10,1},{20,1}}},
		addskillexp1={{{1,0},{2,0}},{{1,EXP_PER},{20,EXP_PER}},{{1,0},{2,0}}},
		skill_skillexp_v={{{1,20000},{20,100000000,Conic}}},
	},
	fengshuang_suiying={ --Phong Suong toai anh
		seriesdamage_p={{{1,20},{15,20},{20,60},{21,62}}},
		colddamage_v={
			[1]={{1,20},{15,350},{20,770}},
			[3]={{1,30},{15,400},{20,1000}}
		},
		skill_eventskilllevel={{{1,1},{20,20}}},
		skill_cost_v={{{1,30},{20,65}}},
		skill_startevent={
			[1]={{1,0},{10,0},{10,1},{20,1}},
			[3]={{1,331},{20,331}}
		},
		addskilldamage1={
			[1]={{1,1062},{2,1062}},
			[3]={{1,1},{20,25}}
		},
		skill_showevent={{{1,0},{10,0},{10,1},{20,1}}},
		addskillexp1={{{1,380},{2,380}},{{1,EXP_PER},{20,EXP_PER}},{{1,0},{2,0}}},
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
							{{1,380},{20,380}},
							{{1,192},{2,192}
						}},
	},
	jinding_foguang={ --Kim Dinh Phat Quang
		colddamage_v={
			[1]={{1,10},{20,585},{21,600}},
			[3]={{1,10},{20,585},{21,600}},
		},
		seriesdamage_p={{{1,20},{20,60},{21,62}}},
		missle_speed_v={{{1,24},{20,28},{21,28}}},
		skill_misslenum_v={{{1,1},{10,1},{20,3},{21,3}}},
	},
	emei120={ --Be Nguyet Phat Tran 120
		skill_appendskill={{{1,86},{20,86}},{{1,18},{20,18}}},
		skill_desc=
			function(level)
				return "§¼ng cÊp kü n¨ng nµy sÏ lÊy theo kü n¨ng hç trî cã cÊp thÊp nhÊt \n"
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
	emei120_1={ --¶ëáÒ120¼¶¼¼ÄÜ
		skill_appendskill={{{1,89},{20,89}},{{1,18},{20,18}}},
	},
	emei120_2={ --¶ëáÒ120¼¶¼¼ÄÜ
		skill_appendskill={{{1,92},{20,92}},{{1,18},{20,18}}},
	},
	emei120_3={ --¶ëáÒ120¼¶¼¼ÄÜ
		skill_appendskill={{{1,282},{20,282}},{{1,18},{20,18}}},
	},
	emei120_4={ --¶ëáÒ120¼¶¼¼ÄÜ
		skill_appendskill={{{1,332},{20,332}},{{1,18},{20,18}}},
	},
	jianemei150={ --Kiem Hoa Van Tinh -150
		physicsenhance_p={{{1,12},{15,120},{20,285},{23,483},{26,582}}},
		seriesdamage_p={{{1,40},{15,40},{20,80},{21,82}}},
		colddamage_v={
			[1]={{1,20},{20,195},{23,250},{26,277}},
			[3]={{1,20},{20,195},{23,250},{26,277}}
		},
		deadlystrike_p={{{1,12},{20,65},{23,81},{26,90}}},
		missle_speed_v={{{1,36},{20,36},{21,36}}},
		skill_attackradius={{{1,448},{20,512},{21,512}}},
		skill_cost_v={{{1,45},{20,45}}},
		skill_eventskilllevel={{{1,1},{20,20}}},
		skill_startevent={
			[1]={{1,0},{10,0},{10,1},{20,1}},
			[3]={{1,1089},{20,1089}}
		},
		skill_showevent={{{1,0},{10,0},{10,1},{20,1}}},
		addskillexp1={{{1,1061},{2,1061}},{{1,EXP_PER},{20,EXP_PER}},{{1,0},{2,0}}},
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
							{{1,1061},{20,1061}},
							{{1,195},{2,195}
						}},
	},
	jianemei150_2={ --Kiem Tam Tinh Dien - 150 tang 2
		physicsenhance_p={{{1,36},{20,175},{23,218},{26,240}}},
		seriesdamage_p={{{1,40},{20,80},{21,82}}},
		deadlystrike_p={{{1,12},{20,24},{23,27}}},
		colddamage_v={
			[1]={{1,10},{20,110},{23,141},{26,157}},
			[3]={{1,10},{20,110},{23,141},{26,157}}
		},
		--vanishedevent: 1091 missle: 353 jianemei150_2
	},
	zhangemei150={ --Bang Vu Lac Tinh - 150 - hoi yeu' can xem lai.
		seriesdamage_p={{{1,40},{15,40},{20,80},{21,82}}},
		colddamage_v={
			[1]={{1,24},{15,420},{20,930},{23,1542},{26,1848}},
			[3]={{1,36},{15,480},{20,1200},{23,2064},{26,2496}}
		},
		skill_cost_v={{{1,36},{20,78},{23,91}}},
		addskillexp1={{{1,1062},{2,1062}},{{1,EXP_PER},{20,EXP_PER}},{{1,0},{2,0}}},
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
							{{1,1062},{20,1062}},
							{{1,196},{2,196}
						}},
	},
	zhangemei150_2={ --tang 2- ko su dung
		colddamage_v={
			[1]={{1,10},{20,585},{21,600}},
			[3]={{1,10},{20,585},{21,600}},
		},
		seriesdamage_p={{{1,20},{20,60},{21,62}}},
		missle_speed_v={{{1,24},{20,28},{21,28}}},
		skill_misslenum_v={{{1,1},{10,1},{20,3},{21,3}}},
	},
}
-----------------------------------------------
--Create by yfeng 2004-05-20
-----------------------------------------------

-----------------------------------------------
--¸ù¾Ý2¸öµã£¬ÇóÏßÐÎº¯Êýf(x)=k*x+b
--y= (y2-y1)*(x-x1)/(x2-x1)+y1
--µ±x2=x1, ÓÐx=c,¸ÃÖ±ÏßÊÇÒ»Ìõ´¹Ö±ÓÚxÖáµÄÖ±Ïß
--ÕâÊÇ¿ÉÒÔÈ¡µÃy=ÈÎÒâÖµ
--Òò´Ë£¬Èç¹ûÒÑÖªÁ½µã(x1,y1),(x2,y2)¿ÉÇóµÃ¹ý´Ë2µãµÄ
--º¯ÊýÎª£º
function Line(x,x1,y1,x2,y2)
	if(x2==x1) then
		return y2
	end
	return (y2-y1)*(x-x1)/(x2-x1)+y1
end

-----------------------------------------------
--¸ù¾Ý2¸öµã£¬Çó2´ÎÐÎº¯Êýf(x)=a*x2+c
--y= (y2-y1)*x*x/(x2*x2-x1*x1)-(y2-y1)*x1*x1/(x2*x2-x1*x1)+y1
--µ±x1»òÕßx2 < 0 ,y =0
--µ±x2=x1, ÓÐx=c,ÊÇÒ»Ìõ´¹Ö±ÓÚxÖáµÄÖ±Ïß
--ÕâÊÇ¿ÉÒÔÈ¡µÃy=ÈÎÒâÖµ
--Òò´Ë£¬Èç¹ûÒÑÖªÁ½µã(x1,y1),(x2,y2)¿ÉÇóµÃ¹ý´Ë2µãµÄ
--º¯ÊýÎª£ºextrac
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
--¸ù¾Ý2¸öµã£¬Çó-2´ÎÐÎº¯Êýf(x)=a*sqrt(x2)+c
--y=(y2-y1)*x/(sqrt(x2)-sqrt(x1))+y1-(y2-y1)/((sqrt(x2)-sqrt(x1))
--µ±x2»òÕßx1<0, y=0,
--µ±x1=x2,ÓÐx=c,ÊÇÒ»Ìõ´¹Ö±ÓÚxÖáµÄÖ±Ïß
--ÕâÊÇ¿ÉÒÔÈ¡µÃy=ÈÎÒâÖµ
--Òò´Ë£¬Èç¹ûÒÑÖªÁ½µã(x1,y1),(x2,y2)¿ÉÇóµÃ¹ý´Ë2µãµÄ
--º¯ÊýÎª£ºextrac
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
--Ãè»æÁ¬½ÓÏß:Link(x,points)
--¸ù¾ÝpointsÌá¹©µÄÒ»ÏµÁÐµã£¬ÓÃÏàÁÚµÄÁ½¸öµãÃè»æÇúÏß
--return yÖµ
--x ÊäÈëÖµ
--points µã¼¯ºÏ
--ÐÎÈç£ºpointsÊÇÐÎÈç{{x1,y1,func=xxx},{x2,y2,func=xxx},...{xn,yn,func=xxx}}µÄÓ³Éä
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
--¼¼ÄÜÉè¶¨¸ñÊ½ÈçÏÂ£º
--SKILLS={
--	¼¼ÄÜÃû³Æ=	{
--		Ä§·¨ÊôÐÔ=	{
--			[1]={{¼¶±ð,ÊýÖµ£¬ÇúÏß}£¬{¼¶±ð£¬ÊýÖµ£¬ÇúÏß}£¬¡£¡£¡£¡£}£¬
--			[2]={{¼¶±ð,ÊýÖµ£¬ÇúÏß}£¬{¼¶±ð£¬ÊýÖµ£¬ÇúÏß}£¬¡£¡£¡£¡£}£¬
--			[3]={{¼¶±ð,ÊýÖµ£¬ÇúÏß}£¬{¼¶±ð£¬ÊýÖµ£¬ÇúÏß}£¬¡£¡£¡£¡£}£¬	
--		}£¬
--		Ä§·¨ÊôÐÔ=	{
--			[1]={{¼¶±ð,ÊýÖµ£¬ÇúÏß}£¬{¼¶±ð£¬ÊýÖµ£¬ÇúÏß}£¬¡£¡£¡£¡£}£¬
--			[2]={{¼¶±ð,ÊýÖµ£¬ÇúÏß}£¬{¼¶±ð£¬ÊýÖµ£¬ÇúÏß}£¬¡£¡£¡£¡£}£¬
--			[3]={{¼¶±ð,ÊýÖµ£¬ÇúÏß}£¬{¼¶±ð£¬ÊýÖµ£¬ÇúÏß}£¬¡£¡£¡£¡£}£¬	
--		}£¬
--		¡£¡£¡£¡£¡£
--	}£¬
--	¼¼ÄÜÃû³Æ=	{
--		Ä§·¨ÊôÐÔ=	{
--			[1]={{¼¶±ð,ÊýÖµ£¬ÇúÏß}£¬{¼¶±ð£¬ÊýÖµ£¬ÇúÏß}£¬¡£¡£¡£¡£}£¬
--			[2]={{¼¶±ð,ÊýÖµ£¬ÇúÏß}£¬{¼¶±ð£¬ÊýÖµ£¬ÇúÏß}£¬¡£¡£¡£¡£}£¬
--			[3]={{¼¶±ð,ÊýÖµ£¬ÇúÏß}£¬{¼¶±ð£¬ÊýÖµ£¬ÇúÏß}£¬¡£¡£¡£¡£}£¬	
--		}£¬
--		Ä§·¨ÊôÐÔ=	{
--			[1]={{¼¶±ð,ÊýÖµ£¬ÇúÏß}£¬{¼¶±ð£¬ÊýÖµ£¬ÇúÏß}£¬¡£¡£¡£¡£}£¬
--			[2]={{¼¶±ð,ÊýÖµ£¬ÇúÏß}£¬{¼¶±ð£¬ÊýÖµ£¬ÇúÏß}£¬¡£¡£¡£¡£}£¬
--			[3]={{¼¶±ð,ÊýÖµ£¬ÇúÏß}£¬{¼¶±ð£¬ÊýÖµ£¬ÇúÏß}£¬¡£¡£¡£¡£}£¬	
--		}£¬
--		¡£¡£¡£¡£¡£
--	}£¬
--	¡£¡£¡£¡£¡£
--}
--Èç£º
--SKILLS={
--	Sanhuan-taoyue={
--		physicsenhance_p={
--			[1]={{1,50},{20,335}},--Ä§·¨ÊôÐÔphysicsenhance_p²ÎÊý1£¬1¼¶Ê±Îª35£¬20¼¶Ê±Îª335£¬ÇúÏß²»Ìî£¬Ä¬ÈÏÏßÐÎ
--			[2]={{1,0},{20,0}},
--		},--Ã»ÓÐ[3]£¬±íÊ¾Ä§·¨ÊôÐÔphysicsenhance_p²ÎÊý2£¬Ä¬ÈÏÎªÈÎºÎÊ±ºò¶¼ÊÇ0
--		lightingdamage_v={
--			[1]={{1,65},{20,350}},
--			[3]={{1,65},{20,350}},
--		}
--	}
--}
--ÒÔÉÏÃèÊö¼¼ÄÜ¡°Èý»·Ì×ÔÂ¡±µÄÄ§·¨ÊôÐÔºÍÊýÖµ
-----------------------------------------------------------
--º¯ÊýGetSkillLevelData(levelname, data, level)
--levelname£ºÄ§·¨ÊôÐÔÃû³Æ
--data£º¼¼ÄÜÃû³Æ
--level£º¼¼ÄÜµÈ¼¶
--return£ºµ±¼¼ÄÜÃû³ÆÎªdata£¬¼¼ÄÜµÈ¼¶Îªlevel
--			Ê±µÄÄ§·¨ÊôÐÔlevelnameËùÐèÇóµÄÈý¸ö²ÎÊýµÄ¾ßÌåÖµ
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

