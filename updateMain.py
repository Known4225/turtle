'''
13.08.25
This script copies the windows and linux libraries as well as the package turtle.h file from the develop branch to the main branch

It will run a check to ensure that the example project builds in both windows and wsl, and will then push to git
'''

import os
import shutil
import subprocess

# Build on development
outWindows = subprocess.run("make winrel")
outWindows = subprocess.run("make winreltextures")
outLinux = subprocess.run("wsl -e make rel")
outLinux = subprocess.run("wsl -e make reltextures")
if outWindows.returncode != 0 or outLinux.returncode != 0:
    quit()

# Check if file exists
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

# Move files to turtle-main
shutil.copy("turtle.h", "../turtle-main/turtle.h")
shutil.copy("Windows/turtle.lib", "../turtle-main/Windows/turtle.lib")
shutil.copy("Linux/libturtle.a", "../turtle-main/Linux/libturtle.a")

# Build on main
os.chdir("../turtle-main")
outWindows = subprocess.run("make winrel")
outLinux = subprocess.run("wsl -e make rel")
# Push to git if checks passed
if outWindows.returncode == 0 and outLinux.returncode == 0:
    subprocess.run("git add .")
    commitMessage = input("Enter commit message: ")
    subprocess.run(f"git commit -m\"{commitMessage}\"")
    subprocess.run("git push")