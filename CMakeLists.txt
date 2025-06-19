cmake_minimum_required(VERSION 3.22)

project(dbom VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adiciona os arquivos do seu projeto
add_executable(dbom
    src/main.cpp
    src/document.cpp
)

# Incluir diretório de headers locais
target_include_directories(dbom PRIVATE src)

# Baixa e inclui a biblioteca nlohmann/json via FetchContent
include(FetchContent)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0  # Ou use a versão mais recente
)

FetchContent_MakeAvailable(json)

# Linka a biblioteca ao seu executável
target_link_libraries(dbom PRIVATE nlohmann_json::nlohmann_json)
