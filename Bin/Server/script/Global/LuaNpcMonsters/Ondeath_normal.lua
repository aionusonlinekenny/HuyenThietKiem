-- Author: Kinnox
-- Date: 25-08-2021
-- Chøc n¨ng: Droprate NPC (Qu¸i th­êng / Boss xanh / Boss hoµng kim)

Include("\\script\\lib\\tasklib.lua");
Include("\\script\\lib\\libevent.lua");

-- ========================= CAÙU HÌNH =========================
DROP_RATE_NORMAL   = 50   -- % c¬ héi qu¸i th­êng r¬i item
BOSS_BLUE_DROP_CNT = 3    -- Boss xanh r¬i thªm X item tõ droprate
EXPBoss            = 3    -- x3 kinh nghiÖm boss xanh

-- B¶ng §Þnh nghÜa droprate theo bËc
DROP_CONFIG = {
    [1]  = { file="\\settings\\droprate\\npcdroprate10.ini",  exp=90*EXPBoss,   moneyN={30,32},  moneyB={100,132} },
    [2]  = { file="\\settings\\droprate\\npcdroprate20.ini",  exp=1300*EXPBoss, moneyN={50,52},  moneyB={100,232} },
    [3]  = { file="\\settings\\droprate\\npcdroprate30.ini",  exp=1400*EXPBoss, moneyN={60,82},  moneyB={200,332} },
    [4]  = { file="\\settings\\droprate\\npcdroprate40.ini",  exp=1500*EXPBoss, moneyN={80,102}, moneyB={200,432} },
    [5]  = { file="\\settings\\droprate\\npcdroprate50.ini",  exp=2000*EXPBoss, moneyN={90,202}, moneyB={300,532} },
    [6]  = { file="\\settings\\droprate\\npcdroprate60.ini",  exp=2000*EXPBoss, moneyN={100,252},moneyB={300,432} },
    [7]  = { file="\\settings\\droprate\\npcdroprate70.ini",  exp=2000*EXPBoss, moneyN={110,272},moneyB={300,532} },
    [8]  = { file="\\settings\\droprate\\npcdroprate80.ini",  exp=2000*EXPBoss, moneyN={210,282},moneyB={300,632} },
    [9]  = { file="\\settings\\droprate\\npcdroprate90.ini",  exp=3000*EXPBoss, moneyN={301,302},moneyB={301,332} },
    [10] = { file="\\settings\\droprate\\npcdroprate110.ini", exp=5000*EXPBoss, moneyN={411,502},moneyB={411,532} },
}

-- ========================= Xö lý chÕt NPC =========================
function OnDeath(nNpcIndex,nLastdamage)
    local nPlayerIndex = NpcIdx2PIdx(nLastdamage)
    PlayerIndex = nPlayerIndex

    local nX,nY,nW = GetNpcPos(nNpcIndex)
    local nSeries  = GetNpcSeries(nNpcIndex)
    local nLevelNpc= GetNpcLevel(nNpcIndex)
    local nGoldNpc = GetNpcGold(nNpcIndex)

    if (nLevelNpc <= 0) then return end
    if (nSeries < 0 or nSeries > 4) then return end

    -- TÝnh bËc tõ level NPC
    local nBac = (nLevelNpc >= 100) and 10 or floor(nLevelNpc/10)
    if nBac < 1 then nBac = 1 end
    if nBac > 10 then nBac = 10 end

    local cfg = DROP_CONFIG[nBac]
    if not cfg then return end

    -- LÊy file droprate vµ gi¸ trÞ EXP/TiÒn
    local nDropItem = cfg.file
    local nExp = cfg.exp
    local nPriceNormal = random(cfg.moneyN[1], cfg.moneyN[2])
    local nPriceBossBlue = random(cfg.moneyB[1], cfg.moneyB[2])

    -- Qu¸i th­êng cã c¬ héi r¬i item tõ droprate
    if random(1,100) <= DROP_RATE_NORMAL then
        DropRateItem(nNpcIndex, 1, nDropItem, 1, 0, nSeries)
    end

    -- ================= Boss xanh =================
    if (nGoldNpc >= 14 and nGoldNpc <= 15) then
        AddOwnExp(nExp)
        for i=1,random(2,3) do DropMoney(nPriceBossBlue,nX,nY) end
        -- Boss xanh r¬i thªm item tõ droprate
        for i=1,BOSS_BLUE_DROP_CNT do
            DropRateItem(nNpcIndex,1,nDropItem,1,0,nSeries)
        end
        -- R¬i m¶nh chØ hoÆc lÖnh bµi
        if random(1,200)==151 then
            DropItem(nW,nX,nY,PlayerIndex,6,random(16,18),1,1,-1,0,0)
        end
        if (nW==336 and random(1,10)==8) then
            DropItem(nW,nX,nY,PlayerIndex,6,12,1,1,-1,0,0)
        end

    -- ================= Boss Hoµng Kim =================
    elseif (nGoldNpc == 2) then
        EventBossHoangKim(nNpcIndex,nW,nX,nY,PlayerIndex,nSeries)

    -- ================= Qu¸i th­êng kh¸c =================
    else
        if random(1,15)==10 then DropMoney(nPriceNormal,nX,nY) end
        if random(1,4)==3 then EventQuocKhanh() end
        if random(1,100)==56 then
            local nIndexItem = DropItem(nW,nX,nY,PlayerIndex,7,25,1,1,-1,0,0)
			if nIndexItem > 0 then
				SetObjPickExecute(nIndexItem,1)
			end
        end
        if random(1,135)==122 then
		DropRateItem(nNpcIndex, 1, nDropItem, 1, nBac, nSeries)
		end
        if (nW==336 and random(1,35)==20) then
            DropItem(nW,nX,nY,PlayerIndex,6,12,1,1,-1,0,0)
        end
    end

    -- ================= Drop tiÒn §ång =================
    SetTask(T_DROPTIENDONG,GetTask(T_DROPTIENDONG)+1)
    if (nBac>7 and GetTask(T_DROPTIENDONG)>=1000) then
        if random(1,3)==3 then
            DropItem(nW,nX,nY,PlayerIndex,6,19,1,1,-1,0,0)
            DropItem(nW,nX,nY,PlayerIndex,6,19,1,1,-1,0,0)
        else
            DropItem(nW,nX,nY,PlayerIndex,6,19,1,1,-1,0,0)
        end
        SetTask(T_DROPTIENDONG,0)
    end

    -- ================= Liªn quan nhiÖm vô =================
    local nW2,nX2,nY2 = GetWorldPos()
    if(GetTask(TASK_KIND)==3 and GetTask(TASK_SAVEQUEST_1)==nW2) then
        if (GetTask(TASK_DT_DANHQUAI)>0) then
            SetTask(TASK_DT_DANHQUAI,GetTask(TASK_DT_DANHQUAI)-1)
            Msg2Player("NhiÖm vô d· tÈu","<color=gold>TiÕn ®é: "..GetTask(TASK_DT_DANHQUAI))
        end
    end
end

function OnRevive(nNpcIndex)
	local nRandom = random(1,100);
	if (nRandom >= 98 or nRandom <= 5) then
		IsBoss(nNpcIndex,random(13,15));
	end;
	SetNpcScript(nNpcIndex, "\\script\\global\\luanpcmonsters\\ondeath_normal.lua");
end;