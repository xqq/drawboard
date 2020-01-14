//
// Created by zhengqian on 2020/01/14.
//

#ifndef DRAWBOARD_LOG_HPP
#define DRAWBOARD_LOG_HPP


class Log {
public:
    static void Info(const char* str);
    static void InfoF(const char* format, ...);
    static void Error(const char* str);
    static void ErrorF(const char* format, ...);
};


#endif //DRAWBOARD_LOG_HPP
