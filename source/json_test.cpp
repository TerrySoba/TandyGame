#include "unit_test.h"
#include "json.h"


TEST(JsonTest1)
{
    Json json = Json::fromFile("guy.jsn");
    JsonValue value = json.getRoot();
    ASSERT_FALSE(value.isInvalid());
    ASSERT_FALSE(value.isFalse());  
    ASSERT_FALSE(value.isTrue());   
    ASSERT_FALSE(value.isBool());   
    ASSERT_FALSE(value.isNull());   
    ASSERT_FALSE(value.isNumber()); 
    ASSERT_FALSE(value.isString()); 
    ASSERT_FALSE(value.isArray());  
    ASSERT_TRUE(value.isObject());
    ASSERT_FALSE(value.isRaw());  
}

TEST(JsonTest2)
{
    Json json = Json::fromString("12.5");
    JsonValue value = json.getRoot();
    ASSERT_FALSE(value.isInvalid());
    ASSERT_FALSE(value.isFalse());  
    ASSERT_FALSE(value.isTrue());   
    ASSERT_FALSE(value.isBool());   
    ASSERT_FALSE(value.isNull());   
    ASSERT_TRUE(value.isNumber());
    ASSERT_FALSE(value.isString()); 
    ASSERT_FALSE(value.isArray());  
    ASSERT_FALSE(value.isObject());
    ASSERT_FALSE(value.isRaw());  
}

TEST(JsonTest3)
{
    Json json = Json::fromString("\"Hello\"");
    JsonValue value = json.getRoot();
    ASSERT_FALSE(value.isInvalid());
    ASSERT_FALSE(value.isFalse());  
    ASSERT_FALSE(value.isTrue());   
    ASSERT_FALSE(value.isBool());   
    ASSERT_FALSE(value.isNull());   
    ASSERT_FALSE(value.isNumber());
    ASSERT_TRUE(value.isString()); 
    ASSERT_FALSE(value.isArray());  
    ASSERT_FALSE(value.isObject());
    ASSERT_FALSE(value.isRaw());  
}

TEST(JsonTest4)
{
    Json json = Json::fromString("[1,2,3]");
    JsonValue value = json.getRoot();
    ASSERT_FALSE(value.isInvalid());
    ASSERT_FALSE(value.isFalse());  
    ASSERT_FALSE(value.isTrue());   
    ASSERT_FALSE(value.isBool());   
    ASSERT_FALSE(value.isNull());   
    ASSERT_FALSE(value.isNumber());
    ASSERT_FALSE(value.isString()); 
    ASSERT_TRUE(value.isArray());  
    ASSERT_FALSE(value.isObject());
    ASSERT_FALSE(value.isRaw());  
}





