#include "benchmark.hpp"
#include "database.hpp"
#include "utils/create_data_directory.hpp"
#include <cli/repl.hpp>

int main(int argc, char* argv[])
{
    create_data_directory();
    Database db;

    if (argc >= 2 && std::string(argv[1]) == "--benchmark")
    {
        int count = 5000; // padrão
        if (argc >= 3)
        {
            try
            {
                count = std::stoi(argv[2]);
            }
            catch (...)
            {
                std::cerr << "Erro: argumento inválido para quantidade.\n";
                return 1;
            }
        }
        benchmark(db, count);
        // Inseridos 1000 documentos em 32720 ms
        // 30.5623 docs/seg

        // Tempo total de parse:  27 ms
        // Tempo total de insert: 15454 ms
        // Tempo total de save:   16159 ms

        // Inseridos 5000 documentos em 660303 ms
        // 7.57228 docs/seg

        // Tempo total de parse:  143 ms
        // Tempo total de insert: 327606 ms
        // Tempo total de save:   327694 ms

        // Inseridos 5000 documentos em 431 ms
        // 11600.9 docs/seg

        // Tempo total de parse:  84 ms
        // Tempo total de insert: 51 ms
        // Tempo total de save:   141 ms
        return 0;
    }
    run_repl(db);
    return 0;
}