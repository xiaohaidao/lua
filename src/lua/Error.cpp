// Copyright (C) 2022 All rights reserved.
// Email: oxox0@qq.com. Created in 202209

#include "lua/Error.h"
 
namespace lua {
namespace { // anonymous namespace
  
const struct ErrorCategory : std::error_category
{
  const char* name() const noexcept override;
  std::string message(int ev) const override;
} theErrCategory {};
  
const char* ErrorCategory::name() const noexcept
{
  return "LuaError";
}
  
std::string ErrorCategory::message(int ev) const
{
  switch (static_cast<Error>(ev))
  {
//   case FlightsErrc::NonexistentLocations:
//     return "nonexistent airport name in request";
  default:
    return "(unrecognized error)";
  }
}
  
// const ErrorCategory theErrCategory {};
  
} // anonymous namespace
} // namespace lua

std::error_code make_error_code(lua::Error e)
{
  return {static_cast<int>(e), theErrCategory};
}

