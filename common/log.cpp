//
// Created by zhengqian on 2020/01/14.
//

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include "log.hpp"

void Log::Info(const char* str) {
    fprintf(stdout, "%s\n", str);
#ifdef _WIN32
    fflush(stdout);
#endif
}

void Log::InfoF(const char* format, ...) {
    va_list arglist;
    va_start(arglist, format);
    vfprintf(stdout, format, arglist);
    va_end(arglist);
#ifdef _WIN32
    fflush(stdout);
#endif
}

void Log::Error(const char* str) {
    fprintf(stderr, "%s\n", str);
#ifdef _WIN32
    fflush(stderr);
#endif
}

void Log::ErrorF(const char* format, ...) {
    va_list arglist;
    va_start(arglist, format);
    vfprintf(stderr, format, arglist);
    va_end(arglist);
#ifdef _WIN32
    fflush(stderr);
#endif
}
