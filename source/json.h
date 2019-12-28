#ifndef JSON_H_INCLUDED
#define JSON_H_INCLUDED

#include <stdio.h>
#include <string>

// forward declare
struct cJSON;

class JsonValue
{
public:
    JsonValue(cJSON* data);

    bool isInvalid();
    bool isFalse();  
    bool isTrue();   
    bool isBool();   
    bool isNull();   
    bool isNumber(); 
    bool isString(); 
    bool isArray();  
    bool isObject(); 
    bool isRaw();

    int size();
    JsonValue at(int pos);
    JsonValue at(const char* name);

    std::string keyString();
    std::string toString();
    int toInt();
    double toDouble();
    
private:
    cJSON* m_data;
};

class Json
{
public:
    Json(const char* data);
    Json(FILE* fp);
    ~Json();

    JsonValue getRoot();
    
private:
    Json(const Json&);
    Json& operator=(const Json&);

private:
    cJSON* jsonRoot;
};

#endif