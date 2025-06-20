#include "database.hpp"
#include "utils/create_data_directory.hpp"
#include <repl.hpp>

int main(int argc, char* argv[])
{
    create_data_directory();
    Database db;
    run_repl(db);
    return 0;
}