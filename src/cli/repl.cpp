#include "repl.hpp"
#include <iostream>
#include <sstream>

void run_repl(Database& db)
{
    db.use_collection("default");

    std::string line;
    std::cout << "DBom NoSQL interativo. Digite 'help' para ajuda.\n";

    while (true)
    {
        std::string name_collection = db.current()->get_name();
        std::cout << "[dbom:" << name_collection << "]>";
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
                      << "  insert <json>               - Insere um documento.\n"
                      << "  get <id>                    - Exibe um documento.\n"
                      << "  delete <id>                 - Remove um documento.\n"
                      << "  list                        - Lista os documentos da colecao.\n"
                      << "  use {collection}            - Insere/Alterna uma colecao.\n"
                      << "  list-collections            - Lista todas as colecoes.\n"
                      << "  delete-collection <name>    - Remove a colecao.\n"
                      << "  exit                        - Encerra o programa.\n";
        }
        else if (cmd == "insert")
        {
            std::string json_str;
            json_str = line.substr(line.find("insert") + 6);
            try
            {
                Document doc = Document::from_json(json_str);
                db.current()->insert(doc);
                db.current()->save();
                std::cout << "Documento inserido." << std::endl;
            }
            catch (std::exception& e)
            {
                std::cerr << "Erro ao interpretar JSON." << std::endl;
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
        else if (cmd == "get")
        {
            std::string id;
            iss >> id;
            if (id.empty())
            {
                std::cout << "Digite um id valido." << std::endl;
            }
            const Document* doc = db.current()->get(id);
            if (doc)
                std::cout << doc << "\n";
            else
                std::cout << "Documento nao encontrado." << std::endl;
        }
        else if (cmd == "delete")
        {
            try
            {
                std::string id;
                iss >> id;
                int result = db.current()->remove(id);
                db.current()->save();
                if (result == 1)
                    std::cout << "Documento removido.\n";
                else
                    std::cout << "Documento nao encontrado.\n";
            }
            catch (const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
        else if (cmd == "list")
        {
            try
            {
                const auto docs = db.current()->list();
                for (const auto& doc : docs)
                {
                    std::cout << doc->to_json() << std::endl;
                }
            }
            catch (const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
        else if (cmd == "use")
        {
            std::string name;
            iss >> name;
            if (db.use_collection(name))
            {
                std::cout << "Usando colecao: " << name << std::endl;
            }
            else
            {
                std::cout << "Erro ao trocar de colecao." << std::endl;
            }
        }
        else if (cmd == "list-collections")
        {
            auto cols = db.list_collections();
            for (const auto& name : cols)
            {
                std::cout << name << "\n";
            }
        }
        else if (cmd == "delete-collection")
        {
            std::string name;
            iss >> name;
            if (db.delete_collection(name))
                std::cout << "Colecao removida." << std::endl;
            else
                std::cout << "Colecao nao existe." << std::endl;
        }
        else
        {
            std::cout << "Comando desconhecido. Digite 'help'.\n";
        }
    }
}