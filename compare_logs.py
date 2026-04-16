import sys
import re

def parse_log(filepath):
    data = {}
    try:
        with open(filepath, 'r') as f:
            for line in f:
                # Matches lines like:
                #   01-test_io...                  PASS (Cost: 250, Instructions: 4)
                #   program3 (input#1)...          PASS (Cost: 11 828, Instructions: 191)
                match = re.search(r'^\s*(.+?)\.\.\.\s+PASS \(Cost: ([\d\s]+), Instructions: ([\d\s]+)\)', line)
                if match:
                    name = match.group(1).strip()
                    cost = int(match.group(2).replace(' ', ''))
                    instr = int(match.group(3).replace(' ', ''))
                    data[name] = {'cost': cost, 'instr': instr}
    except FileNotFoundError:
        print(f"Error: File '{filepath}' not found.")
        sys.exit(1)
    return data

def compare_logs(file1, file2):
    data1 = parse_log(file1)
    data2 = parse_log(file2)
    
    all_tests = sorted(list(set(data1.keys()) | set(data2.keys())))
    
    # Headers
    # Test Name | Cost 1 | Cost 2 | Diff | Instr 1 | Instr 2 | Diff
    header = f"{'Test Name':<30} | {'Cost 1':<10} | {'Cost 2':<10} | {'Diff %':<8} | {'Instr 1':<8} | {'Instr 2':<8} | {'Diff':<5}"
    print("-" * len(header))
    print(header)
    print("-" * len(header))
    
    total_cost_diff = 0
    total_instr_diff = 0
    valid_comparisons = 0

    for test in all_tests:
        d1 = data1.get(test)
        d2 = data2.get(test)
        
        c1 = f"{d1['cost']}" if d1 else "-"
        c2 = f"{d2['cost']}" if d2 else "-"
        
        i1 = f"{d1['instr']}" if d1 else "-"
        i2 = f"{d2['instr']}" if d2 else "-"
        
        c_diff_s = "-"
        i_diff_s = "-"
        
        if d1 and d2:
            c_diff = d2['cost'] - d1['cost']
            i_diff = d2['instr'] - d1['instr']
            
            # Formatting Cost Diff as Percentage
            if c_diff == 0:
                c_diff_s = "--"
            elif d1['cost'] != 0:
                pct = (c_diff / d1['cost']) * 100
                c_diff_s = f"{pct:+.2f}%"
            else:
                c_diff_s = "0.00%"

            if i_diff == 0:
                i_diff_s = "--"
            else:
                i_diff_s = f"{i_diff:+d}"
            
            total_cost_diff += c_diff
            total_instr_diff += i_diff
            valid_comparisons += 1

        print(f"{test:<30} | {c1:<10} | {c2:<10} | {c_diff_s:<8} | {i1:<8} | {i2:<8} | {i_diff_s:<5}")

    print("-" * len(header))
    if valid_comparisons > 0:
        print(f"Total Cost Diff: {total_cost_diff:+d}")
        print(f"Total Instr Diff: {total_instr_diff:+d}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 compare_logs.py <old_log> <new_log>")
        sys.exit(1)
    compare_logs(sys.argv[1], sys.argv[2])
