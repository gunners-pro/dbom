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
    std::unordered_map<std::string, Document> storage;
    std::shared_ptr<Collection> current_collection;
};
