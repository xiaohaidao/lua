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
    lua_state_ = std::shared_ptr<lua_State>(lua,
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
        luaL_openlibs(getLPtr());
    }

}

void Lua::runStr(const std::string &str) {
    if (luaL_loadstring(getLPtr(), str.c_str()) || lua_pcall(getLPtr(), 0, 0, 0)) {
        LOG_ERROR("Run string \"%s\", error : %s\n",
                str.c_str(),  lua_tostring(getLPtr(), -1));

        lua_pop(getLPtr(), 1);
    }
}

void Lua::runFile(const std::string &file) {
    if (luaL_loadfile(getLPtr(), file.c_str()) || lua_pcall(getLPtr(), 0, 0, 0)) {
        LOG_ERROR("Run file \"%s\", error : %s\n",
                file.c_str(),  lua_tostring(getLPtr(), -1));

        lua_pop(getLPtr(), 1);
    }
}

void Lua::pushCclosureFunc(lua_fnc func, int upvalue_num) {
    lua_pushcclosureu(getLPtr(), func, upvalue_num);
}

void Lua::getUpvalueToTop(uint8_t up_index) {
    lua_upvalueiindex(up_index);
}

int Lua::numArguments() {
    return lua_gettop(getLPtr());
}

void Lua::push() {
    lua_pushnil(getLPtr());
}

void Lua::push(double value) {
    lua_pushnumber(getLPtr(), value);
}

void Lua::push(int64_t value) {
    lua_pushinteger(getLPtr(), value);
}

void Lua::push(bool value) {
    lua_pushboolean(getLPtr(), value ? 1 : 0);
}

void Lua::push(const std::string &str) {
    lua_pushlstring(getLPtr(), str.c_str(), str.size());
}

void Lua::push(const char *s) {
    lua_pushstring(getLPtr(), s);
}

void Lua::copyToTop(int index) {
    lua_pushvalue(getLPtr(), index);
}

void Lua::replaceTopTo(int index) {
    lua_replace(getLPtr(), index);
}

void Lua::resizeStack(int size) {
    lua_settop(getLPtr(), size);
}

void Lua::rotateStack(int index, int step) {
    lua_rotate(getLPtr(), index, step);
}

void Lua::removeStack(int index) {
    lua_remove(getLPtr(), index);
}

void Lua::pop(size_t count) {
    lua_pop(getLPtr(), count);
}

void Lua::getGlobalToTop(const std::string &var) {
    lua_getglobal(getLPtr(), var.c_str());
}

int64_t Lua::toType(int index, int64_t &re) {
    re = static_cast<int64_t>(luaL_checkinteger(getLPtr(), index));
    return re;
}

double Lua::toType(int index, double &re) {
    re = static_cast<double>(luaL_checknumber(getLPtr(), index));
    return re;
}

std::string Lua::toType(int index, std::string &re) {
    size_t size;
    const char *c = luaL_checklstring(getLPtr(), index, &size);
    re.assign(c, size);
    return re;
}

bool Lua::isNil(int stack_index)
{
    return lua_isnil(getLPtr(), stack_index);
}

bool Lua::isBoolean(int stack_index)
{
    return lua_isboolean(getLPtr(), stack_index);
}

bool Lua::isInteger(int stack_index)
{
    return lua_isinteger(getLPtr(), stack_index);
}

bool Lua::isNumber(int stack_index)
{
    return lua_isnumber(getLPtr(), stack_index);
}

bool Lua::isString(int stack_index)
{
    return lua_isstring(getLPtr(), stack_index);
}

bool Lua::isTable(int stack_index)
{
    return lua_istable(getLPtr(), stack_index);
}

bool Lua::isCFuntion(int stack_index)
{
    return lua_iscfunction(getLPtr(), stack_index);
}

bool Lua::isUserdata(int stack_index)
{
    return lua_isuserdata(getLPtr(), stack_index);
}

bool Lua::isLightUserdata(int stack_index)
{
    return lua_islightuserdata(getLPtr(), stack_index);
}

bool Lua::isTHread(int stack_index)
{
    return lua_isthread(getLPtr(), stack_index);
}

void Lua::getTable(int table_stack_index, const std::string &key,
        std::string &value) {

    GetGlobalToTop(table);
    push(key);
    // lua_getfield(getLPtr(), -1, key.c_str()) == lua_type
    lua_gettable(getLPtr(), -2);
    size_t len;
    const char *v = lua_tolstring(getLPtr(), -1, &len);
    value.assign(v, len);
    pop(); // pop value
    pop(); // pop table
}

void Lua::getTableKeyValueToTop(int table_stack_index) {
    lua_rawget(getLPtr(), table_stack_index)
}

void Lua::getTableKeyToTop(int table_stack_index, const std::string &key) {
    lua_getfield(getLPtr(), table_stack_index, )
}

void Lua::setTopValueToTable(int table_stack_index, const std::string &key) {
     lua_setfield(getLPtr(), table_stack_index, key.c_str());
}

void Lua::setTopKeyValueToTable(int table_stack_index) {
    lua_rawset(getLPtr(), table_stack_index); // pop the key and value
}

int Lua::tableLen(int table_stack_index) {
     return lua_rawlen(getLPtr(), table_stack_index);
}

void Lua::getTableKeyToTop(int table_stack_index, int key) {
    lua_rawgeti(getLPtr(), table_stack_index, i);
}

void Lua::setTopValueToTable(int table_stack_index, int key) {
     lua_seti(getLPtr(), table_stack_index, key);
}

void Lua::stackDump() {
    int top = lua_gettop(getLPtr());
    for (int i = 0; i <= top; ++i) {
        int t = lua_type(getLPtr(), i);
        switch (t) {
            case LUA_TSTRING:
                LOG_TRACE("'%s'", lua_tostring(getLPtr(), i));
                break;
            case LUA_TBOOLEAN:
                LOG_TRACE(lua_toboolean(getLPtr(), i) ? "true" : "false");
                break;
            case LUA_TNUMBER:
                LOG_TRACE("'%g'", lua_tonumber(getLPtr(), i));
                break;
            default:
                LOG_TRACE("%s", lua_typename(getLPtr(), t));
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
    if (lua_pcall(getLPtr(), n, return_count, 0)) {
        LOG_ERROR("Run function \"%s\", error : %s\n",
                fun_name.c_str(),  lua_tostring(getLPtr(), -1));

        lua_pop(getLPtr(), 1);
    }
    //value = pop(); // get return value
}

lua_State *Lua::getLPtr() const {
    return lua_state_.get();
}

} // namespace lua

