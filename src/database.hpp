#pragma once
#include "collection.hpp"
#include "document.hpp"
#include <memory>

class Database
{
  public:
    bool use_collection(const std::string& name);
    std::vector<std::string> list_collections() const;
    bool delete_collection(const std::string& name);
    std::shared_ptr<Collection> current();

  private:
    std::unordered_map<std::string, std::shared_ptr<Collection>> collections;
    std::string current_collection_name;
};
