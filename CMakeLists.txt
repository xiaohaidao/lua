# Copyright (C) 2022 dobot All rights reserved.
# Author: libobo@dobot.cc. Created in 202204

cmake_minimum_required(VERSION 3.14)

project("c_lua")

# import common configure
include(cmake/common.cmake)

# the depend on lua need add add_dependencies(your_target lua_build)
include(lua)

add_subdirectory(src)
#add_subdirectory(example)

option(BUILD_TESTING "Build test" OFF)
include(CTest)
if(BUILD_TESTING)
    include(googletest)

    ADD_SUBDIRECTORY(tests)
endif()

