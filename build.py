import os
import subprocess
import sys
import platform

# Detect platform
os_name = platform.system()

# Compilation settings
compiler = "g++"
sources = ["main.cpp", "chip8.cpp", "interface.cpp"]
output = "chip8.exe" if os_name == "Windows" else "chip8"
include_dirs = ["."]
library_dirs = ["."]
if os_name == "Windows":
    libraries = ["raylib", "winmm", "gdi32"]
elif os_name == "Linux":
    libraries = ["raylib", "pthread", "m", "dl"]
else:
    print(f"Unsupported OS: {os_name}")
    sys.exit(1)

# Construct command
cmd = [compiler]
cmd.extend(sources)
cmd.extend(["-o", output])
for inc in include_dirs:
    cmd.append(f"-I{inc}")
for lib_dir in library_dirs:
    cmd.append(f"-L{lib_dir}")
for lib in libraries:
    cmd.append(f"-l{lib}")

# Run build
print("Compiling with command:")
print(" ".join(cmd))

try:
    subprocess.run(cmd, check=True)
    print("Build succeeded!")
except subprocess.CalledProcessError:
    print("Build failed.")
    sys.exit(1)
