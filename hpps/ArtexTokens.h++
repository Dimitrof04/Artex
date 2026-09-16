//
// Created by lulu on 9/15/26.
//

#ifndef ARTEX_ARTEXTOKENS_H
#define ARTEX_ARTEXTOKENS_H

// =====================================
// Artex Builder                       |
// =====================================
class ArtexToken {
private:
    std::string command; // Ex: "class ", "int "
    std::string ArtexFormation; // Ex: "CC", "VI"

public:
    ArtexToken(std::string command, std::string ArtexFormation) : command(command), ArtexFormation(ArtexFormation) {
    }

    // Getters: permitem ler os dados privados
    std::string getCommand() const { return command; }
    std::string getFormation() const { return ArtexFormation; }
};

// Tabela única centralizada de Tokens
inline const std::vector<ArtexToken> artexTokens = {
    // artex
    {"FOLDER", "D"},
    {"FILE", "F"},
    // global
    {"class ", "CC"},
    {"public:", "CP"},
    {"private:", "CI"},
    {"self", "Cs"},
    {"this", "Ct"},
    {"int ", "VI"},
    {"float ", "VF"},
    {"double ", "VD"},
    {"string ", "VS"},
    {"char", "VC"},
    {"return ", "RE"},
    {"for ", "FF"},
    {"while ", "FW"},
    {"case ", "FC"},
    {"if ", "FI"},
    {"else", "FE"},
    {"then", "TH"},
    {"const", "TC"},
    {"static", "TS"},
    {"main", "TM"},
    {"void", "TV"},
    {"from", "TF"},
    {"import", "TI"},
    // javascript (web)
    {"let", "wl"},
    // lua
    {"local", "Ll"},
    {"fuction", "Lf"},
    {"require", "Lr"},
    // cpp / c#
    {"#include", "CpI"},
    {"using", "CpU"},
    {"inline", "CpI"},
    {"printf", "CpP"},
    {"namespace", "CpN"},
    {"std::", "Cps"},
    // linux
    {"sudo", "Ls"},
    {"mkdir", "Lm"},
    {"touch", "Lt"},
    // arch
    {"pacman", "LAp"}
};

// Função auxiliar para buscar a formação de um token por nome (ex: "FOLDER" -> "D")
inline std::string getTokenFormation(const std::string &name) {
    for (const auto &token: artexTokens) {
        if (token.getCommand() == name) {
            return token.getFormation();
        }
    }
    return "";
}

#endif //ARTEX_ARTEXTOKENS_H