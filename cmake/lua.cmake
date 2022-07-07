# Copyright (C) 2022 dobot All rights reserved.
# Author: libobo@dobot.cc. Created in 202204

include(FetchContent)
FetchContent_Declare(
    lua
    URL http://www.lua.org/ftp/lua-5.4.4.tar.gz
    URL_HASH SHA256=164c7849653b80ae67bec4b7473b884bf5cc8d2dca05653475ec2ed27b9ebf61
)
FetchContent_MakeAvailable(lua)

add_custom_target(lua_build
    COMMAND make
    COMMAND make local
    WORKING_DIRECTORY ${lua_SOURCE_DIR}
)
include_directories(
    ${lua_SOURCE_DIR}/install/include
)
link_directories(
    ${lua_SOURCE_DIR}/install/lib
)
install(
    DIRECTORY ${lua_SOURCE_DIR}/install/
    DESTINATION .
)

