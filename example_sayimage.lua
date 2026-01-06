-- ========================================
-- EXAMPLES: SayImage() Usage
-- Các ví dụ sử dụng SayImage() để hiển thị NPC avatar
-- ========================================

-- ========================================
-- Example 1: Simple dialog with auto-close
-- Dialog đơn giản, tự động đóng khi click
-- ========================================
function Example1_SimpleDialog()
    SayImage(
        "Xin chào! Ta là Tống Quan viên của làng này.",
        "10/20/17",    -- passerby004s1.spr at position (10, 20)
        0              -- No options = auto close
    )
end

-- ========================================
-- Example 2: Dialog with one option
-- Dialog với 1 lựa chọn
-- ========================================
function Example2_OneOption()
    SayImage(
        "Chào mừng đến với Tân Thủ Thôn! Ta sẽ hướng dẫn ngươi.",
        "10/20/18",    -- passerby028z.spr
        1,             -- 1 option
        "Bắt đầu hướng dẫn/Example2_StartGuide"
    )
end

function Example2_StartGuide()
    Say("Rất tốt! Hãy đi theo đường này...", 0)
end

-- ========================================
-- Example 3: Dialog with multiple options
-- Dialog với nhiều lựa chọn
-- ========================================
function Example3_MultipleOptions()
    SayImage(
        "Ngươi muốn làm gì với ta?",
        "15/25/19",    -- passerby007s.spr
        3,             -- 3 options
        "Mua vật phẩm/Example3_Shop",
        "Nhận nhiệm vụ/Example3_Quest",
        "Kết thúc đối thoại/Example3_Exit"
    )
end

function Example3_Shop()
    Say("Ta không bán gì cả, hãy đến chợ!", 0)
end

function Example3_Quest()
    Say("Ta không có nhiệm vụ cho ngươi lúc này.", 0)
end

function Example3_Exit()
    -- Dialog closes automatically
end

-- ========================================
-- Example 4: Quest dialog chain
-- Chuỗi dialog nhiệm vụ
-- ========================================
function Example4_QuestStart()
    SayImage(
        "Làng của chúng ta đang gặp nguy hiểm!",
        "10/20/20",    -- passerby165_st.spr
        2,
        "Chuyện gì đã xảy ra?/Example4_QuestInfo",
        "Ta không quan tâm/Example4_QuestDecline"
    )
end

function Example4_QuestInfo()
    SayImage(
        "Lũ sói đang tấn công gia súc của chúng ta! Hãy giúp ta đánh chúng đi!",
        "10/20/20",    -- Same NPC
        2,
        "Ta sẽ giúp ngươi/Example4_QuestAccept",
        "Ta không thể giúp/Example4_QuestDecline"
    )
end

function Example4_QuestAccept()
    SayImage(
        "Cảm ơn ngươi! Hãy đánh bại 10 con sói và quay lại gặp ta!",
        "10/20/20",    -- Same NPC
        0              -- Auto close after accepting
    )
    -- TODO: Add quest to player's quest log
end

function Example4_QuestDecline()
    SayImage(
        "Thật đáng tiếc... Làng của chúng ta sẽ gặp khó khăn.",
        "10/20/20",    -- Same NPC
        0              -- Auto close
    )
end

-- ========================================
-- Example 5: Enemy encounter
-- Gặp quái vật
-- ========================================
function Example5_EnemyEncounter()
    SayImage(
        "Grrr... Ngươi dám xâm phạm lãnh địa của ta?!",
        "20/30/32",    -- enemy170_st.spr at position (20, 30)
        3,
        "Tấn công!/Example5_Attack",
        "Chạy trốn/Example5_Run",
        "Thương lượng/Example5_Negotiate"
    )
end

function Example5_Attack()
    Say("Đánh nhau!", 0)
    -- TODO: Start combat
end

function Example5_Run()
    Say("Ngươi chạy trốn...", 0)
    -- TODO: Teleport player away
end

function Example5_Negotiate()
    SayImage(
        "Grr... Ta sẽ tha cho ngươi lần này. Nhưng đừng quay lại!",
        "20/30/32",    -- Same enemy
        0
    )
end

-- ========================================
-- Example 6: Merchant with different positions
-- Thương nhân với vị trí khác nhau
-- ========================================
function Example6_MerchantLeft()
    -- Image on the left side
    SayImage(
        "Chào mừng đến gian hàng!",
        "10/20/21",    -- passerby051z.spr - left position
        2,
        "Mua đồ/Example6_Buy",
        "Bán đồ/Example6_Sell"
    )
end

function Example6_MerchantCenter()
    -- Image more centered
    SayImage(
        "Chào mừng đến gian hàng!",
        "50/30/21",    -- passerby051z.spr - center position
        2,
        "Mua đồ/Example6_Buy",
        "Bán đồ/Example6_Sell"
    )
end

function Example6_Buy()
    Say("Mở cửa hàng...", 0)
    -- TODO: Open shop UI
end

function Example6_Sell()
    Say("Bán vật phẩm...", 0)
    -- TODO: Open sell UI
end

-- ========================================
-- Example 7: Tutorial with sequential steps
-- Hướng dẫn với các bước tuần tự
-- ========================================
function Example7_Tutorial_Step1()
    SayImage(
        "Chào mừng đến với game! Ta sẽ hướng dẫn ngươi.",
        "10/20/22",    -- passerby144_st.spr
        1,
        "Tiếp tục/Example7_Tutorial_Step2"
    )
end

function Example7_Tutorial_Step2()
    SayImage(
        "Đầu tiên, hãy học cách di chuyển. Sử dụng chuột hoặc phím mũi tên.",
        "10/20/22",    -- Same NPC
        1,
        "Đã hiểu/Example7_Tutorial_Step3"
    )
end

function Example7_Tutorial_Step3()
    SayImage(
        "Tiếp theo, hãy học cách tấn công. Nhấn phím Space để tấn công.",
        "10/20/22",    -- Same NPC
        1,
        "Đã hiểu/Example7_Tutorial_Step4"
    )
end

function Example7_Tutorial_Step4()
    SayImage(
        "Rất tốt! Bây giờ ngươi đã sẵn sàng phiêu lưu. Chúc may mắn!",
        "10/20/22",    -- Same NPC
        0              -- End of tutorial
    )
end

-- ========================================
-- Example 8: Using different NPC sprites
-- Sử dụng nhiều sprite NPC khác nhau
-- ========================================
function Example8_NPCShowcase()
    -- Cycle through different NPCs
    local npcList = {
        {key = 17, name = "Người lính"},
        {key = 18, name = "Thương nhân"},
        {key = 19, name = "Nông dân"},
        {key = 32, name = "Sói đầu đàn"},
        {key = 33, name = "Quỷ đỏ"},
    }

    -- Show first NPC
    SayImage(
        "Ta là " .. npcList[1].name .. "!",
        "10/20/" .. npcList[1].key,
        1,
        "Xem NPC tiếp theo/Example8_ShowNext"
    )
end

function Example8_ShowNext()
    -- This would need to track state to show next NPC
    -- For simplicity, just show a different one
    SayImage(
        "Ta là Thương nhân!",
        "10/20/18",
        1,
        "Quay lại/Example8_NPCShowcase"
    )
end

-- ========================================
-- Example 9: Conditional dialog based on player level
-- Dialog có điều kiện dựa trên level
-- ========================================
function Example9_LevelBasedDialog()
    -- Get player level (example function, adjust to your API)
    -- local playerLevel = GetPlayerLevel()

    -- For demo, let's assume level 10
    local playerLevel = 10

    if playerLevel < 10 then
        SayImage(
            "Ngươi còn quá yếu! Hãy quay lại khi đạt level 10.",
            "10/20/23",    -- passerby129_st.spr
            0
        )
    elseif playerLevel >= 10 and playerLevel < 20 then
        SayImage(
            "Tốt lắm! Ngươi đã đủ mạnh. Ta có nhiệm vụ cho ngươi.",
            "10/20/23",
            2,
            "Nhận nhiệm vụ/Example9_AcceptQuest",
            "Từ chối/Example9_DeclineQuest"
        )
    else
        SayImage(
            "Ngươi đã rất mạnh! Ta không có gì để dạy ngươi nữa.",
            "10/20/23",
            0
        )
    end
end

function Example9_AcceptQuest()
    Say("Nhiệm vụ đã được thêm vào!", 0)
end

function Example9_DeclineQuest()
    Say("Hãy quay lại khi ngươi muốn nhận nhiệm vụ.", 0)
end

-- ========================================
-- Example 10: Error handling - Image not found
-- Xử lý lỗi - Ảnh không tồn tại
-- ========================================
function Example10_SafeImageLoad()
    -- Always validate ImageKey exists in PictureList.ini
    -- If unsure, use a known key like 17

    local imageKey = 17  -- Default safe key
    local npcName = "NPC"

    -- Try to use custom key, fallback to default if invalid
    -- In real code, you'd check if key exists
    local customKey = 999  -- This probably doesn't exist

    if customKey > 0 and customKey <= 72 then  -- Max keys in PictureList.ini
        imageKey = customKey
    end

    SayImage(
        "Ta là " .. npcName .. "!",
        "10/20/" .. imageKey,
        0
    )
end

-- ========================================
-- USAGE INSTRUCTIONS
-- ========================================
print("=== SayImage() Examples Loaded ===")
print("")
print("Available test functions:")
print("Example1_SimpleDialog() - Simple auto-close dialog")
print("Example2_OneOption() - Dialog with one option")
print("Example3_MultipleOptions() - Dialog with 3 options")
print("Example4_QuestStart() - Quest dialog chain")
print("Example5_EnemyEncounter() - Enemy dialog")
print("Example6_MerchantLeft() - Merchant with left position")
print("Example7_Tutorial_Step1() - Tutorial sequence")
print("Example8_NPCShowcase() - Show different NPCs")
print("Example9_LevelBasedDialog() - Conditional dialog")
print("Example10_SafeImageLoad() - Safe image loading")
print("")
print("Call any function to test SayImage()!")
print("")
print("Format: SayImage(text, 'X/Y/ImageKey', optionCount, ...)")
print("See SAYIMAGE_GUIDE.md for complete documentation.")
