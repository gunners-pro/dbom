#include "database.hpp"
#include "utils/uuid.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

bool Database::use_collection(const std::string& name)
{
    current_collection = std::make_shared<Collection>(name);
    return true;
}

std::vector<std::string> Database::list_collections() const
{
    std::vector<std::string> collections;
    for (const auto& entry : std::filesystem::directory_iterator("data"))
    {
        if (entry.is_regular_file())
        {
            std::string filename = entry.path().filename().string();
            if (filename.ends_with(".json"))
            {
                collections.push_back(filename.substr(0, filename.size() - 5));
            }
        }
    }
    return collections;
}

bool Database::delete_collection(const std::string& name)
{
    try
    {
        std::string path = "data/" + name + ".json";
        return std::filesystem::remove(path);
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        std::cerr << "Erro ao deletar colecao: " << name << e.what() << std::endl;
        return false;
    }
}

std::shared_ptr<Collection> Database::current()
{
    return current_collection;
}