#ifndef _MYLOG_H
#define _MYLOG_H

#include <ctime>
#include <sstream>
#include <iostream>

#if !defined(_WIN32)
#include <unistd.h>
#include <sys/syscall.h>
#endif

class myout
{
public:
    ~myout() { std::cout << m_stream.str() + "\n"; }

    std::ostringstream &stream() { return m_stream; }

private:
    std::ostringstream m_stream;
};

class mylog
{
public:
    mylog()
    {
        std::time_t now = std::time(NULL);

        std::tm now_tm = std::tm();

#if defined(_WIN32)
        ::localtime_s(&now_tm, &now);
#else
        ::localtime_r(&now, &now_tm);
#endif

        char datetime[20];
        std::strftime(datetime, 20, "%Y-%m-%d %H:%M:%S", &now_tm);

        m_stream << datetime << " ";

#if defined(SYS_gettid)
        m_stream << ::syscall(SYS_gettid) << " ";
#endif
    }

    ~mylog() { std::cout << m_stream.str() + "\n"; }

    std::ostringstream &stream() { return m_stream; }

private:
    std::ostringstream m_stream;
};

#define myout myout().stream()
#define mylog mylog().stream()

#endif
