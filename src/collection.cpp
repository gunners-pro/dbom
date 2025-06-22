#include "collection.hpp"
#include "utils/uuid.hpp"
#include <fstream>
#include <iostream>

Collection::Collection(const std::string& name) : name(name)
{
    file_path = "data/" + name;
    load();
}

void Collection::insert(Document& doc)
{
    std::string id = generate_uuidV4();
    doc.id = id;
    documents[doc.id] = doc;
}

Document* Collection::get(const std::string& id)
{
    auto it = documents.find(id);
    if (it != documents.end())
        return &it->second;
    return nullptr;
}

int Collection::remove(const std::string& id)
{
    int result = documents.erase(id);
    return result;
}

std::vector<Document*> Collection::list() const
{
    std::vector<Document*> docs;
    for (const auto& [id, doc] : documents)
    {
        docs.push_back(const_cast<Document*>(&doc));
    }
    return docs;
}

void Collection::save() const
{
    std::ofstream file(file_path, std::ios::binary);
    for (const auto& [_, doc] : documents)
    {
        doc.serialize(file);
    }
}

void Collection::load()
{
    documents.clear();
    std::ifstream file(file_path, std::ios::binary);
    if (!file)
        return;

    while (file.peek() != EOF)
    {
        Document doc = Document::deserialize(file);
        documents[doc.id] = doc;
    }
}

std::string Collection::get_name() const
{
    return name;
}