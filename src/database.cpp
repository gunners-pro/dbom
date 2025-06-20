#include "database.hpp"
#include "utils/uuid.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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

void Database::save_to_file(const std::string& path) const
{
    json j_array = json::array();
    for (const auto& [_, doc] : storage)
    {
        j_array.push_back(json::parse(doc.to_json()));
    }
    std::ofstream file(path);
    file << j_array.dump(4);
}

void Database::load_from_file(const std::string& path)
{
    std::ifstream file(path);
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