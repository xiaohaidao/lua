// Copyright (C) 2022 All rights reserved.
// Email: oxox0@qq.com. Created in 202209

#ifndef LUA_ERROR_H
#define LUA_ERROR_H

#include <system_error>
 
namespace lua {

enum Error
{
  // no 0
};
 
} // namespace lua

namespace std {
  template <>
    struct is_error_code_enum<lua::Error> : true_type {};
} // namespace std
 
std::error_code make_error_code(lua::Error);

#endif // LUA_ERROR_H
