#pragma once

#include "document.hpp"
#include <string>
#include <unordered_map>

class Collection
{
  public:
    Collection(const std::string& name);

    void insert(Document& doc);
    Document* get(const std::string& id);
    int remove(const std::string& id);
    std::vector<Document*> list() const;
    std::string get_name() const;
    void save() const;

  private:
    std::string name;
    std::string file_path;
    std::unordered_map<std::string, Document> documents;

    void load();
};
