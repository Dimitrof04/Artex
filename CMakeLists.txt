cmake_minimum_required(VERSION 3.15)
project(MeuProjetoProfissional VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontra a biblioteca instalada no sistema (Arch Linux)

add_executable(meu_executavel main.cpp
        hpps/Includes.h++
        hpps/ArtexBuilder.h++
        hpps/VersionManager.h++
        hpps/ArtexTokens.h++
        hpps/json.hpp)

# Linka a biblioteca de forma limpa
#target_link_libraries(meu_executavel PRIVATE nlohmann_json::nlohmann_json)