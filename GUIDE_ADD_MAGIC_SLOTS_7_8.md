# HƯỚNG DẪN THÊM MAGIC SLOT 7 & 8 VÀO HUYENTHIETKIEM
## Thêm Skill Môn Phái và Magic Attribute Thứ 8

**Dự án:** Nâng cấp Gold Item từ 6 magic slots lên 8 magic slots
**Tham khảo:** ThienDieu implementation
**Ngày:** 2025-11-26

---

## PHÂN TÍCH HIỆN TRẠNG

### ThienDieu (Có 8 slots)
```cpp
// GameDataDef.h:105
#define MAX_ITEM_MAGICATTRIB  8

// KBasPropTbl.h:189
typedef struct {
    // ... other fields
    int m_aryMagicAttribs[MAX_ITEM_MAGICATTRIB];  // 8 slots
} KBASICPROP_EQUIPMENT_GOLD;
```

**File format:** 57 columns total
- Columns 12-33: Magic1-Magic7 (7 magic blocks × 3 columns mỗi: Magic, Min, Max)
- **7 magic blocks được define trong file** nhưng code support **8 slots**
- Magic slot 8 có thể được generate runtime hoặc lưu trong DefMagic

### HuyenThietKiem (Chỉ có 6 slots)
```cpp
// GameDataDef.h:102
#define MAX_ITEM_MAGICATTRIB  6

// KBasPropTbl.h:115
typedef struct {
    // ... other fields
    int m_aryMagicAttribs[6];  // Hardcoded 6 slots
} KBASICPROP_GOLD_EQUIPMENT;
```

**File format:** 56 columns total
- Columns 12-32: PropType1-PropType7 (7 property blocks × 3 columns)
- **7 property types được define trong file** nhưng code chỉ support **6 slots**
- PropType7 (columns 31-33) TỒN TẠI trong file nhưng KHÔNG được sử dụng!

### Phát Hiện Quan Trọng

**HuyenThietKiem đã có PropType7 trong file data nhưng code không đọc!**
- File có 56 columns (đã có chỗ cho 7 properties)
- Struct chỉ có array[6] nên property thứ 7 bị bỏ qua
- Cần extend lên array[8] để có 2 slots mới (7 & 8)

---

## KẾ HOẠCH TRIỂN KHAI

### Option 1: Nâng Cấp Lên 7 Slots (Tối Thiểu)
- Thay đổi từ 6 -> 7
- Sử dụng PropType7 đã có trong file
- Ít thay đổi code nhất

### Option 2: Nâng Cấp Lên 8 Slots (Giống ThienDieu)
- Thay đổi từ 6 -> 8
- Match với ThienDieu architecture
- Dễ dàng sync data sau này
- **RECOMMENDED**

**Chúng ta sẽ chọn Option 2: Nâng lên 8 slots**

---

## BƯỚC 1: THAY ĐỔI DEFINES & STRUCTS

### File 1: GameDataDef.h

**Vị trí:** `/home/user/HuyenThietKiem/SwordOnline/Sources/Core/Src/GameDataDef.h`

**Thay đổi:**
```cpp
// BEFORE (Line 102):
#define		MAX_ITEM_MAGICATTRIB			6

// AFTER:
#define		MAX_ITEM_MAGICATTRIB			8
```

**Lưu ý:** Có thể cần thêm define cho MAGICLEVEL:
```cpp
// Check if exists, nếu không có thì thêm:
#define		MAX_ITEM_MAGICLEVEL				MAX_ITEM_MAGICATTRIB * 2
```

### File 2: KBasPropTbl.h

**Vị trí:** `/home/user/HuyenThietKiem/SwordOnline/Sources/Core/Src/KBasPropTbl.h`

**Thay đổi:**
```cpp
// BEFORE (Line ~115):
typedef struct {
    char        m_szName[SZBUFLEN_0];
    int         m_nGenre;
    int         m_nDetailType;
    int         m_nParticularType;
    char        m_szIcon[SZBUFLEN_0];
    int         m_nObjID;
    int         m_nWidth;
    int         m_nHeight;
    char        m_szIntro[SZBUFLEN_1];
    int         m_nSeries;
    int         m_nPrice;
    int         m_nLevel;
    KEQCP_BASIC m_aryPropBasic[7];
    KEQCP_REQ   m_aryPropReq[6];
    int         m_aryMagicAttribs[6];     // ❌ OLD: Hardcoded 6
} KBASICPROP_GOLD_EQUIPMENT;

// AFTER:
typedef struct {
    char        m_szName[SZBUFLEN_0];
    int         m_nGenre;
    int         m_nDetailType;
    int         m_nParticularType;
    char        m_szIcon[SZBUFLEN_0];
    int         m_nObjID;
    int         m_nWidth;
    int         m_nHeight;
    char        m_szIntro[SZBUFLEN_1];
    int         m_nSeries;
    int         m_nPrice;
    int         m_nLevel;
    KEQCP_BASIC m_aryPropBasic[7];
    KEQCP_REQ   m_aryPropReq[6];
    int         m_aryMagicAttribs[MAX_ITEM_MAGICATTRIB];  // ✅ NEW: Use define (8 slots)
} KBASICPROP_GOLD_EQUIPMENT;
```

---

## BƯỚC 2: THAY ĐỔI CODE GENERATOR

### File 3: KItemGenerator.CPP

**Vị trí:** `/home/user/HuyenThietKiem/SwordOnline/Sources/Core/Src/KItemGenerator.CPP`

#### Change 1: Gen_GoldEquipment Function (Line ~514)

**Thay đổi loop:**
```cpp
// BEFORE (Line ~546):
for (i = 0; i < 6; i++)  // ❌ Hardcoded 6
{
    const int* pSrc;
    KItemNormalAttrib* pDst;
    pSrc = &(pTemp->m_aryMagicAttribs[i]);
    pDst = &(sMA[i]);
    // ... processing code
}

// AFTER:
for (i = 0; i < MAX_ITEM_MAGICATTRIB; i++)  // ✅ Use define (8)
{
    const int* pSrc;
    KItemNormalAttrib* pDst;
    pSrc = &(pTemp->m_aryMagicAttribs[i]);
    pDst = &(sMA[i]);
    // ... processing code
}
```

#### Change 2: Array Declaration (Line ~544)

**Thay đổi:**
```cpp
// BEFORE:
KItemNormalAttrib sMA[6];  // ❌ Hardcoded

// AFTER:
KItemNormalAttrib sMA[MAX_ITEM_MAGICATTRIB];  // ✅ Use define
```

#### Change 3: Random Level Generation (Line ~522)

**Thay đổi:**
```cpp
// BEFORE:
for (i = 0; i < 6; i++)  // ❌ Hardcoded
{
    pItem->m_GeneratorParam.nGeneratorLevel[i] = ::GetRandomNumber(0, 10);
}

// AFTER:
for (i = 0; i < MAX_ITEM_MAGICATTRIB; i++)  // ✅ Use define
{
    pItem->m_GeneratorParam.nGeneratorLevel[i] = ::GetRandomNumber(0, 10);
}
```

---

## BƯỚC 3: THAY ĐỔI FILE LOADING

### File 4: KBasPropTbl.cpp (Load GoldEquip.txt)

**Vị trí:** `/home/user/HuyenThietKiem/SwordOnline/Sources/Core/Src/KBasPropTbl.cpp`

**Tìm function:** `KBPT_Gold_Equipment::LoadRecord()`

**Thay đổi:**
```cpp
// Tìm section đọc Magic1-6 (DefMagic1-6)

// BEFORE (approximate line numbers):
for (int i = 0; i < 6; i++)  // ❌ Hardcoded
{
    char szCol[32];
    sprintf(szCol, "Magic%d", i + 1);
    pTF->GetInteger(nRow, szCol, 0, &pEqu->m_aryMagicAttribs[i]);
}

// AFTER:
for (int i = 0; i < MAX_ITEM_MAGICATTRIB; i++)  // ✅ Use define
{
    char szCol[32];
    sprintf(szCol, "Magic%d", i + 1);
    pTF->GetInteger(nRow, szCol, 0, &pEqu->m_aryMagicAttribs[i]);
}
```

**Lưu ý:** Cần kiểm tra tên columns trong GoldEquip.txt:
- Hiện tại: Magic1, Magic2, Magic3, Magic4, Magic5, Magic6
- Cần thêm: Magic7, Magic8 (hoặc tương ứng với PropType7/8)

---

## BƯỚC 4: UPDATE FILE DATA

### File 5: GoldEquip.txt Format

**Vị trí (Server):** `/home/user/HuyenThietKiem/Bin/Server/Settings/Item/GoldEquip.txt`
**Vị trí (Client):** `/home/user/HuyenThietKiem/Bin/Client/Settings/Item/GoldEquip.txt`

#### Current Format (56 columns):
```
ItemName | ItemGenre | DetailType | ParticularType | ImageName | ObjIdx | Width | Height |
Intro | Series | Price | Level |
PropType1 | Min | Max |  // Columns 12-14
PropType2 | Min | Max |  // Columns 15-17
PropType3 | Min | Max |  // Columns 18-20
PropType4 | Min | Max |  // Columns 21-23
PropType5 | Min | Max |  // Columns 24-26
PropType6 | Min | Max |  // Columns 27-29
PropType7 | Min | Max |  // Columns 30-32 ✅ Đã tồn tại!
PropReq1 | Param | ... | PropReq6 | Param |  // Columns 33-44
Magic1-6 | Set | SetNum | SetId | ExtraMagic1 | ExtraMagic2
```

#### Target Format (59 columns):
```
ItemName | ItemGenre | DetailType | ParticularType | ImageName | ObjIdx | Width | Height |
Intro | Series | Price | Level |
PropType1 | Min | Max |
PropType2 | Min | Max |
PropType3 | Min | Max |
PropType4 | Min | Max |
PropType5 | Min | Max |
PropType6 | Min | Max |
PropType7 | Min | Max |  // ✅ Sử dụng existing
PropType8 | Min | Max |  // ✅ THÊM MỚI (columns 33-35)
PropReq1 | Param | ... | PropReq6 | Param |  // Shift to columns 36-47
Magic1-8 | Set | SetNum | SetId | ExtraMagic1 | ExtraMagic2
```

#### Script Thêm PropType8:

**Option A: Thêm columns trống**
```bash
# Backup first
cp GoldEquip.txt GoldEquip.txt.backup

# Add 3 empty columns after column 32 (PropType7 Max)
awk -F'\t' 'BEGIN{OFS="\t"}
{
    # Print first 32 columns
    for(i=1; i<=32; i++) printf "%s\t", $i
    # Add 3 new columns (PropType8, Min8, Max8)
    printf "\t\t\t"
    # Print remaining columns
    for(i=33; i<=NF; i++) printf "%s%s", $i, (i<NF ? "\t" : "\n")
}' GoldEquip.txt.backup > GoldEquip.txt
```

**Option B: Thêm columns với default values**
```bash
# Add PropType8 with value 0
awk -F'\t' 'BEGIN{OFS="\t"}
NR==1 {
    # Header row
    for(i=1; i<=32; i++) printf "%s\t", $i
    printf "PropType8\tMin\tMax\t"
    for(i=33; i<=NF; i++) printf "%s%s", $i, (i<NF ? "\t" : "\n")
    next
}
{
    # Data rows
    for(i=1; i<=32; i++) printf "%s\t", $i
    printf "0\t0\t0\t"  # Default values for PropType8
    for(i=33; i<=NF; i++) printf "%s%s", $i, (i<NF ? "\t" : "\n")
}' GoldEquip.txt.backup > GoldEquip.txt
```

---

## BƯỚC 5: UPDATE RELATED CODE

### Files Cần Kiểm Tra & Update:

#### 1. KItem.h & KItem.cpp
**Kiểm tra:**
```cpp
// KItem.h (Line ~105)
KItemNormalAttrib m_aryMagicAttrib[MAX_ITEM_MAGICATTRIB];
```
✅ Nếu dùng MAX_ITEM_MAGICATTRIB thì tự động update

#### 2. KItemList.cpp
**Tìm kiếm:** `for.*< 6.*MAGIC`
**Thay đổi:** Hardcoded 6 -> MAX_ITEM_MAGICATTRIB

Ví dụ:
```cpp
// Line ~115
for (int i = 0; i < MAX_ITEM_MAGICATTRIB; i++)  // Changed from 6
```

#### 3. KPlayer.cpp
**Tìm kiếm:** Item magic attribute loops
```cpp
// Line ~9821
for (int k = 0; k < MAX_ITEM_MAGICATTRIB; k++)  // Changed from 6
```

#### 4. ScriptFuns.cpp (Lua API)
**Functions:**
- `LuaGetGoldItemMagic`
- `LuaModifyGoldItemMagic`

```cpp
// Update return values
return MAX_ITEM_MAGICATTRIB * 3;  // Instead of 6 * 3
```

#### 5. KProtocolProcess.cpp (Network Sync)
**Kiểm tra struct:**
```cpp
memcpy(Item[nIndex].m_GeneratorParam.nGeneratorLevel,
       pItemSync->m_MagicLevel,
       sizeof(int) * MAX_ITEM_MAGICATTRIB);  // Should auto-update
```

⚠️ **QUAN TRỌNG:** Cần kiểm tra protocol sync between client/server!
Nếu protocol có hardcoded size, cần update cả client & server cùng lúc.

---

## BƯỚC 6: REBUILD & TEST

### 6.1 Compile Order

**Server:**
```bash
cd /home/user/HuyenThietKiem/SwordOnline/Sources/Core
make clean
make all

# Check for errors related to:
# - Array bounds
# - Magic attribute processing
# - Protocol sync
```

**Client:**
```bash
cd /home/user/HuyenThietKiem/SwordOnline/Sources/S3Client
make clean
make all
```

### 6.2 Test Cases

**Test 1: Generate Gold Item**
```lua
-- In game console
/gold 1     -- First gold item
/gold 100   -- Random item
/gold 5000  -- High ID item
```
**Verify:**
- Item generates successfully
- All 8 magic attributes visible (if applicable)
- No crashes

**Test 2: Check Magic Attributes**
```lua
-- Lua script test
function TestGoldItemMagic()
    local idx = FindItemByIndex(2, 1)  -- Find first gold item
    if idx > 0 then
        for i = 1, 8 do
            local magic = GetGoldItemMagic(idx, i)
            print("Magic " .. i .. ": " .. magic)
        end
    end
end
```

**Test 3: Equip & Unequip**
- Equip gold item with magic slot 7 & 8
- Check stats applied correctly
- Unequip and verify stats removed

**Test 4: Save & Load**
- Save character with gold items
- Logout
- Login
- Verify magic attributes persist

**Test 5: Network Sync (Multiplayer)**
- Player A equips gold item
- Player B observes
- Check item display correctly
- Trade item between players

---

## BƯỚC 7: MIGRATION PLAN

### Phase 1: Development (1-2 tuần)

**Week 1:**
- Day 1-2: Code changes (defines, structs, generator)
- Day 3: File format changes
- Day 4-5: Related code updates
- Day 6-7: Initial testing

**Week 2:**
- Day 1-3: Bug fixes
- Day 4-5: Integration testing
- Day 6-7: Buffer & documentation

### Phase 2: Testing (1 tuần)

**Test Environment Setup:**
- Clone production server
- Deploy changes to test server
- Import test accounts

**Testing:**
- Unit tests (generation, attributes, sync)
- Integration tests (save/load, network)
- Performance tests (no degradation)
- Regression tests (existing items still work)

### Phase 3: Deployment (1 ngày)

**Pre-Deploy:**
- Backup production database
- Backup Settings files
- Prepare rollback plan

**Deploy:**
- Maintenance window (3-5 AM)
- Stop server
- Deploy new binaries
- Deploy new Settings files
- Start server
- Monitor logs

**Post-Deploy:**
- Verify server startup
- Test item generation
- Monitor player feedback
- Watch for crashes

---

## RỦI RO & GIẢM THIỂU

### Risk 1: Array Out of Bounds
**Probability:** Medium
**Impact:** Critical (Crash)
**Mitigation:**
- Use MAX_ITEM_MAGICATTRIB consistently
- Add bounds checking in loops
- Test thoroughly with valgrind/sanitizer

### Risk 2: Protocol Mismatch (Client/Server)
**Probability:** High
**Impact:** Critical (Disconnect)
**Mitigation:**
- Deploy client & server simultaneously
- Check protocol struct sizes
- Version check on connect

### Risk 3: Existing Items Break
**Probability:** Low
**Impact:** High
**Mitigation:**
- Items with < 8 attributes should still work
- Default unset attributes to 0
- Test with old save files

### Risk 4: File Format Corruption
**Probability:** Medium
**Impact:** Critical
**Mitigation:**
- Backup original files
- Validate format with script
- Test load before deploy

### Risk 5: Performance Degradation
**Probability:** Low
**Impact:** Medium
**Mitigation:**
- Benchmark before/after
- Profile hot paths
- Monitor server metrics

---

## CHECKLIST TRIỂN KHAI

### Pre-Development
- [ ] Backup entire codebase
- [ ] Create feature branch
- [ ] Review this document with team

### Code Changes
- [ ] GameDataDef.h: MAX_ITEM_MAGICATTRIB = 8
- [ ] KBasPropTbl.h: Update struct to use define
- [ ] KItemGenerator.CPP: Update Gen_GoldEquipment
- [ ] KItemGenerator.CPP: Update array declarations
- [ ] KItemGenerator.CPP: Update all loops
- [ ] KBasPropTbl.cpp: Update LoadRecord
- [ ] Search for hardcoded '6' in magic context
- [ ] Update all related files (KItem, KItemList, KPlayer, etc.)

### Data Changes
- [ ] Backup GoldEquip.txt (Server)
- [ ] Backup GoldEquip.txt (Client)
- [ ] Add PropType8 columns
- [ ] Validate file format
- [ ] Test file loading

### Build & Test
- [ ] Compile server successfully
- [ ] Compile client successfully
- [ ] No warnings about array sizes
- [ ] Unit test: Generate gold items
- [ ] Unit test: Check magic attributes
- [ ] Integration test: Equip/unequip
- [ ] Integration test: Save/load
- [ ] Integration test: Network sync
- [ ] Performance test
- [ ] Regression test

### Deployment
- [ ] Backup production database
- [ ] Backup production Settings
- [ ] Prepare rollback script
- [ ] Schedule maintenance
- [ ] Announce to players
- [ ] Deploy to production
- [ ] Verify server startup
- [ ] Monitor logs
- [ ] Test in production
- [ ] Monitor player feedback

---

## ROLLBACK PLAN

### Trigger Conditions:
- Server crashes repeatedly
- Items cause corruption
- Network protocol errors
- Critical bugs affecting gameplay

### Rollback Steps:
```bash
# 1. Stop server
./stop_server.sh

# 2. Restore old binaries
cp Backup/server_old /path/to/server
cp Backup/client_old /path/to/client

# 3. Restore old Settings
cp Backup/GoldEquip.txt.backup Bin/Server/Settings/Item/GoldEquip.txt
cp Backup/GoldEquip.txt.backup Bin/Client/Settings/Item/GoldEquip.txt

# 4. Restart server
./start_server.sh

# 5. Verify
tail -f logs/server.log
```

**Rollback Time:** < 30 minutes

---

## SUMMARY

### Thay Đổi Chính:

1. **Defines:** MAX_ITEM_MAGICATTRIB: 6 -> 8
2. **Struct:** m_aryMagicAttribs[6] -> m_aryMagicAttribs[MAX_ITEM_MAGICATTRIB]
3. **Loops:** Hardcoded 6 -> MAX_ITEM_MAGICATTRIB
4. **File Format:** Add PropType8 columns (3 columns)
5. **Testing:** Comprehensive tests across all systems

### Timeline:
- Development: 1-2 tuần
- Testing: 1 tuần
- Deployment: 1 ngày
- **Total: 2-3 tuần**

### Benefits:
- ✅ Support skill môn phái (magic slot 7)
- ✅ Extra magic slot 8 cho flexibility
- ✅ Match ThienDieu architecture
- ✅ Dễ sync data từ ThienDieu
- ✅ Future-proof cho thêm magic types

### Risks:
- ⚠️ Protocol sync issues (client/server)
- ⚠️ Existing items compatibility
- ⚠️ Array bounds errors
- **All mitigable với proper testing**

---

## NEXT STEPS

1. **Review:** Team reviews this document
2. **Approve:** Get approval to proceed
3. **Branch:** Create feature branch `feature/magic-slots-8`
4. **Implement:** Follow step-by-step guide
5. **Test:** Thorough testing phase
6. **Deploy:** Production deployment

---

**Prepared by:** Claude Code Analysis
**Date:** 2025-11-26
**Version:** 1.0
**Status:** Ready for Implementation

---

## APPENDIX A: FILES TO MODIFY

### Core Source Files (C++)
```
/home/user/HuyenThietKiem/SwordOnline/Sources/Core/Src/
├── GameDataDef.h                    [MODIFY] Define MAX_ITEM_MAGICATTRIB
├── KBasPropTbl.h                    [MODIFY] Struct KBASICPROP_GOLD_EQUIPMENT
├── KBasPropTbl.cpp                  [MODIFY] LoadRecord function
├── KItemGenerator.CPP               [MODIFY] Gen_GoldEquipment, arrays, loops
├── KItem.h                          [CHECK] m_aryMagicAttrib declaration
├── KItem.cpp                        [CHECK] Magic attribute processing
├── KItemList.cpp                    [CHECK] Loops with magic attributes
├── KPlayer.cpp                      [CHECK] Player item management
├── ScriptFuns.cpp                   [CHECK] Lua API functions
└── KProtocolProcess.cpp             [CHECK] Network protocol sync
```

### Data Files
```
/home/user/HuyenThietKiem/Bin/
├── Server/Settings/Item/
│   ├── GoldEquip.txt                [MODIFY] Add PropType8 columns
│   └── Magicattrib_GE.txt           [CHECK] Magic definitions
└── Client/Settings/Item/
    ├── GoldEquip.txt                [MODIFY] Add PropType8 columns
    └── Magicattrib_GE.txt           [CHECK] Magic definitions
```

---

## APPENDIX B: SEARCH COMMANDS

### Find Hardcoded '6' Related to Magic:
```bash
cd /home/user/HuyenThietKiem/SwordOnline/Sources/Core/Src

# Search for hardcoded 6 in loops
grep -rn "for.*< 6" . | grep -i "magic\|attrib"

# Search for array declarations
grep -rn "\[6\]" . | grep -i "magic\|attrib"

# Search for MAX_ITEM_MAGICATTRIB usage
grep -rn "MAX_ITEM_MAGICATTRIB" .
```

### Verify Changes:
```bash
# After changes, verify define is used
grep -rn "m_aryMagicAttribs\[" . | grep -v "MAX_ITEM_MAGICATTRIB"
# Should return minimal results (only old comments)
```

---

*End of Implementation Guide*
