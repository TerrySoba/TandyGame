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

Json::Json(FILE* fp)
{
    if (!fp)
    {
        throw std::runtime_error("Could not open file.");
    }

    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    std::vector<char> buffer(fileSize + 1);
    fread(&buffer[0], fileSize, 1, fp);

    jsonRoot = cJSON_Parse(&buffer[0]);
}

JsonValue Json::getRoot()
{
    return JsonValue(jsonRoot);
}



JsonValue::JsonValue(cJSON* data) :
    m_data(data)
{
}

int JsonValue::size()
{
    if (isArray() || isObject())
        return cJSON_GetArraySize(m_data);
    else
        return 0;
}

JsonValue JsonValue::at(int pos)
{
    cJSON* res = cJSON_GetArrayItem(m_data, pos);
    if (!res)
    {
        throw std::runtime_error("Index out of range.");
    }
    return JsonValue(res);
}

JsonValue JsonValue::at(const char* name)
{
    cJSON* res = cJSON_GetObjectItemCaseSensitive(m_data, name);
    if (!res)
    {
        throw std::runtime_error("Invalid object key.");
    }
    return JsonValue(res);
}

std::string JsonValue::keyString()
{
    return m_data->string;
}

std::string JsonValue::toString()
{
    if (!isString())
    {
        throw std::runtime_error("Not a string.");
    }
    return m_data->valuestring;
}

int JsonValue::toInt()
{
    if (!isNumber())
    {
        throw std::runtime_error("Not a number.");
    }
    return m_data->valueint;
}

double JsonValue::toDouble()
{
    if (!isNumber())
    {
        throw std::runtime_error("Not a number.");
    }
    return m_data->valuedouble;
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
