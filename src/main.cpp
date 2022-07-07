// Copyright (C) 2022 All rights reserved.
// Email: oxox0@qq.com. Created in 202206

#include <iostream>

#include "lua/Lua.h"

using namespace lua;

// module load xxx, require(xxx) in lua
// int luaopen_xxx(lua_State *L) {
//   luaL_newlib(L, tuplelib);
//   return 1;
// }
// static const struct luaL_Reg tuplelib[] = {
//   {"new", t_new},
//   {NULL, NULL}
// }
//
//
// // cclosure
// lua_pashcclosure(L, c_fun, count);
// int c_fun(lua_State *L) {
//     lua_integer op = luaL_optinteger(L, 1, 0); // the paraemter count <= 256
//      lua_upvalueindex(index);
//      lua_copy(L, index, lua_upvalueindex(index)); // update
//
// }
//
// // user data
// lua_newuserdata(L, size);
// lua_touserdata(L, index);
//
// lua_pushlightuserdata
//
// // metatable
// luaL_newmetatable(L, name);
// luaL_getmetatable(L, name);
// luaL_checkudata(L, index, name):
//
// // object
// lua_setfield
// luaL_setfunc
// __newindex
// __index
// __len
// __tostring

/*
 *template<typename Rty, typename... Aty>
 *int LuaCallFunc(Lua &lua) {
 *
 *}
 *
 *#define LUA_REGISTER_FUN(lua_func, call_func) \
 *    int LuaCallFrom##lua_funcTo(lua_state *l) { \
 *        call_func
 *
 *    }
 */

/*
 *class LuaModule {
 *
 *public:
 *    LuaModuel(){
 *
 *    }
 *
 *};
 */

int test_register_fun() {
    Lua lua;
    //RESITER(lua, "mysin", impl::Func);
    //lua.Register("mySin", )
    return 0;
}

int test_stack() {
    Lua lua(false);
    lua.push(true);
    lua.push(10);
    lua.push();
    lua.push("hello");

    lua.stackDump();

    lua.copyToTop(-4); lua.stackDump();
    lua.replaceTopTo(4); lua.stackDump();
    lua.resizeStack(6); lua.stackDump();
    lua.rotateStack(3, 1); lua.stackDump();
    lua.removeStack(-3); lua.stackDump();
    lua.resizeStack(-5); lua.stackDump();
    return 0;
}

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

/*
 *int lua_p() {
 *    char buff[1024];
 *    int error;
 *    lua_State *L = luaL_newstate();
 *    luaL_openlibs(L);
 *
 *    while (fgets(buff, sizeof(buff), stdin) != NULL) {
 *        error = luaL_loadstring(L, buff) || lua_pcall(L, 0, 0, 0);
 *        if (error) {
 *            fprintf(stderr, "%s\n", lua_tostring(L, -1));
 *            lua_pop(L, 1);
 *        }
 *    }
 *
 *    lua_close(L);
 *    return 0;
 *}
 */

// c call lua



// lua call c
//lua_register

void test_unintialize() {
    int x;
    printf ("x = %d\n", x);
}

void test_mem_leak() {
    char *str = (char*)malloc(5 * sizeof(char));
}

void test_illegal_read_write() {
    char* alphabet = (char*)calloc(26, sizeof(char));

    for(uint8_t i = 0; i < 20; i++) {
        *(alphabet + i) = 'A' + i;
    }
    *(alphabet + 26) = 'a'; //illegal write
    char a = alphabet[26]; // illegal read
    a = alphabet[20]; // uninitialize value

    free(alphabet);
}

void test_illegal_free() {
    char* alphabet = (char*)calloc(26, sizeof(char));
    free(alphabet);
    //free(alphabet);
    delete alphabet;
}

int main(int args, char **argv) {
    std::ios::sync_with_stdio(false);
    //return lua_p();
    //return test_stack();
    //return test_run_str();
    //return test_load_file(args - 1, &argv[1]);
    //return test_call_fun();

    /*
     *test_unintialize();
     *test_mem_leak();
     *test_illegal_read_write();
     *test_illegal_free();
     */
    return 0;
}
