#include "database.hpp"
#include "utils/create_data_directory.hpp"
#include <filesystem>
#include <iostream>
#include <sstream>

int main(int argc, char* argv[])
{
    create_data_directory();
    Database db;
    db.load();

    std::string line;
    std::cout << "DBom NoSQL interativo. Digite 'help' para ajuda.\n";

    while (true)
    {
        std::cout << "> ";
        if (!std::getline(std::cin, line))
            break;

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "exit" || cmd == "quit")
        {
            std::cout << "Saindo...\n";
            break;
        }
        else if (cmd == "help")
        {
            std::cout << "Comandos disponiveis:\n"
                      << "  insert {json}       - Insere um documento\n"
                      << "  use {collection}    - Alterna uma colecao\n"
                      << "  get <id>            - Exibe um documento\n"
                      << "  delete <id>         - Remove um documento\n"
                      << "  list                - Lista todos os documentos\n"
                      << "  exit                - Encerra o programa\n";
        }
        else if (cmd == "insert")
        {
            std::string json_str;
            std::getline(iss, json_str); // pega o restante da linha
            try
            {
                Document doc = Document::from_json(json_str);
                db.insert(doc);
                db.save();
                std::cout << "Documento inserido.\n";
            }
            catch (...)
            {
                std::cerr << "Erro ao interpretar JSON.\n";
            }
        }
        else if (cmd == "get")
        {
            std::string id;
            iss >> id;
            Document* doc = db.get_by_id(id);
            if (doc)
                std::cout << doc->to_json() << "\n";
            else
                std::cout << "Documento não encontrado.\n";
        }
        else if (cmd == "delete")
        {
            std::string id;
            iss >> id;
            db.remove(id);
            db.save();
            std::cout << "Documento removido.\n";
        }
        else if (cmd == "list")
        {
            db.list();
        }
        else if (cmd == "use")
        {
            std::string name;
            iss >> name;
            if (db.use_collection(name))
            {
                std::cout << "Usando coleção: " << name << std::endl;
            }
            else
            {
                std::cout << "Erro ao trocar de coleção." << std::endl;
            }
        }
        else
        {
            std::cout << "Comando desconhecido. Digite 'help'.\n";
        }
    }
}