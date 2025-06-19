#include "document.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string Document::to_json() const
{
    json j;
    j["id"] = id;

    // Itera sobre os campos do documento
    for (const auto& [key, value] : fields)
    {
        // Verifica o tipo armazenado e imprime adequadamente
        if (std::holds_alternative<std::string>(value))
        {
            j[key] = std::get<std::string>(value);
        }
        else if (std::holds_alternative<int>(value))
        {
            j[key] = std::get<int>(value);
        }
        else if (std::holds_alternative<double>(value))
        {
            j[key] = std::get<double>(value);
        }
        else if (std::holds_alternative<bool>(value))
        {
            j[key] = (std::get<bool>(value) ? "true" : "false");
        }
        else if (std::holds_alternative<std::vector<std::string>>(value))
        {
            j[key] = std::get<std::vector<std::string>>(value);
        }
    }

    return j.dump();
};

Document Document::from_json(const std::string& json_str)
{
    Document doc;
    auto j = json::parse(json_str);

    if (j.contains("id"))
        doc.id = j["id"].get<std::string>();

    for (auto it = j.begin(); it != j.end(); it++)
    {
        if (it.key() == "id")
            continue;

        if (it->is_string())
            doc.fields[it.key()] = it->get<std::string>();
        else if (it->is_number_integer())
            doc.fields[it.key()] = it->get<int>();
        else if (it->is_number_float())
            doc.fields[it.key()] = it->get<double>();
        else if (it->is_boolean())
            doc.fields[it.key()] = it->get<bool>();
        else if (it->is_array())
            doc.fields[it.key()] = it->get<std::vector<std::string>>();
    }

    return doc;
};