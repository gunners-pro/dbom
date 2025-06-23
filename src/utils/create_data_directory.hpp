#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

inline void create_data_directory()
{
    try
    {
        if (!fs::exists("data") && !fs::is_directory("data"))
        {
            fs::create_directory("data");
            std::cout << "Diretorio de dados criado." << std::endl;
        }
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "Erro ao criar diretorio de dados: " << e.what() << std::endl;
    }
}