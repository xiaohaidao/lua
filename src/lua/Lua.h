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
    void runStr(const std::string &str);

    void runFile(const std::string &file);

    template<typename... Ts>
    void callLuaFun(const std::string &lua_fun_name, Ts&&... args) {
        getGlobalToTop(lua_fun_name);
        callLuaFunP(lua_fun_name, 0, std::forward<Ts>(args)...);
    }

public: // type definition
    typedef int (*lua_fnc) (lua_State *L);

public: // interaction: lua call c
    void registerFunc(lua_fnc func) {
        pushCclosureFunc(func, 0);
    }

    // @param upvalue_num it will pop the number stack to the up value
    void pushCclosureFunc(lua_fnc func, int upvalue_num);
    void getUpvalueToTop(uint8_t up_index);
    int numArguments();

// // cclosure
// lua_pashcclosure(L, c_fun, count);
// int c_fun(lua_State *L) {
//     lua_integer op = luaL_optinteger(L, 1, 0); // the paraemter count <= 256
//      lua_upvalueindex(index);
//      lua_copy(L, index, lua_upvalueindex(index)); // update
//
// }
//

public: // op stack
    void push(); // push nil
    void push(double value);
    void push(int64_t value);
    void push(bool value);
    void push(const std::string &str);
    void push(const char *s);
    void pushLightUserdata(void *user_data);
    void pushUserData();
    void pushTable();
    void pushCFunction(lua_fnc func);

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

    void getGlobalToTop(const std::string &var);
    void setTopToGlobal(const std::string &var); // top will be pop

public: // op type
    int64_t toType(int index, int64_t &re);
    double toType(int index, double &re);
    std::string toType(int index, std::string &re);
    bool toUserDataType(int index, void *user_data);
    bool toLightUserdataType(int index, void *user_data);

    template<typename T>
    T  toType(int index, T &re) {
        // lua_to*();
        if (std::is_same<typename std::decay<T>::type, int32_t>::value ||
                std::is_same<typename std::decay<T>::type, int16_t>::value ||
                std::is_same<typename std::decay<T>::type, int8_t>::value ||
                std::is_same<typename std::decay<T>::type, uint64_t>::value ||
                std::is_same<typename std::decay<T>::type, uint32_t>::value ||
                std::is_same<typename std::decay<T>::type, uint16_t>::value ||
                std::is_same<typename std::decay<T>::type, uint8_t>::value) {

            int64_t r;
            auto x = toType(index, r);
            re = static_cast<T>(r);
            return x;
        } else if (std::is_same<typename std::decay<T>::type, float>::value) {
            double r;
            auto x = toType(index, r);
            re = static_cast<T>(r);
            return x;
        } else {
            LOG_ERROR("To type error");
        }
    }

public: // check type
    bool isNil(int stack_index);
    bool isBoolean(int stack_index);
    bool isInteger(int stack_index);
    bool isNumber(int stack_index);
    bool isString(int stack_index);
    bool isTable(int stack_index);
    bool isCFuntion(int stack_index);
    bool isUserdata(int stack_index);
    bool isLightUserdata(int stack_index);
    bool isTHread(int stack_index);

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

    LuaType getType(int index);
    const char *getTypeName(int index);

public: // op table

    // it will not trigger meta method
    void getTableKeyValueToTop(int table_stack_index);
    // it will trigger meta method
    void getTableKeyToTop(int table_stack_index, const std::string &key);
    // it will pop top value
    // it will trigger meta method
    void setTopValueToTable(int table_stack_index, const std::string &key);
    void setTopKeyValueToTable(int table_stack_index);

    // the following function is operator integer table,  lua integer index
    // start at 1

    // this is the result of the length operator("#")
    int tableLen(int table_stack_index);
    void getTableKeyToTop(int table_stack_index, int key);
    // it will pop top value
    void setTopValueToTable(int table_stack_index, int key);

    template <typename T>
    void pushTableVector(const std::vector<T> &vec)
    {
        if (new_table) lua_newtable(lua);
        size_t size = value.size();
        for (size_t i = 0; i < size; ++i)
        {
            lua_pushinteger(lua, static_cast<lua_Integer>(value[i]));
            push(value[i]);
            lua_seti(lua, -2, i + 1);
        }
    }

public: // debug
    void stackDump();

    lua_State *getLPtr() const;

private:
    void callLuaFunP(const std::string &fun_name, size_t n);

    template<typename T, typename... Ts>
    void callLuaFunP(const std::string &fun_name, size_t n,
            T &&param, Ts&&... args) {

        push(std::forward<T>(param));
        ++n;
        callLuaFunP(fun_name, n, std::forward<Ts>(args)...);
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
