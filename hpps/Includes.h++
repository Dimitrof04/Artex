//
// Created by lulu on 9/15/26.
//

#ifndef ARTEX_INCLUDES_H
#define ARTEX_INCLUDES_H

#pragma once

#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <set>
#include <string>
#include <vector>
#include <unistd.h>    // Necessário para setuid() e getuid()
#include <stdio.h>
#include <termios.h>
#include "json.hpp"

#include "ArtexTokens.h++"
#include "ArtexBuilder.h++"
#include "VersionManager.h++"

bool Noconfirmroot = false;

char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0) perror("tcsetattr()");
    old.c_lflag &= ~ICANON; // Desativa o modo canônico (esperar por enter)
    old.c_lflag &= ~ECHO;   // Não mostra a tecla digitada na tela
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0) perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ~ICANON");
    return buf;
}

std::string parseJSONC(const std::string &filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return "{}";

    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::string cleanJson;
    cleanJson.reserve(source.size());

    bool inString = false;
    bool inSingleLineComment = false;
    bool inMultiLineComment = false;

    for (size_t i = 0; i < source.size(); ++i) {
        char c = source[i];
        char next = (i + 1 < source.size()) ? source[i + 1] : '\0';

        if (inSingleLineComment) {
            if (c == '\n') inSingleLineComment = false;
            continue;
        }

        if (inMultiLineComment) {
            if (c == '*' && next == '/') {
                inMultiLineComment = false;
                ++i;
            }
            continue;
        }

        if (!inString) {
            if (c == '/' && next == '/') {
                inSingleLineComment = true;
                ++i;
                continue;
            }
            if (c == '/' && next == '*') {
                inMultiLineComment = true;
                ++i;
                continue;
            }
        }

        if (c == '"' && (i == 0 || source[i - 1] != '\\')) {
            inString = !inString;
        }

        cleanJson += c;
    }

    return cleanJson;
}

#endif //ARTEX_INCLUDES_H