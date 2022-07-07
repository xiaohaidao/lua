// Copyright (C) 2022 All rights reserved.
// Email: oxox0@qq.com. Created in 202206

#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <stdio.h>

#define LOG_TRACE(...) fprintf(stdout, __VA_ARGS__)
#define LOG_ERROR(...) fprintf(stderr, __VA_ARGS__)

#endif // UTILS_LOG_H

