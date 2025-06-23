#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

using json = nlohmann::json;

using DocumentValue = std::variant<std::string, double, int, bool, std::vector<std::string>>;

class Document
{
  public:
    std::unordered_map<std::string, DocumentValue> fields;
    std::string id;

    // std::string to_json() const;
    static Document from_json(const std::string& json);
    void serialize(std::ostream& out) const;
    static Document deserialize(std::istream& in);
};