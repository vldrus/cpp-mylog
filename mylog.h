/* Licensed under the MIT License */

#ifndef _MYLOG_H
#define _MYLOG_H 1

#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>

#if defined(_WIN32)
namespace mywin
{
    extern "C" __declspec(dllimport) unsigned long __stdcall GetCurrentThreadId();
}
#include <sys/timeb.h>
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

#if defined(_WIN32)
        ::timeb now_tb = ::timeb();
        ::ftime(&now_tb);
        ::localtime_s(&now_tm, &now_tb.time);
#else
        ::timespec now_ts = ::timespec();
        ::clock_gettime(CLOCK_REALTIME, &now_ts);
        ::localtime_r(&now_ts.tv_sec, &now_tm);
#endif

        char now_str[20];
        std::strftime(now_str, 20, "%Y-%m-%d %H:%M:%S", &now_tm);

        m_stream
            << now_str
            << '.'
            << std::setw(3) << std::setfill('0')
#if defined(_WIN32)
            << (now_tb.millitm)
#else
            << (now_ts.tv_nsec / 1000000)
#endif
            << ' '
#if defined(_WIN32)
            << mywin::GetCurrentThreadId()
            << ' ';
#elif defined(SYS_gettid)
            << ::syscall(SYS_gettid)
            << ' ';
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
