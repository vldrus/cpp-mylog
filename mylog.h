/* Licensed under the MIT License */

#ifndef _MYLOG_H
#define _MYLOG_H 1

#include <ctime>
#include <sstream>
#include <iostream>

#if defined(_WIN32)
namespace mywin
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
        std::cout << m_stream.str() + '\n';
    }

    std::ostringstream &stream()
    {
        return m_stream;
    }

private:
    std::ostringstream m_stream;
};

/* Internal class for mylog macros */
class mylog
{
public:
    mylog()
    {
        std::tm now_tm = std::tm();
        std::time_t now_time = std::time_t();

        std::time(&now_time);

#if defined(_WIN32)
        ::localtime_s(&now_tm, &now_time);
#else
        ::localtime_r(&now_time, &now_tm);
#endif

        char now_str[20];
        std::strftime(now_str, 20, "%Y-%m-%d %H:%M:%S", &now_tm);

        m_stream
            << now_str
            << ' '
#if defined(_WIN32)
            << mywin::GetCurrentThreadId() << ' ';
#elif defined(SYS_gettid)
            << ::syscall(SYS_gettid) << ' ';
#endif
    }

    ~mylog()
    {
        std::cerr << m_stream.str() + '\n';
    }

    std::ostringstream &stream()
    {
        return m_stream;
    }

private:
    std::ostringstream m_stream;
};

/* Print to stdout with a newline. Usage: myout << "Hello"; */
#define myout myout().stream()

/* Log to stderr with date & time. Usage: mylog << "Hello"; */
#define mylog mylog().stream()

#endif /* _MYLOG_H */
