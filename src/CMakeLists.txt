# Copyright (C) 2022 dobot All rights reserved.
# Author: libobo@dobot.cc. Created in 202204

set(library_name "c_lua")
#add_executable(${library_name}
add_library(${library_name}
    #main.cpp
    lua/Lua.h
    lua/Lua.cpp
    utils/Log.h
)

add_dependencies(${library_name} lua_build)

target_compile_features(${library_name} PUBLIC cxx_std_11)

target_link_libraries(${library_name} PUBLIC
    lua dl
)

target_include_directories(${library_name} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_SOURCE_DIR}/include/
)

#set_target_properties((${library_name}
    #PROPERTIES
    #PUBLIC_HEADER $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
#)

install(TARGETS ${library_name}
)

