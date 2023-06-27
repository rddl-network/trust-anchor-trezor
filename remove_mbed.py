import os
import shutil
Import("env")

# REMOVE WPA_SUPPLICANT FROM COMPILATION TO PREVENT FUNCTION CONFLICTS
new_libs = [x for x in env["LIBS"] if x != "-lwpa_supplicant"]
env["LIBS"] = new_libs
