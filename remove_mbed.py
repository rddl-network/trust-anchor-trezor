import os
import shutil
Import("env")

# REMOVE WPA_SUPPLICANT FROM COMPILATION TO PREVENT FUNCTION CONFLICTS
new_libs = [x for x in env["LIBS"] if x != "-lwpa_supplicant"]
env["LIBS"] = new_libs


# REMOVE INTEL SPECIFIC ASSEMBLY LINE 
file_path = "lib/trezor-crypto/aes/aestst.c"
search_string = "__asm__ __volatile__"
# Read the contents of the file
with open(file_path, "r") as file:
    lines = file.readlines()

# Find the line containing the search string and add "//" at the beginning if not present
modified_lines = []
for line in lines:
    if search_string in line and not line.strip().startswith("//"):
        modified_lines.append("//" + line)
    else:
        modified_lines.append(line)

# Write the modified lines back to the file
with open(file_path, "w") as file:
    file.writelines(modified_lines)


# REMOVE SOME FILES
file_paths = ["lib/trezor-crypto/gui", "lib/trezor-crypto/monero", "lib/trezor-crypto/tests", "lib/trezor-crypto/tools"]

for file_path in file_paths:
    # Check if the file exists
    if os.path.exists(file_path):
        # Remove the file
        shutil.rmtree(file_path)

