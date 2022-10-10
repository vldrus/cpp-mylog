/* Licensed under the MIT License */

#pragma once

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
        std::strftime(datetime, 20, "%Y-%m-%d %H:%M:S", &now_tm);

        m_oss << datetime << " ";

#if defined(_WIN32)
        m_oss << mywin::GetCurrentThreadId() << " ";
#elif defined(SYS_gettid)
        m_oss << ::syscall(SYS_gettid) << " ";
#endif
    }

    ~mylog()
    {
        std::cerr << m_oss.str() + '\n';
    }

    std::ostringstream &stream()
    {
        return m_oss;
    }

private:
    std::ostringstream m_oss;
};

#define mylog mylog().stream()
