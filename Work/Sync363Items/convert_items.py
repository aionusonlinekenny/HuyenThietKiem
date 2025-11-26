#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Convert Gold Items from ThienDieu format (57 cols) to HuyenThietKiem format (56 cols)
"""

import sys

def convert_row(thien_cols):
    """
    ThienDieu (57 columns):
    1-45: Name, ItemGenre, ... Require6, Data6
    46-51: DefMagic1-6
    52: Group
    53: NeedToActive1
    54: NeedToActive2
    55: SetId
    56-57: ExpandMagic1-2

    HuyenThietKiem (56 columns):
    1-45: ItemName, ItemGenre, ... PropReq6, Param
    46-51: Magic1-6
    52: Set
    53: SetNum
    54: SetId
    55-56: ExtraMagic1-2
    """

    if len(thien_cols) < 57:
        # Pad with empty strings if needed
        thien_cols.extend([''] * (57 - len(thien_cols)))

    huyen_cols = []

    # Columns 1-45: Direct mapping (Name ... Data6 -> ItemName ... Param)
    huyen_cols.extend(thien_cols[0:45])

    # Columns 46-51: DefMagic1-6 -> Magic1-6
    huyen_cols.extend(thien_cols[45:51])  # Python index 45-50 = cols 46-51

    # Column 52: Group -> Set
    huyen_cols.append(thien_cols[51])  # Python index 51 = col 52

    # Column 53: NeedToActive1 -> SetNum
    huyen_cols.append(thien_cols[52])  # Python index 52 = col 53

    # Column 54: SetId (skip NeedToActive2)
    huyen_cols.append(thien_cols[54])  # Python index 54 = col 55

    # Columns 55-56: ExpandMagic1-2 -> ExtraMagic1-2
    huyen_cols.append(thien_cols[55])  # Python index 55 = col 56
    huyen_cols.append(thien_cols[56])  # Python index 56 = col 57

    return huyen_cols

def main():
    if len(sys.argv) != 3:
        print("Usage: python3 convert_items.py <input_thien.txt> <output_huyen.txt>")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]

    converted_count = 0
    error_count = 0

    with open(input_file, 'r', encoding='latin-1') as fin, \
         open(output_file, 'w', encoding='latin-1') as fout:

        for line_num, line in enumerate(fin, 1):
            line = line.rstrip('\n\r')
            thien_cols = line.split('\t')

            try:
                huyen_cols = convert_row(thien_cols)
                fout.write('\t'.join(huyen_cols) + '\n')
                converted_count += 1

                if converted_count % 50 == 0:
                    print(f"Converted {converted_count} items...")

            except Exception as e:
                print(f"ERROR at line {line_num}: {e}")
                error_count += 1

    print(f"\nConversion complete:")
    print(f"  Converted: {converted_count} items")
    print(f"  Errors: {error_count}")
    print(f"  Output: {output_file}")

if __name__ == '__main__':
    main()
