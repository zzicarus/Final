#include "..\include\binary_serialization.h"
#include "..\include\xml_serialization.h"
void TestForBinary();
void TestForXml();

template <typename InputIterator>
void Print(InputIterator b, InputIterator e)
{
    for (InputIterator it = b; it != e; ++it)
        std::cout << *it << " ";
}
template <typename T, typename P>
void Print(const std::map<T, P> &Map)
{
    for (const auto &elem : Map)
        std::cout << "(" << elem.first << "," << elem.second << "),";
}

int main()
{
    TestForBinary();
    TestForXml();

    return 0;
}

// Test for binary_serialization
void TestForBinary()
{
    using namespace binary_serialization;
    std::cout << std::endl
              << std::endl;
    std::cout << "--- Test for binary serialization ---" << std::endl;
    // Arithmetic types
    int int0 = 256, int1;
    serialize(int0, "../data/int.data");   // serialize object int0 to a binary file int.data
    deserialize(int1, "../data/int.data"); // reconstruct object int1 from the content of int.data
    std::cout << "int Input:  " << int0 << "\nOutput: " << int1 << "\n\n";

    // String
    std::string str0 = "OOP Final Project", str1;
    serialize(str0, "../data/str.data");
    deserialize(str1, "../data/str.data");
    std::cout << "str Input:  " << str0 << "\nOutput: " << str1 << "\n\n";

    // STL containers
    // std::pair
    std::pair<int, int> p0 = {0, 1}, p1;
    serialize(p0, "../data/pair.data");
    deserialize(p1, "../data/pair.data");
    std::cout << "pair Input:  (" << p0.first << ", " << p0.second << ")\nOutput: (" << p1.first << ", " << p1.second << ")\n\n";

    // std::pair nested cases
    std::pair<std::vector<int>, int> pa0 = {{0, 1, 2}, 1}, pa1;
    serialize(pa0, "../data/pair_nested.data");
    deserialize(pa1, "../data/pair_nested.data");
    std::cout << "pair nested Input:  (";
    Print(pa0.first.begin(), pa0.first.end());
    std::cout << pa0.second << ")\nOutput: (";
    Print(pa1.first.begin(), pa1.first.end());
    std::cout << pa1.second << ")\n\n";

    // std::vector
    std::vector<std::vector<int>> vec0 = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}}, vec1;
    serialize(vec0, "../data/vector.data");
    deserialize(vec1, "../data/vector.data");
    std::cout << "vector Input:  ";
    for (const auto &v : vec0)
    {
        std::cout << "{";
        Print(v.begin(), v.end());
        std::cout << "}";
    }
    std::cout << "\nOutput: ";
    for (const auto &v : vec1)
    {
        std::cout << "{";
        Print(v.begin(), v.end());
        std::cout << "}";
    }
    std::cout << "\n\n";

    // std::list
    std::list<int> list0 = {0, 1, 2}, list1;
    serialize(list0, "../data/list.data");
    deserialize(list1, "../data/list.data");
    std::cout << "list Input: ";
    Print(list0.begin(), list0.end());
    std::cout << "\nOutput: ";
    Print(list1.begin(), list1.end());
    std::cout << "\n\n";

    // std::set
    std::set<int> set0 = {0, 1, 2}, set1;
    serialize(set0, "../data/set.data");
    deserialize(set1, "../data/set.data");
    std::cout << "set Input: ";
    Print(set0.begin(), set0.end());
    std::cout << "\nOutput: ";
    Print(set1.begin(), set1.end());
    std::cout << "\n";

    // std::map
    std::map<double, int> a = {{5.0, 5}, {6.0, 6}};
    serialize(a, "../data/map.data");
    std::map<double, int> b;
    deserialize(b, "../data/map.data");
    std::cout << "map Input:  ";
    Print(a);
    std::cout << "Output: ";
    Print(b);
    std::cout << std::endl;
    // nested map
    std::map<int, std::map<double, int>> nestedMap = {
                                             {1, {{1.1, 10}, {1.2, 20}}},
                                             {2, {{2.1, 30}, {2.2, 40}}}},
                                         nestedMap1;

    // 序列化
    serialize(nestedMap, "../data/set.data");
    deserialize(nestedMap1, "../data/set.data");
    std::cout << "Nested Map Input: " << std::endl;
    for (auto elem : nestedMap)
    {
        std::cout << "(" << elem.first << ",";
        std::cout << "[";
        Print(elem.second);
        std::cout << "]" << ")";
    }
    std::cout << "\nDeserialized Output: " << std::endl;
    for (auto elem : nestedMap1)
    {
        std::cout << "(" << elem.first << ",";
        std::cout << "[";
        Print(elem.second);
        std::cout << "]" << ")";
    }
    std::cout << std::endl;
    // user difined type
    struct UDT
    {
        int id;
        std::string name;
        std::vector<double> value;
    };
    UDT udt = {4, "OOP", {101.101, 95.01, 96.03}}, udt1;
    Binary_Open_File_out();
    Binary_Add_Element(udt, id);
    Binary_Add_Element(udt, name);
    Binary_Add_Element(udt, value);
    Binary_End_File_out();
    Binary_Open_File_in();
    Binary_Parse_Element(udt1, id);
    Binary_Parse_Element(udt1, name);
    Binary_Parse_Element(udt1, value);
    Binary_End_File_in();

    std::cout << "User Defined Type: " << udt1.id << " " << udt1.name << " ";
    for (auto i : udt1.value)
        std::cout << i << " ";
}

void TestForXml()
{
    using namespace xml_serialization;
    std::cout << std::endl
              << std::endl;
    std::cout << "--- Test for xml serialization ---" << std::endl;
    const std::string file_name = "../data/test.xml";

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
    std::map<std::string, std::vector<std::vector<int>>> original_map{
        {"one", {{1, 2, 3}, {11, 22, 33}}},
        {"two", {{4, 5, 6}, {44, 55, 66}}},
        {"three", {{7, 8, 9}, {77, 88, 99}}}},
        deserialized_map;
    serialize(original_map, map_type, file_name);
    deserialize(deserialized_map, map_type, file_name);
    std::cout << "\nstd::map<std::string, std::vector<std::vector<int>>>:" << std::endl;
    for (const auto &pair : deserialized_map)
    {
        std::cout << "{ " << pair.first << ": [";
        for (auto items : pair.second)
        {
            std::cout << "{ ";
            for (auto item : items)
                std::cout << item << ",";
            std::cout << "} ";
        }
        std::cout << "] }" << std::endl;
    }
    // user defined
    struct UDT
    {
        int id;
        std::string name;
        std::vector<double> value;
    };
    UDT udt = {4, "OOP", {101.101, 95.01, 96.03}}, udt1;
    XML_Open_File_out();
    XML_Add_Element(udt, id);
    XML_Add_Element(udt, name);
    XML_Add_Element(udt, value);
    XML_End_File_out();
    XML_Open_File_in();
    XML_Parse_Element(udt1, id);
    XML_Parse_Element(udt1, name);
    XML_Parse_Element(udt1, value);

    std::cout << "User Defined Type: " << udt1.id << " " << udt1.name << " ";
    for (auto i : udt1.value)
        std::cout << i << " ";
}