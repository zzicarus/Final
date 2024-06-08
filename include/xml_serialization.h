#ifndef _SERIALIZATION_H_
#define _SERIALIZATION_H_

#include "xml_helpfunc.h"

namespace xml_serialization
{
#define XML_Open_File_out()                                 \
    XMLDocument doc_udt;                                    \
    XMLDeclaration *decl = doc_udt.NewDeclaration();        \
    doc_udt.InsertFirstChild(decl);                         \
                                                            \
    XMLElement *root = doc_udt.NewElement("serialization"); \
    doc_udt.InsertEndChild(root);

#define XML_Add_Element(object, type_name) \
    addElement(object.type_name, doc_udt, root);

#define XML_End_File_out()                                      \
    {                                                           \
        XMLError eResult = doc_udt.SaveFile("../data/udt.xml"); \
        if (eResult != XML_SUCCESS)                             \
        {                                                       \
            std::cout << "Saving File Fails" << std::endl;      \
        }                                                       \
    }

#define XML_Open_File_in()                                             \
    {                                                                  \
        XMLError eResult = doc_udt.LoadFile("../data/udt.xml");        \
        XMLElement *root = doc_udt.FirstChildElement("serialization"); \
    }
#define XML_Parse_Element(object, type_name) \
    parseElement(object.type_name, doc_udt, root);
    using namespace tinyxml2;
    // Serialize and deserialize
    // 委托给 parseElement 去处理不同的类型
    template <typename T>
    void serialize(const T &object, const std::string &type_name, const std::string &file_path)
    {
        XMLDocument doc;
        XMLDeclaration *decl = doc.NewDeclaration();
        doc.InsertFirstChild(decl);

        XMLElement *root = doc.NewElement("serialization");
        doc.InsertEndChild(root);
        addElement(object, doc, root);
        XMLError eResult = doc.SaveFile(file_path.c_str());
        if (eResult != XML_SUCCESS)
        {
            std::cout << "Saving File Fails" << std::endl;
        }
    }
    template <typename T>
    void deserialize(T &object, const std::string &type_name, const std::string &file_path)
    {
        XMLDocument doc;
        XMLError eResult = doc.LoadFile(file_path.c_str());
        if (eResult != XML_SUCCESS)
        {
            std::cout << "Loading File Fails" << std::endl;
            return;
        }
        XMLElement *root = doc.FirstChildElement("serialization");
        if (root == nullptr)
        {
            std::cout << "Loading Root Element Fails" << std::endl;
            return;
        }
        parseElement(object, doc, root);
    }
}

#endif
