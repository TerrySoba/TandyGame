#ifndef _TINY_STRING_H
#define _TINY_STRING_H

#include "malloc.h"
#include "string.h"

class TinyString
{
public:
    TinyString()
    {
        m_data = NULL;
    }

    TinyString(const char* str)
    {
        m_data = strdup(str);
    }

    TinyString(const TinyString& other)
    {
        m_data = strdup(other.m_data);
    }

    bool operator==(const char* str)
    {
        return strcmp(str, m_data) == 0;
    }

    void operator=(const char* str)
    {
        free((void*)m_data);
        m_data = strdup(str);
    }

    int size() const
    {
        return strlen(m_data);
    }

    const char* c_str() const
    {
        return m_data;
    }

    ~TinyString()
    {
        free((void*)m_data);
    }

private:
    char* m_data;
};

#endif