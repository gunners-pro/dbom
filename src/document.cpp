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

void Document::serialize(std::ostream& out) const
{
    size_t id_size = id.size();
    out.write(reinterpret_cast<const char*>(&id_size), sizeof(id_size));
    out.write(id.data(), id_size);

    size_t num_fields = fields.size();
    out.write(reinterpret_cast<const char*>(&num_fields), sizeof(num_fields));

    for (const auto& [key, value] : fields)
    {
        size_t key_size = key.size();
        out.write(reinterpret_cast<const char*>(&key_size), sizeof(key_size));
        out.write(key.data(), key_size);

        uint8_t type = value.index();
        out.write(reinterpret_cast<const char*>(&type), sizeof(type));

        // serializa o valor de acordo com o tipo
        if (std::holds_alternative<std::string>(value))
        {
            const std::string& str = std::get<std::string>(value);
            size_t str_size = str.size();
            out.write(reinterpret_cast<const char*>(&str_size), sizeof(str_size));
            out.write(str.data(), str_size);
        }
        else if (std::holds_alternative<double>(value))
        {
            double d = std::get<double>(value);
            out.write(reinterpret_cast<const char*>(&d), sizeof(d));
        }
        else if (std::holds_alternative<int>(value))
        {
            int i = std::get<int>(value);
            out.write(reinterpret_cast<const char*>(&i), sizeof(i));
        }
        else if (std::holds_alternative<bool>(value))
        {
            bool b = std::get<bool>(value);
            out.write(reinterpret_cast<const char*>(&b), sizeof(b));
        }
        else if (std::holds_alternative<std::vector<std::string>>(value))
        {
            const auto& vec = std::get<std::vector<std::string>>(value);
            size_t vec_size = vec.size();
            out.write(reinterpret_cast<const char*>(&vec_size), sizeof(vec_size));
            for (const auto& s : vec)
            {
                size_t s_size = s.size();
                out.write(reinterpret_cast<const char*>(&s_size), sizeof(s_size));
                out.write(s.data(), s_size);
            }
        }
    }
}

Document Document::deserialize(std::istream& in)
{
    Document doc;
    size_t id_size;
    in.read(reinterpret_cast<char*>(&id_size), sizeof(id_size));
    doc.id.resize(id_size);
    in.read(doc.id.data(), id_size);

    size_t num_fields;
    in.read(reinterpret_cast<char*>(&num_fields), sizeof(num_fields));

    for (size_t i = 0; i < num_fields; ++i)
    {
        size_t key_size;
        in.read(reinterpret_cast<char*>(&key_size), sizeof(key_size));
        std::string key(key_size, ' ');
        in.read(key.data(), key_size);

        uint8_t type;
        in.read(reinterpret_cast<char*>(&type), sizeof(type));

        switch (type)
        {
        case 0: { // string
            size_t str_size;
            in.read(reinterpret_cast<char*>(&str_size), sizeof(str_size));
            std::string str(str_size, ' ');
            in.read(str.data(), str_size);
            doc.fields[key] = str;
            break;
        }
        case 1: { // double
            double d;
            in.read(reinterpret_cast<char*>(&d), sizeof(d));
            doc.fields[key] = d;
            break;
        }
        case 2: { // int
            int i;
            in.read(reinterpret_cast<char*>(&i), sizeof(i));
            doc.fields[key] = i;
            break;
        }
        case 3: { // bool
            bool b;
            in.read(reinterpret_cast<char*>(&b), sizeof(b));
            doc.fields[key] = b;
            break;
        }
        case 4: { // vector<string>
            size_t vec_size;
            in.read(reinterpret_cast<char*>(&vec_size), sizeof(vec_size));
            std::vector<std::string> vec;
            for (size_t j = 0; j < vec_size; ++j)
            {
                size_t s_size;
                in.read(reinterpret_cast<char*>(&s_size), sizeof(s_size));
                std::string s(s_size, ' ');
                in.read(s.data(), s_size);
                vec.push_back(s);
            }
            doc.fields[key] = vec;
            break;
        }
        }
    }

    return doc;
}