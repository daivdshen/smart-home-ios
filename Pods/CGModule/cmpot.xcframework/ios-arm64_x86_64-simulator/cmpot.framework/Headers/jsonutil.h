/**
 *
 * @file jsonutil.h
 * a rapidjson wrapper containig some shortcuts
 * Copyright (C) 2017 Comelit Group SpA
 *
 */

#ifndef JSONUTIL_H
#define JSONUTIL_H

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace Comelit  {
namespace MPot     {
namespace JSON     {

using std::string;

using JsonDocType       = rapidjson::Document;
using JsonType          = rapidjson::Value;
using JsonAllocatorType = rapidjson::Document::AllocatorType;

/**
 * @brief jsonContains
 * check if a generic json item is contained in json object
 * @param val json object
 * @param key json item key
 * @return true if key item is contained
 */
bool jsonContains(const JsonType &val, const string &key);

/**
 * @brief jsonSContains
 * check if a json string item is contained in json object
 * @param val json object
 * @param key json item key
 * @return true if key item is contained and is a string type
 */
bool jsonSContains(const JsonType &val, const string &key);

/**
 * @brief jsonIContains
 * check if a json number item is contained in json object
 * @param val json object
 * @param key json item key
 * @return true if key item is contained and is a signed integer type
 */
bool jsonIContains(const JsonType &val, const string &key);

/**
 * @brief jsonUIContains
 * check if a json number item is contained in json object
 * @param val json object
 * @param key json item key
 * @return true if key item is contained and is an unsigned integer type
 */
bool jsonUIContains(const JsonType &val, const string &key);

/**
 * @brief jsonI64Contains
 * check if a json number item is contained in json object
 * @param val json object
 * @param key json item key
 * @return true if key item is contained and is a 64 bit nsigned integer type
 */
bool jsonI64Contains(const JsonType &val, const string &key);

/**
 * @brief jsonUI64Contains
 * check if a json number item is contained in json object
 * @param val json object
 * @param key json item key
 * @return true if key item is contained and is a 64 bit unsigned integer type
 */
bool jsonUI64Contains(const JsonType &val, const string &key);

/**
 * @brief jsonBContains
 * check if a json number item is contained in json object
 * @param val json object
 * @param key json item key
 * @return true if key item is contained and is a boolean type
 */
bool jsonBContains(const JsonType &val, const string &key);

/**
 * @brief jsonSGet
 * retrieve a string Item from a json object
 * @param val json object
 * @param key json item key
 * @return the string value, or empty if key is not found
 */
string jsonSGet(const JsonType &val, const string &key);

/**
 * @brief jsonIGet
 * retrieve a number Item from a json object
 * @param val json object
 * @param key json item key
 * @return the number value, or -1 if key is not found
 */
int    jsonIGet(const JsonType &val, const string &key);

/**
 * @brief jsonUIGet
 * retrieve a unsigned integer Item from a json object
 * @param val json object
 * @param key json item key
 * @return the number value, or 0xFFFFFFFF if key is not found
 */
unsigned int jsonUIGet(const JsonType &val, const string &key);

/**
 * @brief jsonI64Get
 * retrieve a 64bit signed integer Item from a json object
 * @param val json object
 * @param key json item key
 * @return the number value, or -1 if key is not found
 */
int64_t jsonI64Get(const JsonType &val, const string &key);

/**
 * @brief jsonUI64Get
 * retrieve a 64bit unsigned integer Item from a json object
 * @param val json object
 * @param key json item key
 * @return the number value, or UINT64_MAX if key is not found
 */
uint64_t jsonUI64Get(const JsonType &val, const string &key);

/**
 * @brief jsonBGet
 * retrieve a boolean Item from a json object
 * @param val json object
 * @param key json item key
 * @return the number value, or false if key is not found
 */
bool   jsonBGet(const JsonType &val, const string &key);

/**
 * @brief jsonVGet
 * retrieve a generic json Item from a json object
 * @param val json object
 * @param key json item key
 * @return the json value, or null value if key is not found
 */
JsonType jsonVGet(JsonType &val, const string &key);

/**
 * @brief jsonIGet
 * retrieve value of a json Number
 * @param val json value
 * @return the number value or -1 if json value is not a number
 */
int    jsonIGet(const JsonType &val);

/**
 * @brief jsonUIGet
 * retrieve value of a json unsigned Integer number
 * @param val json value
 * @return the number value or 0xFFFFFFFF if json value is not a number
 */
unsigned int jsonUIGet(const JsonType &val);

/**
 * @brief jsonI64Get
 * retrieve value of a json 64bit signed Integer number
 * @param val json value
 * @return the number value or -1 if json value is not a number
 */
int64_t jsonI64Get(const JsonType &val);

/**
 * @brief jsonUI64Get
 * retrieve value of a json 64bit unsigned Integer number
 * @param val json value
 * @return the number value or UINT64_MAX if json value is not a number
 */
uint64_t jsonUI64Get(const JsonType &val);

/**
 * @brief jsonSGet
 * retrieve value of a json String
 * @param val json value
 * @return the string value or empty if json value is not a string
 */
string jsonSGet(const JsonType &val);

/**
 * @brief jsonDGet
 * retrieve value of a json double
 * @param val json value
 * @return the double value or -1 if json value is not a double
 */
double jsonDGet(const JsonType &val);

/**
 * @brief jsonBGet
 * retrieve value of a json bool
 * @param val json value
 * @return the bool value or false if json value is not a bool
 */
bool   jsonBGet(const JsonType &val);

/**
 * @brief jsonToString
 * serialize a generic json value into a string
 * @param val json value
 * @return serialized string
 */
string jsonToString(const JsonType &val);

/**
 * Add string Member in json object given key and value and a json allocator
 * @param val json object where to add the element
 * @param key json key where to add the element
 * @param value string value
 * @param allocator explicit allocator to be used
 */
void   jsonAddMember(JsonType &val, const string &key, const string &value,
                     JsonAllocatorType &alloc);

/**
 * Add string Member in json object given key and value and a json allocator
 * @param val json object where to add the element
 * @param key json key where to add the element
 * @param value string value
 * @param allocator explicit allocator to be used
 */
void   jsonAddMember(JsonType &val, const string &key, const char *value,
                     JsonAllocatorType &alloc);

/**
 * Add boolean Member in json object given key and value and a json allocator
 * @param val json object where to add the element
 * @param key json key where to add the element
 * @param value boolean value
 * @param allocator explicit allocator to be used
 */
void   jsonAddMember(JsonType &val, const string &key, const bool value,
                     JsonAllocatorType &alloc);

/**
 * Add number Member in json object given key and value and a json allocator
 * @param val json object where to add the element
 * @param key json key where to add the element
 * @param value number value
 * @param allocator explicit allocator to be used
 */
void   jsonAddMember(JsonType &val, const string &key, const int value,
                     JsonAllocatorType &alloc);

/**
 * Add unsgined integer Member in json object given key and value and a json allocator
 * @param val json object where to add the element
 * @param key json key where to add the element
 * @param value number value
 * @param allocator explicit allocator to be used
 */
void   jsonAddMember(JsonType &val, const string &key, const unsigned int value,
                     JsonAllocatorType &alloc);

/**
 * Add Json Member in json object given key and value and a json allocator
 * @param val json object where to add the element
 * @param key json key where to add the element
 * @param value Json value
 * @param allocator explicit allocator to be used
 */
void   jsonAddMember(JsonType &val, const string &key, JsonType &value,
                     JsonAllocatorType &alloc);

/**
 * @brief jsonAddMember
 * Add string Member in json Document given key and value
 * @param val json document where to add the element
 * @param key json key where to add the element
 * @param value string value
 */
void   jsonAddMember(JsonDocType &val, const string &key, const string &value);

/**
 * @brief jsonAddMember
 * Add string Member in json Document given key and value
 * @param val json document where to add the element
 * @param key json key where to add the element
 * @param value string value
 */
void   jsonAddMember(JsonDocType &val, const string &key, const char *value);

/**
 * @brief jsonAddMember
 * Add boolean Member in json Document given key and value
 * @param val json document where to add the element
 * @param key json key where to add the element
 * @param value boolean value
 */
void   jsonAddMember(JsonDocType &val, const string &key, const bool value);

/**
 * @brief jsonAddMember
 * Add number Member in json Document given key and value
 * @param val json document where to add the element
 * @param key json key where to add the element
 * @param value number value
 */
void   jsonAddMember(JsonDocType &val, const string &key, const int value);

/**
 * @brief jsonAddMember
 * Add unsigned integer Member in json Document given key and value
 * @param val json document where to add the element
 * @param key json key where to add the element
 * @param value number value
 */
void   jsonAddMember(JsonDocType &val, const string &key, const unsigned int value);

/**
 * @brief jsonAddMember
 * Add json Member in json Document given key and value
 * @param val json document where to add the element
 * @param key json key where to add the element
 * @param value json value
 */
void   jsonAddMember(JsonDocType &val, const string &key, JsonType &value);

/**
 * @brief jsonElemToArray
 * Add number Member in json array given key and value and a json allocator
 * @param alloc explicit allocator to be used
 * @param valJ destination array
 * @param value number value
 */
void   jsonElemToArray(JsonAllocatorType &alloc, JsonType &valJ,
                       int value);

/**
 * @brief jsonElemToArray
 * Add double Member in json array given key and value and a json allocator
 * @param alloc explicit allocator to be used
 * @param valJ destination array
 * @param value double value
 */
void   jsonElemToArray(JsonAllocatorType &alloc, JsonType &valJ,
                       double value);

/**
 * @brief jsonElemToArray
 * Add string Member in json array given key and value and a json allocator
 * @param alloc explicit allocator to be used
 * @param valJ destination array
 * @param value string value
 */
void   jsonElemToArray(JsonAllocatorType &alloc, JsonType &valJ,
                       const string &value);

/**
 * @brief jsonElemToArray
 * Add json Member in json array given key and value and a json allocator
 * @param alloc explicit allocator to be used
 * @param valJ destination array
 * @param value json value
 */
void   jsonElemToArray(JsonAllocatorType &alloc, JsonType &valJ,
                       JsonType &value);

/**
 * @brief jsonModifyMember
 * modify value of a member at a given key with a new number value
 * @param val json object where to modify the element
 * @param key json key to modify
 * @param value new number value
 */
void   jsonModifyMember(JsonType &val, const string &key, const int value);

/**
 * @brief jsonAddValue
 * add a Json value to a Json Document, at a given key via deep copy
 * @param d destination document
 * @param key key where to add the json value
 * @param value json value to be copied
 */
void   jsonAddValue(JsonDocType &d, const string &key, const JsonType &value);

/**
 * @brief jsonDGet
 * get a double value from a json object value at the given jey
 * @param val json object value
 * @param key
 * @return the value if the corresponding key exists and is a double value
 */
double jsonDGet(const JsonType &val, const std::string &key);

/**
 * @brief jsonIsI
 * @param val
 * @return true if the Json Generic value is a number
 */
bool jsonIsI(const JsonType &val);

/**
 * @brief jsonIsS
 * @param val
 * @return true if the Json Generic value is a string
 */
bool jsonIsS(const JsonType &val);

/**
 * @brief jsonIsD
 * @param val
 * @return true if the Json Generic value is a double
 */
bool jsonIsD(const JsonType &val);

/**
 * @brief jsonIsB
 * @param val
 * @return true if the Json Generic value is a boolean
 */
bool jsonIsB(const JsonType &val);

/**
 * @brief jsonIsA
 * @param val
 * @return true if the Json Generic value is a json array
 */
bool jsonIsA(const JsonType &val);

/**
 * @brief jsonIsO
 * @param val
 * @return true if the Json Generic value is a json object
 */
bool jsonIsO(const JsonType &val);


/**
 * @brief jsonDContains
 * check if a json floating-point item is contained in json object
 * @param val json object
 * @param key json item key
 * @return true if key item is contained
 */
bool jsonDContains(const JsonType &val, const std::string &key);

/**
 * @brief jsonIContains
 * check if a json object item is contained in json object
 * @param val json object
 * @param key json item key
 * @return true if key item is contained
 */
bool jsonOContains(const JsonType &val, const std::string &key);

/**
 * @brief jsonIContains
 * check if a json array item is contained in json object
 * @param val json object
 * @param key json item key
 * @return true if key item is contained
 */
bool jsonAContains(const JsonType &val, const std::string &key);

/**
 * @brief jsonGet
 * retireve the reference to a generic Json Item in a json object (read only)
 * @param val json object
 * @param key json item key
 * @return
 */
const JsonType &jsonGet(const JsonType &val, const std::string &key);

/**
 * @brief jsonAt
 * retireve the reference to a generic Json Item in a json object (read/write)
 * @param val json object
 * @param key json item key
 * @return
 */
JsonType &jsonAt(JsonType &val, const std::string &key);

/**
 * @brief getJsonTypeStr
 * get the type name of a generic json object
 * @param val json object
 * @return the type name (i.e. number/string/object..)
 */
std::string getJsonTypeStr(const JsonType &val);

/**
 * @brief jsonArraySize
 * @param arr
 * @return the size of a json array
 */
size_t jsonArraySize(const JsonType &arr);

/**
 * @brief jsonArrayItemGet
 * get the reference to an item inside a json array
 * @param arr json array
 * @param index item index
 * @return
 */
const JsonType &jsonArrayItemGet(const JsonType &arr, size_t index);

/**
 * @brief parseJsonBuffer
 * parse a Json formatted buffer into a Json object
 * parse result can be checked with jsonHasParsingErrors() function
 * @param buf buffer
 * @param len buffer lenght
 * @return
 */
JsonDocType parseJsonBuffer(char *buf, size_t len);

/**
 * @brief parseJsonString
 * parse a string into a json object
 * parse result can be checked with jsonHasParsingErrors() function
 * @param in string to be parsed
 * @return the Json object
 */
JsonDocType parseJsonString(const std::string &in);

/**
 * @brief parseJsonFile
 * parse a Json formatted file into a Json object
 * parse result can be checked with jsonHasParsingErrors() function
 * @param file_path file path
 * @return the json document
 */
JsonDocType parseJsonFile(const std::string &file_path);

/**
 * @brief jsonHasParsingErrors
 * check if a json document have any parsing error
 * @param in json document
 * @return true if the document has parsing errors
 */
bool jsonHasParsingErrors(const JsonDocType &in);

/**
 * @brief jsonAddArray
 * add an empty array to the json document at a given key
 * @param d json document
 * @param key key where to add the empty array
 */
void jsonAddArray(JsonDocType &d, const std::string &key);

/**
 * @brief jsonAddArray
 * add an empty array to the json value at a given key
 * @param v json value
 * @param key key where to add the empty array
 * @param alloc json allocator
 */
void jsonAddArray(JsonType &v, const std::string &key, JsonAllocatorType &alloc);

/**
 * @brief jsonAddObject
 * add an empty object to the json document at a given key
 * @param d json document
 * @param key key where to add the empty object
 */
void jsonAddObject(JsonDocType &d, const std::string &key);

/**
 * @brief jsonAddObject
 * add an empty object to the json value at a given key
 * @param v json value
 * @param key key where to add the empty object
 * @param alloc json allocator
 */
void jsonAddObject(JsonType &v, const std::string &key, JsonAllocatorType &alloc);

/**
 * @brief jsonAddNull
 * add an null value to the json value at a given key
 * @param v json value
 * @param key key where to add the null value
 * @param alloc json allocator
 */
void jsonAddNull(JsonType &v, const std::string &key, JsonAllocatorType &alloc);

/**
 * @brief createEmptyJson
 * create and empty Json object value
 * @return
 */
JsonType createEmptyJson();

/**
 * @brief createEmptyJson
 * create and empty Json object document
 * @return
 */
JsonDocType createEmptyJsonDoc();

/**
 * @brief createEmptyJsonDoc
 * create and empty Json object document
 * using an existing allocator
 * @param alloc json allocator to be used on the new document
 * @return
 */
JsonDocType createEmptyJsonDoc(JsonAllocatorType *alloc);

} // JSON     NS
} // MPot     NS
} // Comelit  NS

#endif // PARAM_H
