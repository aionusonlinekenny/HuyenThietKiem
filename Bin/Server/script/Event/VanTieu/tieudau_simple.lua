-- Simple test - no includes
function main(NpcIndex)
	dofile("script/Event/VanTieu/tieudau_simple.lua")

	Talk(1, "", "TEST: Script loaded OK!")

	Say("Test Dialog - Tiêu Đầu", 3,
		"Option 1/opt1",
		"Option 2/opt2",
		"Exit/no")
end

function opt1()
	Talk(1, "", "Clicked Option 1")
end

function opt2()
	Talk(1, "", "Clicked Option 2")
end

function no()
	-- Close dialog
end
