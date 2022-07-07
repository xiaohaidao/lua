// Copyright (C) 2022 dobot inc All rights reserved.
// Author: libobo@dobot.cc. Created in 202204

#include "gtest/gtest.h"

#include "lua/Lua.h"

using namespace lua;

TEST(LuaTest, LuaStack) {
    Lua lua(false);
    lua.push(true);
    lua.push(10);
    lua.push();
    lua.push("hello");

    lua.stackDump();

    //lua.ToType();

    lua.copyToTop(-4); lua.stackDump();
    lua.replaceTopTo(4); lua.stackDump();
    lua.resizeStack(6); lua.stackDump();
    lua.rotateStack(3, 1); lua.stackDump();
    lua.removeStack(-3); lua.stackDump();
    lua.resizeStack(-5); lua.stackDump();
    EXPECT_TRUE(true);
}

/*
 *int test_register_fun() {
 *    Lua lua;
 *    //RESITER(lua, "mysin", impl::Func);
 *    //lua.Register("mySin", )
 *    return 0;
 *}
 */

int test_to_type() {
    Lua lua;
    //lua.ToType();
    return 0;
}

int test_call_fun() {
    Lua lua;
    lua.CallLuaFun("f", -1, "str", 3.1415926);
    lua.CallLuaFun("math.sin");
    return 0;
}

//int test_load_file(const std::string &file);
int test_load_file(int args, char **argv) {
    Lua lua;
    for (int i = 0; i < args; ++i) {
        LOG_TRACE("file name : %s\n", argv[i]);
        lua.RunFile(argv[i]);
    }
    return 0;
}

int test_run_str() {
    Lua lua;
    bool loop = true;
    while (loop) {
        std::string buff;
        std::cout << "lua >";
        std::getline(std::cin, buff);
        if (buff.compare("q") == 0 || buff.compare("Q") == 0) {
            loop = false;
        } else {
            lua.RunStr(buff);
        }
    }
    std::cout << "exit lua\n";
    return 0;
}


