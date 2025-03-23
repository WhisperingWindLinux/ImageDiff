import os

def count_lines_in_files(directory, extensions=('.h', '.cpp')):
    total_lines = 0
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith(extensions):
                file_path = os.path.join(root, file)
                try:
                    with open(file_path, 'r', encoding='utf-8') as f:
                        lines = f.readlines()
                        total_lines += len(lines)
                        print(f"Processed {file_path}: {len(lines)} lines")
                except Exception as e:
                    print(f"Could not process {file_path}: {e}")
    return total_lines

if __name__ == "__main__":
    directory_to_scan = input("Enter the directory to scan: ").strip()
    if os.path.isdir(directory_to_scan):
        total = count_lines_in_files(directory_to_scan)
        print(f"\nTotal number of lines in .h and .cpp files: {total}")
    else:
        print("Invalid directory. Please try again.")