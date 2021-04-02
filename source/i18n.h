#ifndef _I18N_INCLUDED
#define _I18N_INCLUDED

#include "tiny_string.h"
#include <stdint.h>
#include <stdio.h>

TinyString getI18NString(const char* path, uint16_t id);

struct StringEntry;

class I18N
{
public:
    I18N(const char* path);
    ~I18N();
    TinyString getString(uint16_t id);

private:
    uint16_t m_entryCount;
    StringEntry* m_entries;
    FILE* m_file;
};


#endif