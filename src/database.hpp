#pragma once
#include "document.hpp"

class Database
{
  public:
    bool use_collection(const std::string& name);

    void insert(Document& doc);
    Document* get_by_id(const std::string& id);
    void remove(const std::string& id);
    void list() const;

    void save();
    void load();

  private:
    std::unordered_map<std::string, Document> storage;
    std::string current_collection;
};
