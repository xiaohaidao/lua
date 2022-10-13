# Copyright (C) 2022 dobot All rights reserved.
# Author: libobo@dobot.cc. Created in 202210

# cmake -DCMAKE_TOOLCHAIN_FILE=${file}

# set(CMAKE_SYSTEM_NAME Linux)
# set(CMAKE_SYSTEM_PROCESSOR arm)

# set(tools /opt/gcc-linaro-4.9.4-arm-linux-gnueabihf/bin)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
