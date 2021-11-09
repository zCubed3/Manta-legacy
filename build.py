#!/bin/python3

# We need os.system and other os things
import os

# https://stackoverflow.com/questions/1868714/how-do-i-copy-an-entire-directory-of-files-into-an-existing-directory-using-pyth/22331852
from distutils.dir_util import copy_tree

# and we need json for storing the settings, this is flexible to allow users to easily modify their builds
import jsons


class BuildSettings:
    content_dir = "REPLACE_ME"


# If it doesn't exist we provide a default one and abort
if not os.path.exists("./build_config.json"):
    print("Warning: ./build_config.json wasn't found, creating a default one!")
    print("Please populate the fields within ./build_config.json!")

    temp = open("./build_config.json", "w")
    temp.write(jsons.dumps(BuildSettings()))
    temp.close()

    exit(1)

file = open("./build_config.json", "r")
build_settings = jsons.loads(file.read(), BuildSettings)
file.close()

print("Content dir: %s" % build_settings.content_dir)
print("")

# and we need regex
import re

# Since we expect the end user to handle cmake themselves, we need to automatically find cmake- directories
# https://stackoverflow.com/questions/973473/getting-a-list-of-all-subdirectories-in-the-current-directory
cmake_build_dirs = []

for e in os.scandir():
    if e.is_dir() and "cmake-" in e.path: # Strict, must contain cmake-!
        print("Found cmake build dir: %s" % e.path)
        cmake_build_dirs.append(e.path)

# Then we actually invoke the build command dependent on the platform
# TODO: Other platforms than Unix-like (Linux in this case)
for dir in cmake_build_dirs:
    print("\nBuilding: %s" % dir)
    os.system("make --directory=%s" % dir)


# After building we need to populate the builds with the default content
for e in os.scandir("./builds"):
    if e.is_dir():
        print("Copying engine content to %s" % e.path)
        copy_tree(build_settings.content_dir, e.path)