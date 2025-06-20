#include "collection.hpp"
#include "utils/uuid.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Collection::Collection(const std::string& name) : name(name)
{
    file_path = "data/" + name + ".json";
    load();
}

void Collection::insert(Document& doc)
{
    std::string id = generate_uuidV4();
    doc.id = id;
    documents[doc.id] = doc;
    save();
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
    save();
    return result;
}

void Collection::list() const
{
    for (const auto& [id, doc] : documents)
    {
        std::cout << doc.to_json() << "\n";
    }
}

void Collection::save() const
{
    json j_array = json::array();
    for (const auto& [_, doc] : documents)
    {
        j_array.push_back(json::parse(doc.to_json()));
    }
    std::ofstream file(file_path);
    file << j_array.dump(4);
}

void Collection::load()
{
    documents.clear();
    std::ifstream file(file_path);
    if (!file)
        return;

    json j_array;
    file >> j_array;
    for (const auto& j : j_array)
    {
        Document doc = Document::from_json(j.dump());
        documents[doc.id] = doc;
    }
}

std::string Collection::get_name() const
{
    return name;
}