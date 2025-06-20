#include "database.hpp"
#include "utils/uuid.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool Database::use_collection(const std::string& name)
{
    if (name.empty())
        return false;

    current_collection = "data/" + name + ".json";
    load();
    return true;
}

void Database::insert(Document& doc)
{
    std::string id = generate_uuidV4();
    doc.id = id;
    storage[id] = doc;
}

Document* Database::get_by_id(const std::string& id)
{
    if (storage.count(id))
        return &storage[id];
    return nullptr;
}

void Database::remove(const std::string& id)
{
    storage.erase(id);
}

void Database::list() const
{
    for (const auto& [id, doc] : storage)
    {
        std::cout << id << ": " << doc.to_json() << std::endl;
    }
}

void Database::save()
{
    json j_array = json::array();
    for (const auto& [_, doc] : storage)
    {
        j_array.push_back(json::parse(doc.to_json()));
    }
    std::ofstream file(current_collection);
    file << j_array.dump(4);
}

void Database::load()
{
    storage.clear();
    std::ifstream file(current_collection);
    if (!file)
        return;

    json j_array;
    file >> j_array;
    for (const auto& j : j_array)
    {
        Document doc = Document::from_json(j.dump());
        storage[doc.id] = doc;
    }
}