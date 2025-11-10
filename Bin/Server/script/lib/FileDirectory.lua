
function SetFileName(szFilePath)
	return szFilePath;
end

function IsValidFile(szFilePath) 
	local szCheckFile = TabFile_Load(szFilePath);
	if szCheckFile == nil then 
		print(format("File: %s IsValidFile Error",szFilePath) )
		return nil;
	end
	return 1;
end

function GetRow(szFilePath)
	if not IsValidFile(szFilePath) then 
		return 0;
	end
	local nTotalData = TabFile_GetRowCount(szFilePath);
	if (nTotalData == 0 or nTotalData == nil) then 
		print( format("[%s]Count Data Error!\n", szFilePath) )
		return 0;
	end
	return nTotalData;
end

function GetCell(szFileName,nRow,nColum)
	local Data = TabFile_GetCell(szFileName,nRow,nColum);
	if type(tonumber(Data)) == "number" then 
		return tonumber(Data);
	else
		return Data;
	end
end

function CloseFile(szFileName)
	if(TabFile_Clear(szFileName) == nil) then
		print( format("[%s]Clear Dialoger Data Error!\n", szFileName) )
		return
	end
end

