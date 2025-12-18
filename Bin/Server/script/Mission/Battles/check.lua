--Author: Kinnox;
--NameApp: CheckCondition Sonjing;
--Date Creat: 12/03/2023;
--Note: Khong duoc chinh sua;
function check(nPlayerIndex,nSelect)
	local dumamay = nSelect;
	SetTask(123,dumamay);
	print("chay vao day",nSelect,dumamay,GetTask(123));
end;