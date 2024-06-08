#ifndef _XML_HELPFUNC_H
#define _XML_HELPFUNC_H
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <type_traits>
#include "../tinyxml/tinyxml2.h"
using namespace tinyxml2;
// help func

template <typename T>
bool isVector(const std::vector<T> &v)
{
    return true;
}
template <typename T>
bool isVector(const T &)
{
    return false;
}
template <typename K, typename V>
bool isMap(const std::map<K, V> &m)
{
    return true;
}
template <typename T>
bool isMap(const T &)
{
    return false;
}
template <typename T>
bool isList(const std::list<T> &l)
{
    return true;
}
template <typename T>
bool isList(const T &)
{
    return false;
}
template <typename T1, typename T2>
bool isPair(const std::pair<T1, T2> &p)
{
    return true;
}
template <typename T>
bool isPair(const T &)
{
    return false;
}
template <typename T>
bool specialType(T temp)
{
    return (isVector(temp) || isList(temp) || isPair(temp) || isMap(temp));
}
template <typename T>
XMLError readElementVal(T &object, XMLElement *element)
{
    if (element == nullptr)
    {
        std::cout << "Load Fails" << std::endl;
        return XML_ERROR_PARSING_ELEMENT;
    }
    if constexpr (std::is_same<T, int>::value)
    {
        return element->QueryIntAttribute("val", &object);
    }
    else if constexpr (std::is_same<T, float>::value)
    {
        return element->QueryFloatAttribute("val", &object);
    }
    else if constexpr (std::is_same<T, double>::value)
    {
        return element->QueryDoubleAttribute("val", &object);
    }
    else if constexpr (std::is_same<T, size_t>::value)
    {
        int64_t temp;
        XMLError eResult = element->QueryInt64Attribute("val", &temp);
        object = static_cast<size_t>(temp);
        return eResult;
    }
    else if constexpr (std::is_same<T, bool>::value)
    {
        return element->QueryBoolAttribute("val", &object);
    }
    else if constexpr (std::is_same<T, std::string>::value)
    {
        const char *temp = element->Attribute("val");
        if (temp != nullptr)
        {
            object = std::string(temp);
            return XML_SUCCESS;
        }
        else
        {
            return XML_NO_ATTRIBUTE;
        }
    }
    else if constexpr (std::is_same<T, char>::value)
    {
        const char *temp = element->Attribute("val");
        if (temp != nullptr && strlen(temp) == 1)
        {
            object = temp[0];
            return XML_SUCCESS;
        }
        else
        {
            return XML_NO_ATTRIBUTE;
        }
    }
    return XML_ERROR_PARSING_ELEMENT;
}

template <typename T>
const char *convertTypeName()
{
    T temp;
    if constexpr (std::is_same<T, int>::value)
        return "int";
    else if constexpr (std::is_same<T, char>::value)
        return "char";
    else if constexpr (std::is_same<T, size_t>::value)
        return "size_t";
    else if constexpr (std::is_same<T, float>::value)
        return "float";
    else if constexpr (std::is_same<T, double>::value)
        return "double";
    else if constexpr (std::is_same<T, bool>::value)
        return "bool";
    else if constexpr (std::is_same<T, std::string>::value)
        return "string";
    else if (isVector(temp))
        return "vector";
    else if (isPair(temp))
        return "pair";
    else if (isMap(temp))
        return "map";
    else if (isList(temp))
        return "list";
    else
        return "Unknown";
}
// 将 object 拆分开，将单个元素转换为 xml
// 针对基本类型：int, char, size_t, float, double, bool, string
template <typename T>
void addElement(const T &object, XMLDocument &doc, XMLElement *root)
{
    XMLElement *val_type = doc.NewElement(convertTypeName<T>());
    root->InsertEndChild(val_type);

    if constexpr (std::is_same<T, std::string>::value)
        val_type->SetAttribute("val", object.c_str());
    else if constexpr (std::is_same<T, char>::value)
    {
        std::string temp(1, object);
        val_type->SetAttribute("val", temp.c_str());
    }
    else
    {
        val_type->SetAttribute("val", object);
    }
}
// pair
template <typename F, typename S>
void addElement(const std::pair<F, S> &object, XMLDocument &doc, XMLElement *root)
{
    XMLElement *pair_root = doc.NewElement("pair");
    root->InsertEndChild(pair_root);
    // first 部分的处理
    XMLElement *first = doc.NewElement("first");
    addElement(object.first, doc, first);
    pair_root->InsertEndChild(first);
    XMLElement *second = doc.NewElement("second");
    addElement(object.second, doc, second);
    pair_root->InsertEndChild(second);
}
// vector
template <typename T>
void addElement(const std::vector<T> &object, XMLDocument &doc, XMLElement *root)
{
    XMLElement *vector_root = doc.NewElement("vector");
    root->InsertEndChild(vector_root);
    // data
    for (auto &item : object)
        addElement(item, doc, vector_root);
}
// list
template <typename T>
void addElement(const std::list<T> &object, XMLDocument &doc, XMLElement *root)
{
    XMLElement *list_root = doc.NewElement("list");
    root->InsertEndChild(list_root);
    // data
    for (auto &item : object)
        addElement(item, doc, list_root);
}
// map
template <typename K, typename V>
void addElement(const std::map<K, V> &object, XMLDocument &doc, XMLElement *root)
{
    XMLElement *map_root = doc.NewElement("map");
    root->InsertEndChild(map_root);
    // data
    for (const auto &item : object)
    {
        XMLElement *map_entry = doc.NewElement("entry");
        map_root->InsertEndChild(map_entry);
        XMLElement *key = doc.NewElement("key");
        addElement(item.first, doc, key);
        map_entry->InsertEndChild(key);
        XMLElement *value = doc.NewElement("value");
        addElement(item.second, doc, value);
        map_entry->InsertEndChild(value);
    }
}
/*
 * Parse the element of the xml
 */
template <typename T>
XMLError parseElement(T &object, XMLDocument &doc, XMLElement *&root, bool isToFind = true)
{
    XMLError eResult;
    // std::cout << "parseElement : Typename " << convertTypeName<T>() << std::endl;
    XMLElement *element = root->FirstChildElement(convertTypeName<T>());
    eResult = readElementVal(object, element);
    return eResult;
}
template <typename T>
XMLError parseElement(T &object, XMLElement *&root)
{
    XMLError eResult;
    // std::cout << "parseElement : Typename " << convertTypeName<T>() << std::endl;
    eResult = readElementVal(object, root);
    return eResult;
}
// vector
template <typename T>
XMLError parseElement(std::vector<T> &object, XMLDocument &doc, XMLElement *root, bool isToFind = true)
{
    XMLError eResult = XML_SUCCESS;
    XMLElement *vector_element;
    if (isToFind)
        vector_element = root->FirstChildElement("vector");
    else
        vector_element = root;
    // std::cout << "parseElement : Typename " << convertTypeName<T>() << std::endl;
    XMLElement *element = vector_element->FirstChildElement(convertTypeName<T>());
    for (XMLElement *ptr = element; ptr != nullptr; ptr = ptr->NextSiblingElement())
    {
        T item;
        if (specialType(item))
            eResult = parseElement(item, doc, ptr, false);
        else
            eResult = parseElement(item, ptr);
        if (eResult == XML_SUCCESS)
        {
            object.push_back(item);
        }
        else
        {
            std::cout << "解析元素失败" << std::endl;
            return eResult;
        }
    }
    return eResult;
}
// list
template <typename T>
XMLError parseElement(std::list<T> &object, XMLDocument &doc, XMLElement *root, bool isToFind = true)
{
    XMLError eResult = XML_SUCCESS;
    XMLElement *list_element;
    if (isToFind)
        list_element = root->FirstChildElement("list");
    else
        list_element = root;
    // std::cout << "parseElement : Typename " << convertTypeName<T>() << std::endl;
    XMLElement *element = list_element->FirstChildElement(convertTypeName<T>());
    for (XMLElement *ptr = element; ptr != nullptr; ptr = ptr->NextSiblingElement())
    {
        T item;
        if (specialType(item))
            eResult = parseElement(item, doc, ptr, false);
        else
            eResult = parseElement(item, ptr);
        if (eResult == XML_SUCCESS)
        {
            object.push_back(item);
        }
        else
        {
            std::cout << "解析元素失败" << std::endl;
            return eResult;
        }
    }
    return eResult;
}
// pair
template <typename F, typename S>
XMLError parseElement(std::pair<F, S> &object, XMLDocument &doc, XMLElement *root, bool isToFind = true)
{
    XMLError eResult = XML_SUCCESS;
    XMLElement *pair_element;
    if (isToFind)
        pair_element = root->FirstChildElement("pair");
    else
        pair_element = root;

    XMLElement *first = pair_element->FirstChildElement("first");
    XMLElement *second = pair_element->FirstChildElement("second");
    F itemF;
    eResult = parseElement(object.first, doc, first);
    S itemS;
    eResult = parseElement(object.second, doc, second);
    return eResult;
}
// map
template <typename K, typename V>
XMLError parseElement(std::map<K, V> &object, XMLDocument &doc, XMLElement *root, bool isToFind = true)
{
    XMLError eResult = XML_SUCCESS;
    XMLElement *map_element;
    if (isToFind)
        map_element = root->FirstChildElement("map");
    else
        map_element = root;

    // std::cout << "parseElement : Typename map<" << convertTypeName<K>() << ", " << convertTypeName<V>() << ">" << std::endl;
    for (XMLElement *entry = map_element->FirstChildElement("entry"); entry != nullptr; entry = entry->NextSiblingElement("entry"))
    {
        K key;
        V value;
        XMLElement *key_element = entry->FirstChildElement("key");

        eResult = parseElement(key, doc, key_element);

        if (eResult != XML_SUCCESS)
        {
            std::cout << "parse key fails" << std::endl;
            return eResult;
        }

        XMLElement *value_element = entry->FirstChildElement("value");

        eResult = parseElement(value, doc, value_element);

        if (eResult != XML_SUCCESS)
        {
            std::cout << "parse value fails" << std::endl;
            return eResult;
        }
        object.emplace(key, value);
    }

    return eResult;
}
#endif