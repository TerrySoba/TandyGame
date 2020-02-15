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
    void operator=(const TinyString& other);

    int size() const;
    const char* c_str() const;
    
private:
    char* m_data;
};

TinyString operator+(const TinyString& a, const TinyString& b);
TinyString operator+(const TinyString& a, const char* b);
TinyString operator+(const char* a, const TinyString& b);


#endif