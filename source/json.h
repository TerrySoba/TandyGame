#ifndef JSON_H_INCLUDED
#define JSON_H_INCLUDED

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

private:
    cJSON* m_data;
};

class Json
{
public:
    static Json fromFile(const char* filename);
    static Json fromString(const char* data);

    JsonValue getRoot();
    ~Json();
private:

    Json(const char* data);
    
private:
    cJSON* jsonRoot;
};

#endif