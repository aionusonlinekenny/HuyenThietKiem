# Hướng Dẫn Thêm Model 3D Cho NPCs 2085/2086/2087

## Vấn Đề Hiện Tại

NPCs **2085, 2086, 2087** (Đồng/Bạc/Vàng Tiêu Xa) đã được định nghĩa trong file `Npcs.txt` nhưng **KHÔNG CÓ NpcResType** (model 3D), nên crash khi spawn.

## Cách 2: Sửa File Npcs.txt Để Thêm Model

### Bước 1: Mở File Npcs.txt

File: `/home/user/HuyenThietKiem/Bin/Server/Settings/Npcs.txt`

### Bước 2: Tìm Dòng NPCs 2085/2086/2087

Hiện tại (dòng 2086-2088):
```
Đồng Tiêu Xa	0	5	0		[TRỐNG]		enemy241		...
Bạc Tiêu Xa	0	5	0		[TRỐNG]		enemy241		...
Vàng Tiêu Xa	0	5	0		[TRỐNG]		enemy241		...
```

**Cột thứ 5-6-7 TRỐNG** = không có model!

### Bước 3: Chọn Model Để Thêm

Có 3 lựa chọn:

#### Option A: Dùng Building Model (Vật Thể)
```
Đồng Tiêu Xa	0	5	0		[TRỐNG]	[TRỐNG]	building1	...
Bạc Tiêu Xa	0	5	0		[TRỐNG]	[TRỐNG]	building2	...
Vàng Tiêu Xa	0	5	0		[TRỐNG]	[TRỐNG]	building3	...
```

Có 30 building models: `building1` đến `building30`

#### Option B: Dùng Passerby Model (NPC Người)
```
Đồng Tiêu Xa	0	5	0		[TRỐNG]	[TRỐNG]	passerby379	...
Bạc Tiêu Xa	0	5	0		[TRỐNG]	[TRỐNG]	passerby380	...
Vàng Tiêu Xa	0	5	0		[TRỐNG]	[TRỐNG]	passerby381	...
```

Có nhiều passerby models (001-381+)

#### Option C: Dùng Enemy/Boss Model
```
Đồng Tiêu Xa	0	5	0		[TRỐNG]	[TRỐNG]	enemy001	...
```

### Bước 4: Cấu Trúc File Npcs.txt

File có dạng TAB-separated values:

```
Name	Kind	Camp	Series	Treasure	HeadImage	ClientOnly	CorpseIdx	RedLum	GreenLum	BlueLum	NpcResType	...
```

**NpcResType** (cột 12) = tên model 3D

### Bước 5: Ví Dụ Sửa Dòng 2086

**TRƯỚC** (crash):
```
Đồng Tiêu Xa	0	5	0								enemy241	[các cột khác...]
```

**SAU** (có model):
```
Đồng Tiêu Xa	0	5	0								building1	[các cột khác...]
```

Hoặc:
```
Đồng Tiêu Xa	0	5	0	1			43				passerby036_1	[các cột khác...]
```

### Bước 6: Test Các Building Models

Để tìm model đẹp nhất, test từng cái:

1. Sửa NPC 2085 dùng `building1`
2. Sửa NPC 2086 dùng `building2`
3. Sửa NPC 2087 dùng `building3`
4. Spawn in-game và xem model nào đẹp
5. Thử tiếp `building4`, `building5`, ... cho đến khi tìm được model giống cart

### Bước 7: Cấu Trúc Đầy Đủ Của 1 NPC Entry

Ví dụ NPC 2159 (có model):

```
Ông chủ Tiêu cục Lục Tam Cốn	3	6	0								passerby036_1	0	0	0	0	0	47	33	[...]
```

Sao chép format này cho 2085/2086/2087:

```
Đồng Tiêu Xa	3	6	0								building15	0	0	0	0	0	22	30	[giữ nguyên các cột sau]
```

## Cách 3: Tạo Model 3D Mới (Advanced)

Nếu muốn tạo model cart 3D hoàn toàn mới:

### 1. Tools Cần Thiết
- **3D Modeling Software**: 3DS Max, Blender, Maya
- **JX/Sword Online Model Tools**: Công cụ chuyển đổi .spr format

### 2. Quy Trình
1. Tạo model 3D cart trong 3DS Max/Blender
2. Export sang định dạng JX Engine (.spr hoặc .ini)
3. Đặt file vào `/Bin/Client/Spr/Npc/[tên_model]/`
4. Cập nhật NpcRes settings
5. Thêm tên model vào Npcs.txt

### 3. File Model Locations
- Client models: `/Bin/Client/Spr/Npc/`
- NpcRes configs: `/Bin/Client/Settings/NpcRes/`

**LƯU Ý**: Tạo model mới rất phức tạp, khuyên dùng models có sẵn trước.

## Giải Pháp Tạm Thời (Đang Dùng)

Hiện tại code đang test **NPCs 1153/1154/1155** (building1/2/3).

Test xem model nào đẹp, nếu không đẹp thì:
- Thử building4-30
- Hoặc sửa file Npcs.txt thêm model cho 2085/2086/2087

## Commands Test

```bash
# Xem cấu trúc NPC có model
sed -n '2160p' /home/user/HuyenThietKiem/Bin/Server/Settings/Npcs.txt

# Xem NPC building
sed -n '1153p' /home/user/HuyenThietKiem/Bin/Server/Settings/Npcs.txt

# Backup trước khi sửa
cp Npcs.txt Npcs.txt.backup
```

## Khuyến Nghị

1. **Nhanh**: Dùng building models có sẵn (NPCs 1153-1186)
2. **Trung bình**: Sửa Npcs.txt thêm model cho 2085/2086/2087
3. **Lâu**: Tạo model 3D mới từ đầu

Bạn nên chọn option 1 hoặc 2.
