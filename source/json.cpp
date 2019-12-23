#include "json.h"

#include "3rd_party/cJSON/cJSON.h"


#include <stdio.h>

#include <stdexcept>
#include <vector>

Json::Json(const char* data)
{
    if (!data)
    {
        throw std::runtime_error("Invalid data.");
    }
    jsonRoot = cJSON_Parse(data);
}

Json::~Json()
{
    if (jsonRoot) cJSON_Delete(jsonRoot);
}

Json Json::fromFile(const char* filename)
{
    FILE* fp = fopen(filename, "rb");
    if (!fp)
    {
        throw std::runtime_error("Could not open file.");
    }

    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    std::vector<char> buffer(fileSize + 1);
    fread(&buffer[0], fileSize, 1, fp);
    fclose(fp);

    return Json(&buffer[0]);
}

Json Json::fromString(const char* data)
{
    return Json(data);
}

JsonValue Json::getRoot()
{
    return JsonValue(jsonRoot);
}



JsonValue::JsonValue(cJSON* data) :
    m_data(data)
{
}

bool JsonValue::isInvalid()
{
    return cJSON_IsInvalid(m_data);
}

bool JsonValue::isFalse()
{
    return cJSON_IsFalse(m_data);
}

bool JsonValue::isTrue()
{
    return cJSON_IsTrue(m_data);
}

bool JsonValue::isBool()
{
    return cJSON_IsBool(m_data);
}

bool JsonValue::isNull()
{
    return cJSON_IsNull(m_data);
}

bool JsonValue::isNumber()
{
    return cJSON_IsNumber(m_data);
}

bool JsonValue::isString()
{
    return cJSON_IsString(m_data);
}

bool JsonValue::isArray()
{
    return cJSON_IsArray(m_data);
}

bool JsonValue::isObject()
{
    return cJSON_IsObject(m_data);
}

bool JsonValue::isRaw()
{
    return cJSON_IsRaw(m_data);
}
