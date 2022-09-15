/* Licensed under the MIT License */

#ifndef MYLOG_H
#define MYLOG_H

#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>

#if defined(_WIN32)
namespace mylog_w
{
    extern "C" __declspec(dllimport) unsigned long __stdcall GetCurrentThreadId();
}
#else
#include <unistd.h>
#include <sys/syscall.h>
#endif

/* Internal class for myout macros */
class myout
{
public:
    ~myout()
    {
        std::cout << oss.str() + '\n';
    }

    std::ostringstream &stream()
    {
        return oss;
    }

private:
    std::ostringstream oss;
};

/* Internal class for myerr macros */
class myerr
{
public:
    ~myerr()
    {
        std::cerr << oss.str() + '\n';
    }

    std::ostringstream &stream()
    {
        return oss;
    }

private:
    std::ostringstream oss;
};

/* Internal class for mylog macros */
class mylog
{
public:
    mylog()
    {
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        auto now_millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

        auto now_tm = std::tm();

#if defined(_WIN32)
        ::localtime_s(&now_tm, &now_time);
#else
        ::localtime_r(&now_time, &now_tm);
#endif

#if defined(_WIN32)
        auto thread_id = mylog_w::GetCurrentThreadId();
#else
        auto thread_id = ::syscall(SYS_gettid);
#endif

        oss << std::put_time(&now_tm, "%Y-%m-%d")
            << " "
            << std::put_time(&now_tm, "%H:%M:%S")
            << "."
            << std::setw(3) << std::setfill('0') << (now_millis % 1000)
            << " [ "
            << thread_id
            << " ] ";
    }

    ~mylog()
    {
        std::cerr << oss.str() + '\n';
    }

    std::ostringstream &stream()
    {
        return oss;
    }

private:
    std::ostringstream oss;
};

/* Print to stdout with a newline. Usage: myout << "Hello"; */
#define myout myout().stream()

/* Print to stderr with a newline. Usage: myerr << "Hello"; */
#define myerr myerr().stream()

/* Log to stderr with date & time. Usage: mylog << "Hello"; */
#define mylog mylog().stream()

#endif /* MYLOG_H */
