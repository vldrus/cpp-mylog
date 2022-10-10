/* Licensed under the MIT License */

#pragma once

#include <sstream>
#include <iostream>

class myout
{
public:
    ~myout()
    {
        std::cout << m_oss.str() + '\n';
    }

    std::ostringstream &stream()
    {
        return m_oss;
    }

private:
    std::ostringstream m_oss;
};

#define myout myout().stream()
