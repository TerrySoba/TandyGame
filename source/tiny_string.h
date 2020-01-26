#ifndef _TINY_STRING_H
#define _TINY_STRING_H

class TinyString
{
public:
    TinyString();
    TinyString(const char* str);
    TinyString(const TinyString& other);
    ~TinyString();

    bool operator==(const char* str);
    void operator=(const char* str);

    int size() const;
    const char* c_str() const;
    
private:
    char* m_data;
};

#endif