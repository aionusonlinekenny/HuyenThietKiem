# KẾ HOẠCH ĐỒNG BỘ HỆ THỐNG GOLD ITEM
## ThienDieu → HuyenThietKiem

**Dự án:** Sync Gold Item Systems
**Ngày:** 2025-11-26
**Branch:** claude/sync-gold-item-systems-01VqDsY6XKn8RfrNfhe8AB6m

---

## EXECUTIVE SUMMARY

Sau khi phân tích chi tiết, đề xuất **thêm 363 gold items thiếu** từ ThienDieu vào HuyenThietKiem thông qua chuyển đổi format.

**Lợi ích:**
- Tăng 6.4% variety cho người chơi
- Risk có thể kiểm soát
- Không phá vỡ code hiện tại
- Dễ rollback

**Timeline:** 7-11 ngày
**Effort:** Medium
**Risk:** Medium (có thể giảm thiểu)

---

## PHASE 1: PREPARATION (2 ngày)

### Task 1.1: Backup & Setup
```bash
# 1. Backup HuyenThietKiem
cd /home/user/HuyenThietKiem
mkdir -p Backup/Sync_$(date +%Y%m%d)
cp -r Bin/Server/Settings/Item/ Backup/Sync_$(date +%Y%m%d)/
cp -r Bin/Client/Settings/Item/ Backup/Sync_$(date +%Y%m%d)/

# 2. Create working directory
mkdir -p Work/GoldSync
cd Work/GoldSync
```

**Output:** Backup files ready

### Task 1.2: Extract Items Lists
```bash
# 1. Extract ThienDieu items
awk -F'\t' 'NR>1 {print NR-1 "\t" $1}' \
  /home/user/ThienDieuOnline/bin/server/settings/item/GoldEquip.txt \
  > thien_items.txt

# 2. Extract HuyenThietKiem items
awk -F'\t' 'NR>1 {print NR-1 "\t" $1}' \
  /home/user/HuyenThietKiem/Bin/Server/Settings/Item/GoldEquip.txt \
  > huyen_items.txt

# 3. Find missing items (only in ThienDieu)
comm -23 \
  <(awk '{print $2}' thien_items.txt | sort) \
  <(awk '{print $2}' huyen_items.txt | sort) \
  > missing_items_names.txt

# 4. Get line numbers of missing items
grep -F -f missing_items_names.txt thien_items.txt | awk '{print $1}' \
  > missing_items_lines.txt

# 5. Count
wc -l missing_items_names.txt
# Expected: 363
```

**Output:**
- `thien_items.txt` - All ThienDieu items
- `huyen_items.txt` - All HuyenThietKiem items
- `missing_items_names.txt` - 363 missing names
- `missing_items_lines.txt` - Line numbers to extract

### Task 1.3: Column Mapping Analysis
```bash
# 1. Compare headers
head -1 /home/user/ThienDieuOnline/bin/server/settings/item/GoldEquip.txt \
  > thien_header.txt
head -1 /home/user/HuyenThietKiem/Bin/Server/Settings/Item/GoldEquip.txt \
  > huyen_header.txt

# 2. Count columns
awk -F'\t' '{print NF}' thien_header.txt  # Should be 55
awk -F'\t' '{print NF}' huyen_header.txt  # Should be 52

# 3. Side-by-side comparison (manual review)
paste -d '\n' thien_header.txt huyen_header.txt | less
```

**Output:** Column mapping document

**Deliverable:**
- Confirmed 363 missing items
- Column mapping ready
- Backup completed

---

## PHASE 2: SCRIPT DEVELOPMENT (2-3 ngày)

### Task 2.1: Write Conversion Script

**File:** `convert_gold_items.py`

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Convert Gold Items from ThienDieu format to HuyenThietKiem format

ThienDieu: 55 columns
HuyenThietKiem: 52 columns

Key differences:
- Name -> ItemName
- Image -> ImageName
- ObjDrop -> ObjIdx
- Descript -> Intro
- Magic1-7 (with Min1-3, Max1-3) -> PropType1-7 (Min, Max) + Magic1-6
- Group -> Set
- NeedToActive1/2 -> SetNum
- ExpandMagic -> ExtraMagic
"""

import sys
import csv
import logging

logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')

# Column mapping: ThienDieu index -> HuyenThietKiem index
BASIC_MAPPING = {
    0: 0,   # Name -> ItemName
    1: 1,   # ItemGenre
    2: 2,   # DetailType
    3: 3,   # ParticularType
    4: 4,   # Image -> ImageName
    5: 5,   # ObjDrop -> ObjIdx
    6: 6,   # Width
    7: 7,   # Height
    8: 8,   # Descript -> Intro
    9: 9,   # Series
    10: 10, # Price
    11: 11, # Level
}

def convert_row(thien_row):
    """Convert single row from ThienDieu to HuyenThietKiem format"""

    if len(thien_row) < 55:
        logging.warning(f"Row too short: {len(thien_row)} columns (expected 55)")
        return None

    huyen_row = [''] * 52

    # Basic fields
    for t_idx, h_idx in BASIC_MAPPING.items():
        huyen_row[h_idx] = thien_row[t_idx]

    # PropType1-7 mapping
    # ThienDieu: Magic1(12) Min1(13) Max1(14) Magic2(15) Min2(16) Max2(17) ... Magic7(30) Min7(31) Max7(32)
    # HuyenThiet: PropType1(12) Min(13) Max(14) PropType2(15) Min(16) Max(17) ... PropType7(30) Min(31) Max(32)
    # Note: ThienDieu has Min1/Min2/Min3 for each Magic, we only use Min1

    for i in range(7):
        thien_magic_idx = 12 + i * 9  # Each magic block: Magic(0) Min1(1) Max1(2) ... Min3(7) Max3(8)
        huyen_prop_idx = 12 + i * 3

        if thien_magic_idx + 2 < len(thien_row):
            huyen_row[huyen_prop_idx] = thien_row[thien_magic_idx]       # Magic -> PropType
            huyen_row[huyen_prop_idx + 1] = thien_row[thien_magic_idx + 1]  # Min1 -> Min
            huyen_row[huyen_prop_idx + 2] = thien_row[thien_magic_idx + 2]  # Max1 -> Max

    # Requirements (33-44: same positions in both)
    for i in range(12):
        req_idx = 33 + i
        if req_idx < len(thien_row):
            huyen_row[req_idx] = thien_row[req_idx]

    # DefMagic1-6 -> Magic1-6 (45-50: same positions)
    for i in range(6):
        magic_idx = 45 + i
        if magic_idx < len(thien_row):
            huyen_row[magic_idx] = thien_row[magic_idx]

    # Set fields
    # ThienDieu: Group(51) NeedToActive1(52) NeedToActive2(53) SetId(54) ExpandMagic1(55) ExpandMagic2(56)
    # HuyenThiet: Set(51) SetNum(52) SetId(53) ExtraMagic1(54) ExtraMagic2(55)

    huyen_row[51] = thien_row[51] if len(thien_row) > 51 else ''  # Group -> Set
    huyen_row[52] = thien_row[52] if len(thien_row) > 52 else ''  # NeedToActive1 -> SetNum
    huyen_row[53] = thien_row[54] if len(thien_row) > 54 else ''  # SetId
    huyen_row[54] = thien_row[55] if len(thien_row) > 55 else ''  # ExpandMagic1 -> ExtraMagic1
    huyen_row[55] = thien_row[56] if len(thien_row) > 56 else ''  # ExpandMagic2 -> ExtraMagic2

    return huyen_row

def main():
    if len(sys.argv) != 4:
        print("Usage: python convert_gold_items.py <thien_goldequip.txt> <missing_lines.txt> <output.txt>")
        sys.exit(1)

    thien_file = sys.argv[1]
    missing_lines_file = sys.argv[2]
    output_file = sys.argv[3]

    # Load line numbers to extract
    with open(missing_lines_file, 'r') as f:
        missing_lines = set(int(line.strip()) for line in f if line.strip())

    logging.info(f"Loading {len(missing_lines)} items from ThienDieu format")

    # Convert
    converted_count = 0
    error_count = 0

    with open(thien_file, 'r', encoding='utf-8') as fin, \
         open(output_file, 'w', encoding='utf-8', newline='') as fout:

        reader = csv.reader(fin, delimiter='\t')
        writer = csv.writer(fout, delimiter='\t', lineterminator='\n')

        header = next(reader)  # Skip header

        for line_num, row in enumerate(reader, start=1):
            if line_num not in missing_lines:
                continue

            huyen_row = convert_row(row)

            if huyen_row:
                writer.writerow(huyen_row)
                converted_count += 1
                logging.info(f"Line {line_num}: Converted '{row[0]}'")
            else:
                error_count += 1
                logging.error(f"Line {line_num}: Failed to convert '{row[0]}'")

    logging.info(f"\nConversion complete:")
    logging.info(f"  Converted: {converted_count}")
    logging.info(f"  Errors: {error_count}")
    logging.info(f"  Output: {output_file}")

if __name__ == '__main__':
    main()
```

**Output:** `convert_gold_items.py` script

### Task 2.2: Write Validation Script

**File:** `validate_goldequip.py`

```python
#!/usr/bin/env python3
"""Validate GoldEquip.txt format"""

import sys
import csv

def validate(filepath, expected_cols=52):
    """Validate GoldEquip.txt file"""

    errors = []
    warnings = []

    with open(filepath, 'r', encoding='utf-8') as f:
        reader = csv.reader(f, delimiter='\t')

        header = next(reader)
        if len(header) != expected_cols:
            errors.append(f"Header: Expected {expected_cols} columns, got {len(header)}")

        for line_num, row in enumerate(reader, start=2):
            if len(row) == 0:
                warnings.append(f"Line {line_num}: Empty row")
                continue

            if len(row) != expected_cols:
                errors.append(f"Line {line_num}: Expected {expected_cols} columns, got {len(row)}")

            # Check required fields not empty
            if not row[0]:  # ItemName
                errors.append(f"Line {line_num}: ItemName is empty")

            # Check numeric fields
            try:
                int(row[1])  # ItemGenre
            except ValueError:
                errors.append(f"Line {line_num}: ItemGenre not numeric: '{row[1]}'")

    print(f"Validation results for: {filepath}")
    print(f"  Errors: {len(errors)}")
    print(f"  Warnings: {len(warnings)}")

    if errors:
        print("\nErrors:")
        for err in errors[:10]:  # Show first 10
            print(f"  - {err}")
        if len(errors) > 10:
            print(f"  ... and {len(errors) - 10} more")

    if warnings:
        print("\nWarnings:")
        for warn in warnings[:5]:
            print(f"  - {warn}")
        if len(warnings) > 5:
            print(f"  ... and {len(warnings) - 5} more")

    return len(errors) == 0

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: python validate_goldequip.py <file.txt>")
        sys.exit(1)

    success = validate(sys.argv[1])
    sys.exit(0 if success else 1)
```

**Output:** `validate_goldequip.py` script

### Task 2.3: Test Scripts on Sample Data

```bash
# 1. Extract first 10 missing items for testing
head -10 missing_items_lines.txt > test_lines.txt

# 2. Run conversion
python convert_gold_items.py \
  /home/user/ThienDieuOnline/bin/server/settings/item/GoldEquip.txt \
  test_lines.txt \
  test_output.txt

# 3. Validate output
python validate_goldequip.py test_output.txt

# 4. Manual review
less test_output.txt

# 5. Compare with original HuyenThietKiem format
head -5 /home/user/HuyenThietKiem/Bin/Server/Settings/Item/GoldEquip.txt
head -5 test_output.txt
```

**Deliverable:**
- Working conversion script
- Working validation script
- Tested on sample data

---

## PHASE 3: FULL CONVERSION (1-2 ngày)

### Task 3.1: Run Full Conversion

```bash
# 1. Convert all 363 items
python convert_gold_items.py \
  /home/user/ThienDieuOnline/bin/server/settings/item/GoldEquip.txt \
  missing_items_lines.txt \
  new_items_huyen_format.txt

# 2. Verify count
wc -l new_items_huyen_format.txt
# Expected: 363

# 3. Validate format
python validate_goldequip.py new_items_huyen_format.txt

# 4. Check for duplicates
sort new_items_huyen_format.txt | uniq -d
# Should be empty
```

**Output:** `new_items_huyen_format.txt` with 363 items

### Task 3.2: Merge with Existing File

```bash
# 1. Backup original
cp /home/user/HuyenThietKiem/Bin/Server/Settings/Item/GoldEquip.txt \
   GoldEquip_original.txt

# 2. Create merged file
head -1 GoldEquip_original.txt > GoldEquip_merged.txt  # Header
tail -n +2 GoldEquip_original.txt >> GoldEquip_merged.txt  # Original items
cat new_items_huyen_format.txt >> GoldEquip_merged.txt  # New items

# 3. Verify line count
wc -l GoldEquip_merged.txt
# Expected: 1 (header) + 5308 (original) + 363 (new) = 5672

# 4. Validate merged file
python validate_goldequip.py GoldEquip_merged.txt

# 5. Check for duplicate names
tail -n +2 GoldEquip_merged.txt | awk -F'\t' '{print $1}' | sort | uniq -d
# Should be empty
```

**Output:** `GoldEquip_merged.txt` with 5,672 items

### Task 3.3: Update GoldEquipRes.txt (if needed)

```bash
# 1. Check if new items need resource mapping
# Extract new item IDs (line numbers from merged file)
# Compare with existing GoldEquipRes.txt

# 2. If needed, add new resource mappings
# Format: ItemID\tResourceID\tFlash
# (May need to create resources or map to existing ones)

# For now, we assume resources are already mapped or will use default
```

**Deliverable:**
- `GoldEquip_merged.txt` - Final merged file
- Validated, no errors
- No duplicates

---

## PHASE 4: TESTING (3-5 ngày)

### Task 4.1: Setup Test Server

```bash
# 1. Deploy to test environment
cp GoldEquip_merged.txt /path/to/test/server/Settings/Item/GoldEquip.txt

# 2. Copy to client as well
cp GoldEquip_merged.txt /path/to/test/client/Settings/Item/GoldEquip.txt

# 3. Start test server
cd /path/to/test/server
./start_server.sh

# 4. Monitor logs
tail -f logs/server.log | grep -i "gold\|error\|crash"
```

### Task 4.2: Execute Test Cases

**Test Case 1: Server Startup**
- [ ] Server starts without crash
- [ ] No errors in log about GoldEquip.txt loading
- [ ] Log shows correct item count

**Test Case 2: Generate Existing Items**
```
/gold 1      # First item (existing)
/gold 100    # Random existing item
/gold 5000   # Near-end existing item
```
- [ ] Items generate successfully
- [ ] Stats display correctly
- [ ] Can equip/unequip

**Test Case 3: Generate New Items**
```
/gold 5310   # First new item
/gold 5400   # Middle new item
/gold 5671   # Last new item
```
- [ ] Items generate successfully
- [ ] Stats display correctly
- [ ] Can equip/unequip
- [ ] Magic attributes apply

**Test Case 4: Set Bonuses**
- [ ] Equip items from same set
- [ ] Set bonus activates
- [ ] Set bonus stats apply
- [ ] Unequip removes bonus

**Test Case 5: Item Display**
- [ ] Item icon displays correctly
- [ ] Item tooltip shows correct info
- [ ] Item name displays correctly
- [ ] Item stats readable

**Test Case 6: Save/Load**
- [ ] Save character with new item equipped
- [ ] Logout
- [ ] Login
- [ ] Item still equipped correctly
- [ ] Stats still correct

**Test Case 7: Trade/Drop**
- [ ] Can trade new item to another player
- [ ] Can drop new item on ground
- [ ] Other player can pick up
- [ ] Item maintains properties

**Test Case 8: Performance**
```bash
# Measure server startup time
time ./start_server.sh

# Measure memory usage
ps aux | grep server | awk '{print $6}'
```
- [ ] Startup time < +5% increase
- [ ] Memory usage < +2% increase

### Task 4.3: Bug Fixing

If any test fails:
1. Document the bug
2. Investigate root cause
3. Fix (script or data)
4. Re-convert
5. Re-test

**Deliverable:**
- All test cases passed
- No critical bugs
- Performance acceptable

---

## PHASE 5: DEPLOY TO PRODUCTION (1 ngày)

### Task 5.1: Pre-Deploy Preparation

```bash
# 1. Final backup
cd /home/user/HuyenThietKiem
tar -czf Backup/GoldEquip_PreDeploy_$(date +%Y%m%d_%H%M%S).tar.gz \
  Bin/Server/Settings/Item/GoldEquip.txt \
  Bin/Client/Settings/Item/GoldEquip.txt

# 2. Prepare deployment package
mkdir -p Deploy/GoldSync_$(date +%Y%m%d)
cp Work/GoldSync/GoldEquip_merged.txt \
   Deploy/GoldSync_$(date +%Y%m%d)/GoldEquip.txt

# 3. Create checksum
md5sum Deploy/GoldSync_$(date +%Y%m%d)/GoldEquip.txt > \
       Deploy/GoldSync_$(date +%Y%m%d)/checksum.md5

# 4. Prepare rollback script
cat > Deploy/GoldSync_$(date +%Y%m%d)/rollback.sh << 'EOF'
#!/bin/bash
echo "Rolling back GoldEquip.txt..."
cp Backup/GoldEquip_PreDeploy_*/GoldEquip.txt Bin/Server/Settings/Item/
cp Backup/GoldEquip_PreDeploy_*/GoldEquip.txt Bin/Client/Settings/Item/
echo "Rollback complete. Please restart server."
EOF
chmod +x Deploy/GoldSync_$(date +%Y%m%d)/rollback.sh
```

### Task 5.2: Deployment

**Maintenance Window:** 3:00 AM - 5:00 AM

**Steps:**

1. **Announce (T-24h)**
   ```
   Thông báo: Bảo trì hệ thống
   Thời gian: 3:00-5:00 ngày DD/MM/YYYY
   Nội dung: Cập nhật gold items mới
   Dự kiến: 2 giờ
   ```

2. **Stop Server (T-0h)**
   ```bash
   ./stop_server.sh
   # Wait for graceful shutdown
   sleep 30
   # Force kill if needed
   killall -9 server_process_name
   ```

3. **Deploy Files**
   ```bash
   # Server
   cp Deploy/GoldSync_*/GoldEquip.txt \
      Bin/Server/Settings/Item/GoldEquip.txt

   # Client
   cp Deploy/GoldSync_*/GoldEquip.txt \
      Bin/Client/Settings/Item/GoldEquip.txt

   # Verify checksum
   md5sum Bin/Server/Settings/Item/GoldEquip.txt
   cat Deploy/GoldSync_*/checksum.md5
   ```

4. **Start Server**
   ```bash
   ./start_server.sh

   # Monitor startup
   tail -f logs/server.log

   # Check for errors
   grep -i "error\|crash\|fail" logs/server.log
   ```

5. **Verify**
   ```bash
   # Check server running
   ps aux | grep server

   # Test login
   ./test_client.sh

   # Test generate item
   # (via GM account)
   /gold 5671  # Last new item
   ```

6. **Monitor (T+1h to T+24h)**
   - Watch error logs
   - Monitor player feedback
   - Check crash reports
   - Monitor performance metrics

### Task 5.3: Post-Deploy

**If Successful:**
- [ ] Announce completion
- [ ] Document deployment
- [ ] Keep backup for 7 days
- [ ] Close tickets

**If Failed:**
```bash
# Execute rollback
cd Deploy/GoldSync_*
./rollback.sh

# Restart server
./start_server.sh

# Announce rollback
# Investigate issues
# Plan re-deployment
```

**Deliverable:**
- Production deployed
- Verified working
- Documentation complete

---

## TIMELINE & RESOURCES

### Timeline

| Phase | Duration | Dependencies |
|-------|----------|--------------|
| Phase 1: Preparation | 2 days | - |
| Phase 2: Script Development | 2-3 days | Phase 1 |
| Phase 3: Conversion | 1-2 days | Phase 2 |
| Phase 4: Testing | 3-5 days | Phase 3 |
| Phase 5: Deployment | 1 day | Phase 4 |
| **TOTAL** | **9-13 days** | |

### Resources Required

- **Developer:** 1 person (full-time)
- **Tester:** 1 person (part-time, Phase 4)
- **DevOps:** 1 person (part-time, Phase 5)

### Checkpoints

- **Checkpoint 1 (End of Phase 2):** Scripts tested and working
- **Checkpoint 2 (End of Phase 3):** Conversion complete, validated
- **Checkpoint 3 (End of Phase 4):** All tests passed
- **Go/No-Go Decision:** Before Phase 5

---

## RISK MANAGEMENT

### High Priority Risks

| Risk | Probability | Impact | Mitigation |
|------|-------------|--------|------------|
| Conversion errors | Medium | High | Thorough validation, test on sample first |
| Server crash on load | Low | Critical | Test on test server extensively |
| Magic attributes wrong | Medium | High | Validate against existing items |
| Performance degradation | Low | Medium | Benchmark before/after |

### Rollback Plan

**Trigger Conditions:**
- Server crashes repeatedly
- Items cause client crash
- Major bugs affecting gameplay
- Performance degradation > 20%

**Rollback Procedure:**
1. Stop server
2. Restore backup files
3. Restart server
4. Verify stability
5. Announce to players

**Rollback Time:** < 30 minutes

---

## SUCCESS CRITERIA

### Must Have
- [ ] All 363 items imported successfully
- [ ] No server crashes
- [ ] Items can be generated
- [ ] Items can be equipped
- [ ] Magic attributes apply correctly

### Should Have
- [ ] Set bonuses work
- [ ] Items tradeable
- [ ] Performance maintained
- [ ] No visual glitches

### Nice to Have
- [ ] Items documented
- [ ] Admin guide created
- [ ] Player guide created

---

## DELIVERABLES

1. **Scripts**
   - convert_gold_items.py
   - validate_goldequip.py

2. **Data Files**
   - GoldEquip_merged.txt (5,672 items)
   - Backup files

3. **Documentation**
   - This deployment plan
   - Test results
   - Deployment log

4. **Reports**
   - Conversion report
   - Test report
   - Deployment report

---

## APPROVAL

**Prepared by:** Claude Code
**Date:** 2025-11-26

**Approvals Required:**

- [ ] Technical Lead: _______________  Date: _______
- [ ] Project Manager: _______________  Date: _______
- [ ] QA Lead: _______________  Date: _______

**Deployment Authorized:**

- [ ] Production Manager: _______________  Date: _______

---

## NEXT STEPS

1. **Review this plan**
2. **Approve/Modify plan**
3. **Assign resources**
4. **Start Phase 1**

**Questions? Contact:** [Your Contact Info]

---

*End of Sync Plan*
