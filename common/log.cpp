//
// Created by zhengqian on 2020/01/14.
//

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include "log.hpp"

void Log::Info(const char* str) {
    fprintf(stdout, "%s\n", str);
}

void Log::InfoF(const char* format, ...) {
    va_list arglist;
    va_start(arglist, format);
    vfprintf(stdout, format, arglist);
    va_end(arglist);
}

void Log::Error(const char* str) {
    fprintf(stderr, "%s\n", str);
}

void Log::ErrorF(const char* format, ...) {
    va_list arglist;
    va_start(arglist, format);
    vfprintf(stderr, format, arglist);
    va_end(arglist);
}
