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
        std::time_t now = std::time(NULL);

        std::tm now_tm = std::tm();

#if defined(_WIN32) && defined(__BORLANDC__)
        ::localtime_s(&now, &now_tm);
#elif defined(_WIN32)
        ::localtime_s(&now_tm, &now);
#else
        ::localtime_r(&now, &now_tm);
#endif

        char datetime[20];
        std::strftime(datetime, 20, "%Y-%m-%d %H:%M:%S", &now_tm);

        const char space = ' ';

        m_stream << datetime << space;

#if defined(_WIN32)
        m_stream << mywin::GetCurrentThreadId() << space;
#elif defined(SYS_gettid)
        m_stream << ::syscall(SYS_gettid) << space;
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
