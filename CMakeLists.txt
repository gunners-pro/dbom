cmake_minimum_required(VERSION 3.22)

project(dbom_core VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adiciona os arquivos do seu projeto
add_library(dbom_core STATIC
    src/document.cpp
    src/database.cpp
    src/collection.cpp
    src/cli/repl.cpp
)

# Executável
add_executable(dbom_cli    
    src/main.cpp
)

# Link com a lib
target_link_libraries(dbom_cli PRIVATE dbom_core)

# Incluir diretório de headers locais
target_include_directories(dbom_core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

