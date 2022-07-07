// Copyright (C) 2022 All rights reserved.
// Email: oxox0@qq.com. Created in 202206


#ifndef LUA_LUA_H
#define LUA_LUA_H

#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>

#include "utils/Log.h"

typedef struct lua_State lua_State;

namespace lua {

class Lua {
public:
    explicit Lua(lua_State *lua);
    explicit Lua(bool use_standard_lib = true);
    ~Lua() {}

public: // interaction: c call lua
    void RunStr(const std::string &str);

    void RunFile(const std::string &file);

    template<typename... Ts>
    void CallLuaFun(const std::string &lua_fun_name, Ts&&... args) {
        GetGlobalToTop(lua_fun_name);
        CallLuaFunP(lua_fun_name, 0, std::forward<Ts>(args)...);
    }

public: // interaction: lua call c


public: // op stack
    void push();
    void push(double value);
    void push(int64_t value);
    void push(bool value);
    void push(const std::string &str);
    void push(const char *s);
    void pushLightUserdata(void *user_data);
    void pushUserData();

    template<typename T>
    void push(T &&v) {
        if (std::is_same<typename std::decay<T>::type, int16_t>::value ||
                std::is_same<typename std::decay<T>::type, int8_t>::value ||
                std::is_same<typename std::decay<T>::type, int32_t>::value ||
                std::is_same<typename std::decay<T>::type, uint64_t>::value ||
                std::is_same<typename std::decay<T>::type, uint32_t>::value ||
                std::is_same<typename std::decay<T>::type, uint16_t>::value ||
                std::is_same<typename std::decay<T>::type, uint8_t>::value) {

            push(static_cast<int64_t>(std::forward<T>(v)));
        } else if (std::is_same<typename std::decay<T>::type, float>::value) {
            push(static_cast<double>(std::forward<T>(v)));
        } else {
            //static_assert(false, "error parameter type");
            LOG_ERROR("error parameter type");
        }
    }


    // default index > 0 is stack from bottom to top, otherwise is top to bottom
    void copyToTop(int index);
    void replaceTopTo(int index);
    void resizeStack(int size);
    void rotateStack(int index, int step); // > 0 is from top to bottom
    void removeStack(int index);

    void pop(size_t count = 1);

    void GetGlobalToTop(const std::string &var);

public: // op type
    int64_t ToType(int index, int64_t &re);
    double ToType(int index, double &re);
    std::string ToType(int index, std::string &re);
    bool ToUserDataType(int index, void *user_data);
    bool ToLightUserdataType(int index, void *user_data);

    template<typename T>
    T  ToType(int index, T &re) {
        // lua_to*();
        if (std::is_same<typename std::decay<T>::type, int32_t>::value ||
                std::is_same<typename std::decay<T>::type, int16_t>::value ||
                std::is_same<typename std::decay<T>::type, int8_t>::value ||
                std::is_same<typename std::decay<T>::type, uint64_t>::value ||
                std::is_same<typename std::decay<T>::type, uint32_t>::value ||
                std::is_same<typename std::decay<T>::type, uint16_t>::value ||
                std::is_same<typename std::decay<T>::type, uint8_t>::value) {

            int64_t r;
            auto x = ToType(index, r);
            re = static_cast<T>(r);
            return x;
        } else if (std::is_same<typename std::decay<T>::type, float>::value) {
            double r;
            auto x = ToType(index, r);
            re = static_cast<T>(r);
            return x;
        } else {
            LOG_ERROR("To type error");
        }
    }

public: // check type
    bool IsInteger();
    bool IsNumber();
    bool IsString();
    bool IsUserdata();

    enum LuaType {
        Nil,
        Boolean,
        LightUserdata,
        Number,
        String,
        Table,
        Function,
        Userdata,
        Thread,
    };

    LuaType GetType(int index);
    const char *GetTypeName(int index);

public: // op table
    void GetTable(const std::string &table, const std::string &key,
            std::string &value);

    void SetTable(const std::string &table, const std::string &key,
            const std::string &value);

public: // debug
    void stackDump();

    lua_State *GetLPtr() const;

private:
    void CallLuaFunP(const std::string &fun_name, size_t n);

    template<typename T, typename... Ts>
    void CallLuaFunP(const std::string &fun_name, size_t n,
            T &&param, Ts&&... args) {

        push(std::forward<T>(param));
        ++n;
        CallLuaFunP(fun_name, n, std::forward<Ts>(args)...);
    }

private:
    std::shared_ptr<lua_State> lua_state_;

}; // class Lua

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

} // namespace lua

#endif // LUA_LUA_H
