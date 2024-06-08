#include "xml_serialization.h"
#include "tinyxml2.h"
#include <iostream>
#include <vector>
using namespace xml_serialization;
struct UserDefinedType
{
    int idx;
    std::string name;
    std::vector<double> data;
};
int main()
{
    const std::string file_name = "test.xml";

    // int
    int i = 10086, io;
    const std::string int_type = "int";
    serialize(i, int_type, file_name);
    deserialize(io, int_type, file_name);
    std::cout << "int: " << io << std::endl;

    // char
    char c = 'A', co;
    const std::string char_type = "char";
    serialize(c, char_type, file_name);
    deserialize(co, char_type, file_name);
    std::cout << "char: " << co << std::endl;

    // size_t
    size_t s = 12345, so;
    const std::string size_t_type = "size_t";
    serialize(s, size_t_type, file_name);
    deserialize(so, size_t_type, file_name);
    std::cout << "size_t: " << so << std::endl;

    // float
    float f = 123.456f, fo;
    const std::string float_type = "float";
    serialize(f, float_type, file_name);
    deserialize(fo, float_type, file_name);
    std::cout << "float: " << fo << std::endl;

    // double
    double d = 789.012, do_;
    const std::string double_type = "double";
    serialize(d, double_type, file_name);
    deserialize(do_, double_type, file_name);
    std::cout << "double: " << do_ << std::endl;

    // bool
    bool b = true, bo;
    const std::string bool_type = "bool";
    serialize(b, bool_type, file_name);
    deserialize(bo, bool_type, file_name);
    std::cout << "bool: " << bo << std::endl;

    // std::string
    std::string str = "Hello, oOP!", stro;
    const std::string string_type = "string";
    serialize(str, string_type, file_name);
    deserialize(stro, string_type, file_name);
    std::cout << "string: " << stro << std::endl;
    /*
    TODO: how to detect the loop? while 套起来之后，向xml添加？
    */
    // vector
    std::vector<double> vec = {0.5, 0.6, 0.7}, vec1;
    const std::string vec_type = "vector";
    serialize(vec, vec_type, file_name);
    deserialize(vec1, vec_type, file_name);
    std::cout << "vector: ";
    for (auto i : vec1)
        std::cout << i << " ";
    std::cout << std::endl;
    // nested vector
    std::vector<std::vector<double>> nvec = {{0.5, 0.6, 0.7}, {0.55, 0.67}}, nvec1;
    const std::string nvec_type = "vector";
    serialize(nvec, nvec_type, file_name);
    deserialize(nvec1, nvec_type, file_name);
    std::cout << "vector<std::vector<double>>: " << std::endl;
    for (auto i : nvec1)
    {
        for (auto item : i)
            std::cout << item << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // std::pair
    std::pair<std::string, int> pa = {"it is string in pair ", 6}, pa1;
    const std::string pair_type = "pair";
    serialize(pa, pair_type, file_name);
    deserialize(pa1, pair_type, file_name);
    std::cout << "pair: ";
    std::cout << "First: " << pa1.first << "Second: " << pa1.second << std::endl;
    // nested pair
    std::pair<std::string, std::pair<int, double>> nested_pair{"example", {42, 3.14}}, deserialized_pair;
    xml_serialization::serialize(nested_pair, pair_type, file_name);
    xml_serialization::deserialize(deserialized_pair, pair_type, file_name);
    std::cout << "Nested pair: " << std::endl;
    std::cout << "{ " << deserialized_pair.first << ": { "
              << deserialized_pair.second.first << ", "
              << deserialized_pair.second.second << " } }" << std::endl;
    // std::list
    std::list<double> li = {0.5, 0.6, 0.7, 0.88}, li1;
    const std::string list_type = "list";
    serialize(li, list_type, file_name);
    deserialize(li1, list_type, file_name);
    std::cout << "list: ";
    for (auto i : li1)
        std::cout << i << " ";
    std::cout << std::endl;
    // nested list
    std::list<std::vector<double>> nli = {{0.5, 0.6, 0.7, 0.88}, {0.1}, {0.2}}, nli1;
    const std::string nlist_type = "list";
    serialize(nli, nlist_type, file_name);
    deserialize(nli1, nlist_type, file_name);
    std::cout << "nlist<std::vector<double>>: " << std::endl;
    for (auto i : nli1)
    {
        std::cout << "  ";
        for (auto ii : i)
            std::cout << ii << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    // std::map
    std::map<std::string, int> ma{{"one", 1}, {"two", 2}, {"three", 3}}, ma1;
    const std::string map_type = "map";
    serialize(ma, map_type, file_name);
    deserialize(ma1, map_type, file_name);
    std::cout << "map: " << std::endl;
    for (const auto &pair : ma1)
        std::cout << "{ " << pair.first << ": " << pair.second << " }" << std::endl;
    // nested map
    std::map<std::string, std::vector<int>> original_map{
        {"one", {1, 2, 3}},
        {"two", {4, 5, 6}},
        {"three", {7, 8, 9}}},
        deserialized_map;
    serialize(original_map, map_type, file_name);
    deserialize(deserialized_map, map_type, file_name);
    std::cout << "map<string, vector<int>>:" << std::endl;
    for (const auto &pair : deserialized_map)
    {
        std::cout << "{ " << pair.first << ": [";
        for (size_t i = 0; i < pair.second.size(); ++i)
        {
            std::cout << pair.second[i];
            if (i < pair.second.size() - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << "] }" << std::endl;
    }

    return 0;
}