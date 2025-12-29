-- Magic Attribute ID to Vietnamese Name Mapping
-- Based on KMagicAttrib.h and MagicDesc.ini

MagicAttribName = {
    -- Item attributes
    [28] = "Sát thương vũ khí (min)",
    [29] = "Sát thương vũ khí (max)",
    [30] = "Né tránh",
    [31] = "Độ bền",
    [32] = "Yêu cầu Sức mạnh",
    [33] = "Yêu cầu Thân pháp",
    [34] = "Yêu cầu Sinh khí",
    [35] = "Yêu cầu Nội công",
    [36] = "Yêu cầu Đẳng cấp",
    [40] = "Tăng sát thương vũ khí %",
    [41] = "Tăng né tránh %",
    [42] = "Giảm yêu cầu %",

    -- Damage attributes
    [56] = "Độ chính xác",
    [57] = "Độ chính xác %",
    [58] = "Bỏ qua né tránh %",
    [59] = "Sát thương vật lý",
    [60] = "Băng sát",
    [61] = "Hỏa sát",
    [62] = "Lôi sát",
    [63] = "Độc sát",
    [64] = "Sát thương ngũ hành",
    [65] = "Tăng sát thương vật lý %",
    [66] = "Hút sinh lực %",
    [67] = "Hút nội lực %",
    [68] = "Hút thể lực %",
    [69] = "Đẩy lùi %",
    [70] = "Chí mạng %",
    [71] = "Chí tử %",
    [72] = "Làm choáng %",
    [73] = "Ngũ hành tương khắc %",

    -- Normal attributes
    [85] = "Sinh lực tối đa",
    [86] = "Sinh lực tối đa %",
    [87] = "Sinh lực",
    [88] = "Hồi sinh lực",
    [89] = "Nội lực tối đa",
    [90] = "Nội lực tối đa %",
    [91] = "Nội lực",
    [92] = "Hồi nội lực",
    [93] = "Thể lực tối đa",
    [94] = "Thể lực tối đa %",
    [95] = "Thể lực",
    [96] = "Hồi thể lực",
    [97] = "Sức mạnh",
    [98] = "Thân pháp",
    [99] = "Sinh khí",
    [100] = "Nội công",
    [101] = "Kháng độc %",
    [102] = "Kháng hỏa %",
    [103] = "Kháng lôi %",
    [104] = "Kháng vật lý %",
    [105] = "Kháng băng %",
    [106] = "Giảm thời gian băng %",
    [107] = "Giảm thời gian cháy %",
    [108] = "Giảm thời gian độc %",
    [109] = "Giảm sát thương độc",
    [110] = "Giảm thời gian choáng %",
    [111] = "Tốc độ chạy %",
    [112] = "Tầm nhìn %",
    [113] = "Hồi phục nhanh",
    [114] = "Kháng tất cả %",
    [115] = "Tốc độ đánh",
    [116] = "Tốc độ xuất chiêu",
    [117] = "Phản sát thương cận",
    [118] = "Phản sát thương cận %",
    [119] = "Phản sát thương xa",
    [120] = "Phản sát thương xa %",
    [121] = "Sát thương vật lý +",
    [122] = "Hỏa sát +",
    [123] = "Băng sát +",
    [124] = "Lôi sát +",
    [125] = "Độc sát +",
    [126] = "Sát thương vật lý % +",
    [129] = "Giáp vật lý",
    [130] = "Giáp băng",
    [131] = "Giáp hỏa",
    [132] = "Giáp độc",
    [133] = "Giáp lôi",
    [135] = "May mắn",
}

-- Get attribute name by ID, returns "Unknown" if not found
function GetMagicAttribName(nAttribType)
    if MagicAttribName[nAttribType] then
        return MagicAttribName[nAttribType]
    else
        return "Thuộc tính #" .. nAttribType
    end
end
