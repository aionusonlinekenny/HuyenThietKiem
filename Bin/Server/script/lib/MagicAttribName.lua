-- Magic Attribute ID to Vietnamese Name Mapping
-- Based on KMagicAttrib.h and MagicDesc.ini

MagicAttribName = {
    -- Item attributes
    [28] = "S¸t th­¬ng vò khÝ (min)",
    [29] = "S¸t th­¬ng vò khÝ (max)",
    [30] = "NÐ tr¸nh",
    [31] = "§é bÒn",
    [32] = "Yªu cÇu Søc m¹nh",
    [33] = "Yªu cÇu Th©n ph¸p",
    [34] = "Yªu cÇu Sinh khÝ",
    [35] = "Yªu cÇu Néi c«ng",
    [36] = "Yªu cÇu §¼ng cÊp",
    [40] = "T¨ng s¸t th­¬ng vò khÝ %",
    [41] = "T¨ng nÐ tr¸nh %",
    [42] = "Gi¶m yªu cÇu %",

    -- Damage attributes
    [56] = "§é chÝnh x¸c",
    [57] = "§é chÝnh x¸c %",
    [58] = "Bá qua nÐ tr¸nh %",
    [59] = "S¸t th­¬ng vËt lý",
    [60] = "B¨ng s¸t",
    [61] = "Hãa s¸t",
    [62] = "L«i s¸t",
    [63] = "§éc s¸t",
    [64] = "S¸t th­¬ng ngò hµnh",
    [65] = "T¨ng s¸t th­¬ng vËt lý %",
    [66] = "Hót sinh lùc %",
    [67] = "Hót néi lùc %",
    [68] = "Hót thÓ lùc %",
    [69] = "§Èy lïi %",
    [70] = "ChÝ m¹ng %",
    [71] = "ChÝ tö %",
    [72] = "Lµm cho¸ng %",
    [73] = "Ngò hµnh t­¬ng kh¾c %",

    -- Normal attributes
    [85]  = "Sinh lùc tèi ®a",
    [86]  = "Sinh lùc tèi ®a %",
    [87]  = "Sinh lùc",
    [88]  = "Håi sinh lùc",
    [89]  = "Néi lùc tèi ®a",
    [90]  = "Néi lùc tèi ®a %",
    [91]  = "Néi lùc",
    [92]  = "Håi néi lùc",
    [93]  = "ThÓ lùc tèi ®a",
    [94]  = "ThÓ lùc tèi ®a %",
    [95]  = "ThÓ lùc",
    [96]  = "Håi thÓ lùc",
    [97]  = "Søc m¹nh",
    [98]  = "Th©n ph¸p",
    [99]  = "Sinh khÝ",
    [100] = "Néi c«ng",
    [101] = "Kh¸ng ®éc %",
    [102] = "Kh¸ng hãa %",
    [103] = "Kh¸ng l«i %",
    [104] = "Kh¸ng vËt lý %",
    [105] = "Kh¸ng b¨ng %",
    [106] = "Gi¶m thêi gian b¨ng %",
    [107] = "Gi¶m thêi gian ch¸y %",
    [108] = "Gi¶m thêi gian ®éc %",
    [109] = "Gi¶m s¸t th­¬ng ®éc",
    [110] = "Gi¶m thêi gian cho¸ng %",
    [111] = "Tèc ®é ch¹y %",
    [112] = "TÇm nh×n %",
    [113] = "Håi phôc nhanh",
    [114] = "Kh¸ng tÊt c¶ %",
    [115] = "Tèc ®é ®¸nh",
    [116] = "Tèc ®é xuÊt chiªu",
    [117] = "Ph¶n s¸t th­¬ng cËn",
    [118] = "Ph¶n s¸t th­¬ng cËn %",
    [119] = "Ph¶n s¸t th­¬ng xa",
    [120] = "Ph¶n s¸t th­¬ng xa %",
    [121] = "S¸t th­¬ng vËt lý +",
    [122] = "Hãa s¸t +",
    [123] = "B¨ng s¸t +",
    [124] = "L«i s¸t +",
    [125] = "§éc s¸t +",
    [126] = "S¸t th­¬ng vËt lý % +",
    [129] = "Gi¸p vËt lý",
    [130] = "Gi¸p b¨ng",
    [131] = "Gi¸p hãa",
    [132] = "Gi¸p ®éc",
    [133] = "Gi¸p l«i",
    [135] = "May m¾n",
}

-- Get attribute name by ID, returns "Unknown" if not found
function GetMagicAttribName(nAttribType)
    if MagicAttribName[nAttribType] then
        return MagicAttribName[nAttribType]
    else
        return "Thuéc tÝnh #" .. nAttribType
    end
end
