// Copyright (C) 2022 All rights reserved.
// Email: oxox0@qq.com. Created in 202206

#include "lua/Lua.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace lua {

Lua::Lua(lua_State *lua) {
    lua_state_ = std::shared_ptr<lua_State>(luaL_newstate(),
            [](lua_State *p) {});
}

Lua::Lua(bool use_standard_lib) {
    LOG_TRACE("new lua_state\n");
    lua_state_ = std::shared_ptr<lua_State>(luaL_newstate(),
            [](lua_State *p) {
            lua_close(p);
            LOG_TRACE("destroy lua_state\n");
            });

    if (use_standard_lib) { // use lua standard library
        luaL_openlibs(GetLPtr());
    }

}

void Lua::runStr(const std::string &str) {
    if (luaL_loadstring(GetLPtr(), str.c_str()) || lua_pcall(GetLPtr(), 0, 0, 0)) {
        LOG_ERROR("Run string \"%s\", error : %s\n",
                str.c_str(),  lua_tostring(GetLPtr(), -1));

        lua_pop(GetLPtr(), 1);
    }
}

void Lua::runFile(const std::string &file) {
    if (luaL_loadfile(GetLPtr(), file.c_str()) || lua_pcall(GetLPtr(), 0, 0, 0)) {
        LOG_ERROR("Run file \"%s\", error : %s\n",
                file.c_str(),  lua_tostring(GetLPtr(), -1));

        lua_pop(GetLPtr(), 1);
    }
}

void Lua::push() {
    lua_pushnil(GetLPtr());
}

void Lua::push(double value) {
    lua_pushnumber(GetLPtr(), value);
}

void Lua::push(int64_t value) {
    lua_pushinteger(GetLPtr(), value);
}

void Lua::push(bool value) {
    lua_pushboolean(GetLPtr(), value ? 1 : 0);
}

void Lua::push(const std::string &str) {
    lua_pushlstring(GetLPtr(), str.c_str(), str.size());
}

void Lua::push(const char *s) {
    lua_pushstring(GetLPtr(), s);
}

void Lua::copyToTop(int index) {
    lua_pushvalue(GetLPtr(), index);
}

void Lua::replaceTopTo(int index) {
    lua_replace(GetLPtr(), index);
}

void Lua::resizeStack(int size) {
    lua_settop(GetLPtr(), size);
}

void Lua::rotateStack(int index, int step) {
    lua_rotate(GetLPtr(), index, step);
}

void Lua::removeStack(int index) {
    lua_remove(GetLPtr(), index);
}

void Lua::pop(size_t count) {
    lua_pop(GetLPtr(), count);
}

void Lua::getGlobalToTop(const std::string &var) {
    lua_getglobal(GetLPtr(), var.c_str());
}

int64_t Lua::toType(int index, int64_t &re) {
    re = static_cast<int64_t>(luaL_checkinteger(GetLPtr(), index));
    return re;
}

double Lua::toType(int index, double &re) {
    re = static_cast<double>(luaL_checknumber(GetLPtr(), index));
    return re;
}

std::string Lua::toType(int index, std::string &re) {
    size_t size;
    const char *c = luaL_checklstring(GetLPtr(), index, &size);
    re.assign(c, size);
    return re;
}

void Lua::getTable(const std::string &table, const std::string &key,
        std::string &value) {

    GetGlobalToTop(table);
    push(key);
    // lua_getfield(GetLPtr(), -1, key.c_str()) == lua_type
    lua_gettable(GetLPtr(), -2);
    size_t len;
    const char *v = lua_tolstring(GetLPtr(), -1, &len);
    value.assign(v, len);
    pop(); // pop value
    pop(); // pop table
}

void Lua::setTable(const std::string &table, const std::string &key,
        const std::string &value) {

    GetGlobalToTop(table);
    push(key);
    push(value);
    // lua_setfield(GetLPtr(), -2, key.c_str()); // don't push key
    lua_settable(GetLPtr(), -3); // pop the key and value
    pop(); // pop table
}

void Lua::stackDump() {
    int top = lua_gettop(GetLPtr());
    for (int i = 0; i <= top; ++i) {
        int t = lua_type(GetLPtr(), i);
        switch (t) {
            case LUA_TSTRING:
                LOG_TRACE("'%s'", lua_tostring(GetLPtr(), i));
                break;
            case LUA_TBOOLEAN:
                LOG_TRACE(lua_toboolean(GetLPtr(), i) ? "true" : "false");
                break;
            case LUA_TNUMBER:
                LOG_TRACE("'%g'", lua_tonumber(GetLPtr(), i));
                break;
            default:
                LOG_TRACE("%s", lua_typename(GetLPtr(), t));
                break;
        }
        LOG_TRACE("  ");
    }
    LOG_TRACE("\n");
}

void Lua::callLuaFunP(const std::string &fun_name, size_t n) {
    stackDump();
    LOG_TRACE("function parameter count: %d\n", n);
    int return_count = 0;
    if (lua_pcall(GetLPtr(), n, return_count, 0)) {
        LOG_ERROR("Run function \"%s\", error : %s\n",
                fun_name.c_str(),  lua_tostring(GetLPtr(), -1));

        lua_pop(GetLPtr(), 1);
    }
    //value = pop(); // get return value
}

lua_State *Lua::getLPtr() const {
    return lua_state_.get();
}

} // namespace lua

