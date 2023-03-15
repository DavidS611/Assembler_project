import sys

def compare_files(file1, file2):

# Define ANSI escape codes for colors
    RED = '\033[91m'
    GREEN = '\033[92m'
    ENDC = '\033[0m'
    
    difference_detected = False
    with open(file1, 'r') as f1, open(file2, 'r') as f2:
        for line_num, (line1, line2) in enumerate(zip(f1, f2), start=1):
            if line1.strip() != line2.strip():
                difference_detected = True
                print(f"Files '{file1}' and '{file2}' {RED}differ{ENDC}:")
                print(f"\"{line1.strip()}\" \t \"{line2.strip()}\"")
    if not difference_detected:
        print(f"Files '{file1}' and '{file2}' are {GREEN}identical{ENDC}.")

if __name__ == '__main__':
    # Retrieve file paths from command-line arguments
    file1 = sys.argv[1]
    file2 = sys.argv[2]
    # Compare files
    compare_files(file1, file2)

