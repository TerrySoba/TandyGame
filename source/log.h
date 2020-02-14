#ifndef _LOG_INCLUDED
#define _LOG_INCLUDED

#include <stdio.h>

#define LOG_ENTRY(message) \
    { \
        FILE* fp = fopen("logfile.txt", "ab"); \
        fprintf(fp, message ## " (" ## __FILE__ ## ":%d)\n", __LINE__); \
        fclose(fp); \
    }


#endif