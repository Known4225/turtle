'''
13.08.25
This script copies the windows and linux libraries as well as the package turtle.h file from the develop branch to the main branch

It will run a check to ensure that the example project builds in both windows and wsl, and will then push to git
'''

import os
import shutil
import subprocess

# Build on development
outWindows = subprocess.run("make winrel").returncode
outLinux = subprocess.run("wsl -e make rel").returncode
if outWindows != 0 or outLinux != 0:
    quit()

# Check if file exists
if not os.path.isfile("turtle.c"):
    print("Could not find file turtle.c")
    quit()
if not os.path.isfile("turtle.h"):
    print("Could not find file turtle.h")
    quit()
if not os.path.isfile("Windows/turtle.lib"):
    print("Could not find file Windows/turtle.lib")
    quit()
if not os.path.isfile("Linux/libturtle.a"):
    print("Could not find file Linux/libturtle.a")
    quit()
if not os.path.isdir("../turtle-main"):
    print("Could not find directory ../turtle-main")
    quit()

print("\
████████╗██╗   ██╗██████╗ ████████╗██╗     ███████╗    ███╗   ███╗ █████╗ ██╗███╗   ██╗\n\
╚══██╔══╝██║   ██║██╔══██╗╚══██╔══╝██║     ██╔════╝    ████╗ ████║██╔══██╗██║████╗  ██║\n\
   ██║   ██║   ██║██████╔╝   ██║   ██║     █████╗█████╗██╔████╔██║███████║██║██╔██╗ ██║\n\
   ██║   ██║   ██║██╔══██╗   ██║   ██║     ██╔══╝╚════╝██║╚██╔╝██║██╔══██║██║██║╚██╗██║\n\
   ██║   ╚██████╔╝██║  ██║   ██║   ███████╗███████╗    ██║ ╚═╝ ██║██║  ██║██║██║ ╚████║\n\
   ╚═╝    ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚══════╝    ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝")
# Move files to turtle-main
shutil.copy("turtle.c", "../turtle-main/turtle.c")
shutil.copy("turtle.h", "../turtle-main/turtle.h")
shutil.copy("Windows/turtle.lib", "../turtle-main/Windows/turtle.lib")
shutil.copy("Linux/libturtle.a", "../turtle-main/Linux/libturtle.a")

# Build on main
os.chdir("../turtle-main")
outWindows = subprocess.run("make winrel").returncode
outLinux = subprocess.run("wsl -e make rel").returncode
# Push to git if checks passed
if outWindows == 0 and outLinux == 0:
    subprocess.run("git pull")
    subprocess.run("git add .")
    commitMessage = input("Enter commit message: ")
    subprocess.run(f"git commit -m\"{commitMessage}\"")
    subprocess.run("git push")
    os.chdir("../turtle-development")
    subprocess.run("git pull")
    subprocess.run("git add .")
    subprocess.run(f"git commit -m\"{commitMessage}\"")
    subprocess.run("git push")