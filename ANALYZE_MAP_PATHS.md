# Phân tích đường dẫn map files

## Quan sát từ pak file:

### Format .wor files:
```
��ԭ����\�䵱��\�䵱��.wor           → 3 cấp, tên cuối lặp
�����õ�\������ս��.wor                 → 2 cấp
���ϱ���\����\ҽҩ��.wor               → 3 cấp
��������\���ɽ\���ɽ.wor              → 3 cấp, tên cuối lặp
���ϱ���\��������.wor                   → 2 cấp
������\������\��ͥ����ɽ��1.wor      → 3 cấp
```

### Format region files:
```
\maps\��ԭ����\����\v_095\094_region_s.dat
\maps\������\���궴�Թ�\v_095\100_region_s.dat
\maps\��������\��ɽ\v_129\142_region_s.dat
```

## Phân tích cách game load:

### Từ KSubWorld::LoadMap():

```cpp
// Step 1: Read MapList.ini
g_SetFilePath("\\settings");
IniFile.Load("MapList.ini");
IniFile.GetString("List", szKeyName, "", szPathName, sizeof(szPathName));
// szPathName = value from MapList.ini, VD: "场景地图\城市\成都"

// Step 2: Load .wor file
g_SetFilePath("\\maps");
sprintf(szFileName, "%s.wor", szPathName);
// szFileName = "场景地图\城市\成都.wor"
IniFile.Load(szFileName);
// => Tìm file tại: \maps\场景地图\城市\成都.wor

// Step 3: Set path cho regions
sprintf(szPath, "\\maps\\%s", szPathName);
// szPath = "\maps\场景地图\城市\成都"
g_SetFilePath(szPath);
```

### Từ KRegion::LoadObject():

```cpp
// Base path đã được set = "\maps\场景地图\城市\成都"
g_GetFilePath(szFile);  // szFile = "\maps\场景地图\城市\成都"
sprintf(szFilePath, "\\%sv_%03d", szFile, nY);
// szFilePath = "\maps\场景地图\城市\成都v_000"
sprintf(szFile, "%s\\%03d_%s", szFilePath, nX, REGION_COMBIN_FILE_NAME_SERVER);
// szFile = "\maps\场景地图\城市\成都v_000\000_Region_S.dat"
```

## Vấn đề:

Game tạo path: `\maps\场景地图\城市\成都.wor`
Nhưng pak có path: `\maps\场景地图\城市\成都\成都.wor`

Có 2 khả năng:
1. Game engine tự động xử lý khi load từ pak
2. Có thêm logic nào đó trong g_SetFilePath hoặc IniFile.Load

## Giải pháp:

Cần xem cách XPackFile/KPakFile xử lý file paths
