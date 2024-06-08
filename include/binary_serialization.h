#ifndef _BINARY_SERIALIZATION_H
#define _BINARY_SERIALIZATION_H

#include <fstream>
#include <iostream>
// 定义宏，用于生成自定义类型的序列化和反序列化函数

namespace binary_serialization
{
    // General template for serialization
    template <typename T>
    void serialize(const T &object, const std::string &file_path)
    {
        std::ofstream out(file_path, std::ios::binary);
        if (out.is_open())
        {
            out.write(reinterpret_cast<const char *>(&object), sizeof(T));
        }
        out.close();
    }

    template <typename T>
    void serialize(const T &object, std::ofstream &out)
    {
        out.write(reinterpret_cast<const char *>(&object), sizeof(T));
    }

    // General template for deserialization
    template <typename T>
    void deserialize(T &object, const std::string &file_path)
    {
        std::ifstream in(file_path, std::ios::binary);
        if (in.is_open())
        {
            in.read(reinterpret_cast<char *>(&object), sizeof(T));
        }
        in.close();
    }

    template <typename T>
    void deserialize(T &object, std::ifstream &in)
    {
        in.read(reinterpret_cast<char *>(&object), sizeof(T));
    }

    // Specialization for std::string serialization & deserialization
    void serialize(const std::string &object, const std::string &file_path)
    {
        std::ofstream out(file_path, std::ios::binary);
        if (out.is_open())
        {
            size_t size = object.size();
            out.write(reinterpret_cast<const char *>(&size), sizeof(size));
            out.write(object.c_str(), size);
        }
        out.close();
    }

    void serialize(const std::string &object, std::ofstream &out)
    {
        size_t size = object.size();
        out.write(reinterpret_cast<const char *>(&size), sizeof(size));
        out.write(object.c_str(), size);
    }

    void deserialize(std::string &object, const std::string &file_path)
    {
        std::ifstream in(file_path, std::ios::binary);
        if (in.is_open())
        {
            size_t size;
            in.read(reinterpret_cast<char *>(&size), sizeof(size));
            char *buffer = new char[size + 1];
            in.read(buffer, size);
            buffer[size] = '\0';
            object.assign(buffer, size + 1);
            delete[] buffer;
        }
        in.close();
    }

    void deserialize(std::string &object, std::ifstream &in)
    {
        size_t size;
        in.read(reinterpret_cast<char *>(&size), sizeof(size));
        char *buffer = new char[size + 1];
        in.read(buffer, size);
        buffer[size] = '\0';
        object.assign(buffer, size);
        delete[] buffer;
    }

    // Specialization for std::vector<T> serialization & deserialization
    template <typename T>
    void serialize(const std::vector<T> &object, std::ofstream &out)
    {
        size_t size = object.size();
        out.write(reinterpret_cast<const char *>(&size), sizeof(size));
        for (const auto &elem : object)
        {
            serialize(elem, out);
        }
    }
    template <typename T>
    void serialize(const std::vector<T> &object, const std::string &file_path)
    {
        std::ofstream out(file_path, std::ios::binary);
        if (out.is_open())
        {
            size_t size = object.size();
            out.write(reinterpret_cast<const char *>(&size), sizeof(size));
            for (const T &elem : object)
            {
                serialize(elem, out);
            }
        }
        out.close();
    }
    template <typename T>
    void deserialize(std::vector<T> &object, std::ifstream &in)
    {
        size_t size;
        in.read(reinterpret_cast<char *>(&size), sizeof(size));
        object.resize(size);
        for (auto &elem : object)
        {
            deserialize(elem, in);
        }
    }
    template <typename T>
    void deserialize(std::vector<T> &object, const std::string &file_path)
    {
        std::ifstream in(file_path, std::ios::binary);
        if (in.is_open())
        {
            size_t size;
            in.read(reinterpret_cast<char *>(&size), sizeof(size));
            object.resize(size);
            for (auto &elem : object)
            {
                deserialize(elem, in);
            }
        }
        in.close();
    }

    // Specialization for std::pair<T,H> serialization & deserialization
    template <typename T, typename Q>
    void serialize(const std::pair<T, Q> &object, std::ofstream &out)
    {
        serialize(object.first, out);
        serialize(object.second, out);
    }
    template <typename T, typename Q>
    void serialize(const std::pair<T, Q> &object, const std::string &file_path)
    {
        std::ofstream out(file_path, std::ios::binary);
        if (out.is_open())
        {
            // 先T后Q
            serialize(object.first, out);
            serialize(object.second, out);
        }
        out.close();
    }
    template <typename T, typename Q>
    void deserialize(std::pair<T, Q> &object, std::ifstream &in)
    {
        deserialize(object.first, in);
        deserialize(object.second, in);
    }
    template <typename T, typename Q>
    void deserialize(std::pair<T, Q> &object, const std::string &file_path)
    {
        std::ifstream in(file_path, std::ios::binary);
        if (in.is_open())
        {
            deserialize(object.first, in);
            deserialize(object.second, in);
        }
        in.close();
    }

    // Specialization for std::list<T> serialization & deserialization
    template <typename T>
    void serialize(const std::list<T> &object, std::ofstream &out)
    {
        size_t size = object.size();
        // 指针怎么处理？是自动生成的吗？
        out.write(reinterpret_cast<const char *>(&size), sizeof(size));
        for (const auto &elem : object)
        {
            serialize(elem, out);
        }
    }
    template <typename T>
    void serialize(const std::list<T> &object, const std::string &file_path)
    {
        std::ofstream out(file_path, std::ios::binary);
        if (out.is_open())
        {
            size_t size = object.size();
            out.write(reinterpret_cast<const char *>(&size), sizeof(size));
            for (const T &elem : object)
            {
                serialize(elem, out);
            }
        }
        out.close();
    }
    template <typename T>
    void deserialize(std::list<T> &object, std::ifstream &in)
    {
        size_t size;
        in.read(reinterpret_cast<char *>(&size), sizeof(size));
        object.resize(size); // list supports it
        for (auto &elem : object)
        {
            deserialize(elem, in);
        }
    }
    template <typename T>
    void deserialize(std::list<T> &object, const std::string &file_path)
    {
        std::ifstream in(file_path, std::ios::binary);
        if (in.is_open())
        {
            size_t size;
            in.read(reinterpret_cast<char *>(&size), sizeof(size));
            object.resize(size);
            for (auto &elem : object)
            {
                deserialize(elem, in);
            }
        }
        in.close();
    }

    // Specialization for std::set<T> serialization & deserialization
    template <typename T>
    void serialize(const std::set<T> &object, std::ofstream &out)
    {
        size_t size = object.size();
        // 指针怎么处理？是自动生成的吗？
        out.write(reinterpret_cast<const char *>(&size), sizeof(size));
        for (const auto &elem : object)
        {
            serialize(elem, out);
        }
    }
    template <typename T>
    void serialize(const std::set<T> &object, const std::string &file_path)
    {
        std::ofstream out(file_path, std::ios::binary);
        if (out.is_open())
        {
            size_t size = object.size();
            out.write(reinterpret_cast<const char *>(&size), sizeof(size));
            for (const T &elem : object)
            {
                serialize(elem, out);
            }
        }
        out.close();
    }
    template <typename T>
    void deserialize(std::set<T> &object, std::ifstream &in)
    {
        size_t size;
        in.read(reinterpret_cast<char *>(&size), sizeof(size));
        // object.resize(size); // set supports it
        for (size_t index = 0; index < size; index++)
        {
            T temp;
            deserialize(temp, in);
            object.insert(temp);
        }
        //
    }
    template <typename T>
    void deserialize(std::set<T> &object, const std::string &file_path)
    {
        std::ifstream in(file_path, std::ios::binary);
        if (in.is_open())
        {
            size_t size;
            in.read(reinterpret_cast<char *>(&size), sizeof(size));
            for (size_t index = 0; index < size; index++)
            {
                T temp;
                deserialize(temp, in);
                object.insert(temp);
            }
        }
        in.close();
    }

    // Specialization for std::map<T,P> serialization & deserialization
    template <typename T, typename P>
    void serialize(const std::map<T, P> &object, std::ofstream &out)
    {
        size_t size = object.size();
        out.write(reinterpret_cast<const char *>(&size), sizeof(size));
        for (const auto &elem : object)
        {
            serialize(elem.first, out);
            serialize(elem.second, out);
        }
    }
    template <typename T, typename P>
    void serialize(const std::map<T, P> &object, const std::string &file_path)
    {
        std::ofstream out(file_path, std::ios::binary);
        if (out.is_open())
        {
            size_t size = object.size();
            out.write(reinterpret_cast<const char *>(&size), sizeof(size));
            for (const auto &elem : object)
            {
                serialize(elem.first, out);
                serialize(elem.second, out);
            }
        }
        out.close();
    }
    template <typename T, typename P>
    void deserialize(std::map<T, P> &object, std::ifstream &in)
    {
        size_t size;
        in.read(reinterpret_cast<char *>(&size), sizeof(size));
        // object.resize(size); // set supports it
        for (size_t index = 0; index < size; index++)
        {
            T tempT;
            P tempP;
            deserialize(tempT, in);
            deserialize(tempP, in);
            object.emplace(tempT, tempP);
        }
        //
    }
    template <typename T, typename P>
    void deserialize(std::map<T, P> &object, const std::string &file_path)
    {
        std::ifstream in(file_path, std::ios::binary);
        if (in.is_open())
        {
            size_t size;
            in.read(reinterpret_cast<char *>(&size), sizeof(size));
            for (size_t index = 0; index < size; index++)
            {
                T tempT;
                P tempP;
                deserialize(tempT, in);
                deserialize(tempP, in);
                object.emplace(tempT, tempP);
            }
        }
        in.close();
    }

}

#endif // _BINARY_SERIALIZATION_H
