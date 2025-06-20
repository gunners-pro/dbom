#pragma once
#include "document.hpp"

class Database
{
  public:
    void insert(Document& doc);
    Document* get_by_id(const std::string& id);
    void remove(const std::string& id);
    void list() const;

    void save_to_file(const std::string& path) const;
    void load_from_file(const std::string& path);

  private:
    std::unordered_map<std::string, Document> storage;
};
