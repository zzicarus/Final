#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define DEFINE_SERIALIZABLE(Type, ...)                            \
    void serialize(const Type &obj, const std::string &filename)  \
    {                                                             \
        std::ofstream ofs(filename, std::ios::binary);            \
        if (!ofs)                                                 \
        {                                                         \
            std::cerr << "无法打开文件进行写入" << std::endl;     \
            return;                                               \
        }                                                         \
        binary_serialization::serialize(ofs, obj.*__VA_ARGS__);   \
        ofs.close();                                              \
    }                                                             \
    void deserialize(Type &obj, const std::string &filename)      \
    {                                                             \
        std::ifstream ifs(filename, std::ios::binary);            \
        if (!ifs)                                                 \
        {                                                         \
            std::cerr << "无法打开文件进行读取" << std::endl;     \
            return;                                               \
        }                                                         \
        binary_serialization::deserialize(ifs, obj.*__VA_ARGS__); \
        ifs.close();                                              \
    }

namespace binary_serialization
{
    template <typename... Args>
    void serialize(std::ofstream &out, Args... args)
    {
        int temp[] = {(binary_serialization::serialize(out, args), 0)...};
    }
    template <typename... Args>
    void deserialize(std::ifstream &in, Args... args)
    {
        int temp[] = {(binary_serialization::deserialize(in, args), 0)...};
    }
    // 序列化模板

    template <typename T>
    void serialize(std::ofstream &out, const T &object)
    {
        out.write(reinterpret_cast<const char *>(&object), sizeof(T));
    }

    // 反序列化模板
    template <typename T>
    void deserialize(std::ifstream &in, T &object)
    {
        in.read(reinterpret_cast<char *>(&object), sizeof(T));
    }

    // std::string特化
    void serialize(std::ofstream &out, const std::string &object)
    {
        size_t size = object.size();
        out.write(reinterpret_cast<const char *>(&size), sizeof(size));
        out.write(object.c_str(), size);
    }

    void deserialize(std::ifstream &in, std::string &object)
    {
        size_t size;
        in.read(reinterpret_cast<char *>(&size), sizeof(size));
        object.resize(size);
        in.read(&object[0], size);
    }
}

class CustomType
{
public:
    int id;
    std::string name;
    double value;

    CustomType(int id = 0, const std::string &name = "", double value = 0.0)
        : id(id), name(name), value(value) {}
};

// 使用宏定义CustomType的序列化和反序列化函数
DEFINE_SERIALIZABLE(CustomType, &CustomType::id, &CustomType::name, &CustomType::value)

int main()
{
    CustomType obj1(1, "test", 3.14);
    serialize(obj1, "custom_type.data");

    CustomType obj2;
    deserialize(obj2, "custom_type.data");

    std::cout << "ID: " << obj2.id << ", Name: " << obj2.name << ", Value: " << obj2.value << std::endl;

    return 0;
}
