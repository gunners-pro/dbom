#pragma once

#include "document.hpp"
#include <string>
#include <unordered_map>

class Collection
{
  public:
    Collection(const std::string& name);

    void insert(Document& doc);
    const Document* get(const std::string& id);
    int remove(const std::string& id);
    std::vector<const Document*> list() const;
    std::string get_name() const;
    void save() const;
    void load();

  private:
    std::string name;
    std::string file_path;
    std::unordered_map<std::string, Document> documents;
};
