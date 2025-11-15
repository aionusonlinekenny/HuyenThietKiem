-- Simple test script for Tiêu Đầu NPC
-- Test if basic dialog works

function main(nNpcIdx)
	Talk(1, "", "Test: NPC script loaded successfully!")
	Say("Tiêu Đầu - Test Dialog", 2,
		"Option 1/test1",
		"Exit/no")
end

function test1()
	Talk(1, "", "Test option 1 clicked!")
end

function no()
	-- Do nothing, close dialog
end
