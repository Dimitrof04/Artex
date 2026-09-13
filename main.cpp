// 1. Biblioteca Padrão do C++ (STL)
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
    if (tcsetattr(0, TCSAD_NOW, &old) < 0) perror("tcsetattr ~ICANON");
    return buf;
}

// 2. Bibliotecas de Terceiros (External Dependencies)
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

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

static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

std::string base64_encode(const std::string &in) {
    std::string out;
    int val = 0, valb = -6;
    for (unsigned char c: in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

std::string base64_decode(const std::string &in) {
    std::string out;
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T[base64_chars[i]] = i;

    int val = 0, valb = -8;
    for (unsigned char c: in) {
        if (T[c] == -1) break;
        val = (val << 8) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

// Helper para gerenciar Escape de caracteres
// System Escaper
class ArtexEscaper {
public:
    static std::string encodeContent(const std::string &input) {
        std::string out = "";

        // 1. Escape de caracteres de controle
        for (char c: input) {
            if (c == '#') out += "##";
            else if (c == '$') out += "$$";
            else if (c == '`') out += "``";
            else out += c;
        }

        // Função Lambda auxiliar de substituição
        auto replaceAll = [](std::string &str, const std::string &from, const std::string &to) {
            size_t startPos = 0;
            while ((startPos = str.find(from, startPos)) != std::string::npos) {
                str.replace(startPos, from.length(), to);
                startPos += to.length();
            }
        };

        // 2. Loop Automático: substitui todas as palavras-chave cadastradas pelos seus tokens!
        for (const auto &token: artexTokens) {
            // Ignora tokens de sistema (pasta e arquivo)
            if (token.getCommand() == "FOLDER" || token.getCommand() == "FILE") continue;

            replaceAll(out, token.getCommand(), "$" + token.getFormation() + "`");
        }

        return out;
    }

    static std::string decodeContent(const std::string &input) {
        std::string out = input;

        auto replaceAll = [](std::string &str, const std::string &from, const std::string &to) {
            size_t startPos = 0;
            while ((startPos = str.find(from, startPos)) != std::string::npos) {
                str.replace(startPos, from.length(), to);
                startPos += to.length();
            }
        };

        // 1. Loop Automático: Reverte todos os tokens para as palavras-chave originais!
        for (const auto &token: artexTokens) {
            if (token.getCommand() == "FOLDER" || token.getCommand() == "FILE") continue;

            replaceAll(out, "$" + token.getFormation() + "`", token.getCommand());
        }

        // 2. Reverte os escapes de caracteres especiais
        std::string unescaped = "";
        for (size_t i = 0; i < out.length(); ++i) {
            if (i + 1 < out.length()) {
                if (out[i] == '#' && out[i + 1] == '#') {
                    unescaped += '#';
                    i++;
                    continue;
                }
                if (out[i] == '$' && out[i + 1] == '$') {
                    unescaped += '$';
                    i++;
                    continue;
                }
                if (out[i] == '`' && out[i + 1] == '`') {
                    unescaped += '`';
                    i++;
                    continue;
                }
            }
            unescaped += out[i];
        }

        return unescaped;
    }

    static std::string escape(const std::string &input) {
        std::string out = "";
        for (char c: input) {
            if (c == '#') out += "##";
            else if (c == '$') out += "$$";
            else if (c == '`') out += "``";
            else out += c;
        }
        return out;
    }
};

// ==========================================
// Estruturas de Dados
// ==========================================

class File {
private:
    std::string fileName;
    std::string typeFile; // ex: txt, cpp, json
    std::string content;
    std::string parent;

public:
    File(std::string name, std::string type, std::string content, std::string parent = "")
        : fileName(name), typeFile(type), content(content), parent(parent) {
    }

    std::string getName() const { return fileName; }
    std::string getType() const { return typeFile; }
    std::string getContent() const { return content; }
    std::string getParent() const { return parent; }

    void setContent(const std::string &newContent) { content = newContent; }
    void setParent(const std::string &newParent) { parent = newParent; }
    void rename(const std::string &newName) { fileName = newName; }

    // Serializa o arquivo para formato .artex
    // Serializa o arquivo para formato .artex
    std::string serialize() const {
        std::string escapedName = ArtexEscaper::escape(fileName);
        std::string escapedType = ArtexEscaper::escape(typeFile);

        // MUDANÇA AQUI: troca as palavras-chave (int, class, etc.) pelos tokens $VI`, $CC`, etc.
        std::string encodedContent = ArtexEscaper::encodeContent(content);

        std::string escapedParent = ArtexEscaper::escape(parent);

        return "$" + getTokenFormation("FILE") + "`" + escapedName + "|" + escapedType + "|\n" + encodedContent + "|" +
               escapedParent + "`\n";
    }
};

class BuilderFolder {
private:
    std::string folderName;
    int folderID;
    int parentID;

    std::vector<BuilderFolder> subFolders;
    std::vector<File> files;

public:
    BuilderFolder(std::string folderName, int folderID, int parentID = -1)
        : folderName(folderName), folderID(folderID), parentID(parentID) {
    }

    void rename(std::string name) { folderName = name; }
    int getID() const { return folderID; }
    int getParentID() const { return parentID; }
    std::string getName() const { return folderName; }

    void addSubFolder(const BuilderFolder &folder) { subFolders.push_back(folder); }
    void addFile(const File &file) { files.push_back(file); }

    std::string serialize() const {
        std::ostringstream ss;
        std::string escapedName = ArtexEscaper::escape(folderName);

        // Header da pasta: $D`id|parent_id|nome`
        ss << "$" << getTokenFormation("FOLDER") << "`" << folderID << "|" << parentID << "|" << escapedName << "`\n";
        for (const auto &file: files) {
            ss << "  " << file.serialize() << "\n";
        }

        for (const auto &folder: subFolders) {
            ss << folder.serialize();
        }

        return ss.str();
    }
};

// ==========================================
// Empacotador Automático de Pastas Reais
// ==========================================
class ArtexBuilder {
private:
    inline static int idCounter;

    // Função auxiliar para verificar se a extensão deve ser ignorada
    static bool isIgnoredExtension(const std::string &ext) {
        // Converte a extensão para minúsculas
        std::string lowerExt = ext;
        std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(), ::tolower);

        // Lista de extensões binárias/compiladas perigosas
        static const std::set<std::string> ignoredExtensions = {
            "exe", "dll", "so", "dylib", "a", "lib", "o", "obj",
            "out", "bin", "class", "pyc", "pyd", "elf", "sh"
        };

        return ignoredExtensions.count(lowerExt) > 0;
    }

    static void scanDirectoryRecursively(const fs::path &basePath, const fs::path &currentPath,
                                         BuilderFolder &currentFolder) {
        for (const auto &entry: fs::directory_iterator(currentPath)) {
            // Ignora o próprio arquivo .artex
            if (entry.path().extension() == ".artex") continue;

            if (fs::is_directory(entry)) {
                int newID = ++idCounter;
                std::string relPath = fs::relative(entry.path(), basePath).string();
                BuilderFolder subFolder(relPath, newID, currentFolder.getID());

                scanDirectoryRecursively(basePath, entry.path(), subFolder);

                // Só adiciona a pasta se ela contiver arquivos ou subpastas
                currentFolder.addSubFolder(subFolder);
            } else if (fs::is_regular_file(entry)) {
                std::string ext = entry.path().extension().string();
                if (!ext.empty() && ext[0] == '.') ext = ext.substr(1);

                // FILTRO: Ignora se for um arquivo compilado/binário
                if (isIgnoredExtension(ext)) {
                    std::cout << "[IGNORADO BINÁRIO] " << entry.path().filename().string() << std::endl;
                    continue;
                }

                std::string filename = entry.path().stem().string();

                std::ifstream inFile(entry.path(), std::ios::in | std::ios::binary);
                std::string content = "";
                if (inFile) {
                    content = std::string((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
                }

                std::string relParent = fs::relative(entry.path().parent_path(), basePath).string();
                if (relParent == ".") relParent = "root";

                File fileObj(filename, ext, content, relParent);
                currentFolder.addFile(fileObj);
            }
        }
    }

public:
    static bool packToArtex(const std::string &inputPathStr) {
        fs::path inputPath(inputPathStr);
        if (!fs::exists(inputPath)) {
            std::cerr << "[ERRO] O caminho especificado nao existe: " << inputPathStr << std::endl;
            return false;
        }

        idCounter = 0;
        fs::path outputPath = inputPath;

        if (fs::is_directory(inputPath)) {
            outputPath += ".artex";
            BuilderFolder rootFolder(inputPath.filename().string(), idCounter, -1);
            scanDirectoryRecursively(inputPath, inputPath, rootFolder);

            std::ofstream outFile(outputPath, std::ios::binary);
            if (!outFile.is_open()) return false;
            outFile << "# Artex Archive - Generated automatically\n";
            outFile << rootFolder.serialize();
            outFile.close();
        } else {
            std::string ext = inputPath.extension().string();
            if (!ext.empty() && ext[0] == '.') ext = ext.substr(1);

            // Verifica se o arquivo único é binário
            if (isIgnoredExtension(ext)) {
                std::cerr << "[ERRO] Arquivos compilados/binarios nao sao permitidos: " << inputPathStr << std::endl;
                return false;
            }

            outputPath.replace_extension(".artex");

            std::ifstream inFile(inputPath, std::ios::in | std::ios::binary);
            std::string content = "";
            if (inFile) {
                content = std::string((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
            }

            File singleFile(inputPath.stem().string(), ext, content, "SINGLE_FILE");

            std::ofstream outFile(outputPath, std::ios::binary);
            if (!outFile.is_open()) return false;
            outFile << "# Artex Single File\n";
            outFile << singleFile.serialize();
            outFile.close();
        }

        std::cout << "[ARTEX] Arquivo gerado com sucesso em: " << outputPath.string() << std::endl;
        return true;
    }
};

class ArtexUnpacker {
public:
    static bool unpackFromArtex(const std::string &artexFilePath) {
        fs::path artexPath(artexFilePath);
        if (!fs::exists(artexPath) || artexPath.extension() != ".artex") {
            std::cerr << "[ERRO] Arquivo .artex invalido ou nao encontrado.\n";
            return false;
        }

        std::ifstream inFile(artexPath, std::ios::in | std::ios::binary);
        if (!inFile.is_open()) return false;

        std::string fullContent((std::istreambuf_iterator<char>(inFile)),
                                std::istreambuf_iterator<char>());
        inFile.close();

        std::string folderToken = "$" + getTokenFormation("FOLDER") + "`";
        std::string fileToken = "$" + getTokenFormation("FILE") + "`";

        // Verifica se é um arquivo único ou pasta
        bool isSingleFileMode = (fullContent.find(folderToken) == std::string::npos);

        fs::path outputDir = artexPath.parent_path();
        if (!isSingleFileMode) {
            outputDir /= artexPath.stem(); // Para pastas, cria o diretório com o nome do projeto
            fs::create_directories(outputDir);
        }

        const int larguraBarra = 20;

        size_t totalBytes = fullContent.length();
        size_t pos = 0;
        size_t ultimoProgressoInt = 0; // Evita redesenhar a barra se a porcentagem não mudou

        while (pos < totalBytes) {
            // 1. Processa Pastas ($D`id|parent|caminho_relativo`)
            if (!isSingleFileMode && fullContent.compare(pos, folderToken.length(), folderToken) == 0) {
                pos += folderToken.length();
                size_t endTag = fullContent.find('`', pos);
                if (endTag == std::string::npos) break;

                std::string header = fullContent.substr(pos, endTag - pos);
                pos = endTag + 1;

                std::stringstream ss(header);
                std::string id, parentId, folderPathStr;

                if (std::getline(ss, id, '|') && std::getline(ss, parentId, '|') && std::getline(ss, folderPathStr)) {
                    folderPathStr = ArtexEscaper::decodeContent(folderPathStr);

                    if (parentId != "-1" && !folderPathStr.empty()) {
                        fs::path subFolderPath = outputDir / folderPathStr;
                        fs::create_directories(subFolderPath);
                    }
                }
            }
            // 2. Processa Arquivos ($F`nome|tipo|conteudo|parent`)
            else if (fullContent.compare(pos, fileToken.length(), fileToken) == 0) {
                pos += fileToken.length();
                size_t endTag = fullContent.find('`', pos);
                if (endTag == std::string::npos) break;

                std::string dataBlock = fullContent.substr(pos, endTag - pos);
                pos = endTag + 1;

                size_t p1 = dataBlock.find('|');
                size_t p2 = dataBlock.find('|', p1 + 1);
                size_t p3 = dataBlock.rfind('|');

                if (p1 != std::string::npos && p2 != std::string::npos && p3 != std::string::npos && p2 < p3) {
                    std::string rawName = dataBlock.substr(0, p1);
                    std::string rawType = dataBlock.substr(p1 + 1, p2 - p1 - 1);
                    std::string rawContent = dataBlock.substr(p2 + 1, p3 - p2 - 1);
                    std::string rawParent = dataBlock.substr(p3 + 1);

                    std::string realName = ArtexEscaper::decodeContent(rawName);
                    std::string realType = ArtexEscaper::decodeContent(rawType);
                    std::string realContent = ArtexEscaper::decodeContent(rawContent);
                    std::string realParent = ArtexEscaper::decodeContent(rawParent);

                    fs::path targetFolder = outputDir;

                    if (realParent != "SINGLE_FILE" && realParent != "root" && !realParent.empty()) {
                        targetFolder = outputDir / realParent;
                        fs::create_directories(targetFolder);
                    }

                    fs::path filePath = targetFolder / (realType.empty() ? realName : (realName + "." + realType));

                    std::ofstream outFile(filePath, std::ios::binary);
                    if (outFile.is_open()) {
                        outFile << realContent;
                        outFile.close();
                        // Nota: Removido o print antigo para não quebrar a barra visualmente
                    }
                }
            } else {
                pos++;
            }

            // ==========================================
            // CÁLCULO E EXIBIÇÃO DA BARRA DE PROGRESSO
            // ==========================================
            float progresso = static_cast<float>(pos) / totalBytes;
            size_t progressoInt = static_cast<size_t>(progresso * 100);

            // Só redesenha se a porcentagem mudou (melhora muito a performance)
            if (progressoInt != ultimoProgressoInt || pos == totalBytes) {
                ultimoProgressoInt = progressoInt;
                int posicaoBarra = larguraBarra * progresso;

                std::cout << "\rDesempacotando: [";
                for (int j = 0; j < larguraBarra; ++j) {
                    if (j < posicaoBarra) std::cout << "=";
                    else if (j == posicaoBarra) std::cout << ">";
                    else std::cout << " ";
                }
                std::cout << "] " << progressoInt << "%";
                std::cout.flush();
            }
        }

        std::cout << "\n[ARTEX] Concluido com sucesso!\n";
        return true;
    }
};

// ==========================================
// JSONC PARSER (REMOVE COMENTÁRIOS)
// ==========================================
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

// ==========================================
// VERSION MANAGER CLASS
// ==========================================

bool Noconfirmroot = false;

class VersionManager {
private:
    const std::string rootPath = "/artex";
    const std::string gitSavePath = "/artex/gitsave";
    const std::string jsonSavesPath = "/artex/jsonsaves";
    const std::string versionsFile = "/artex/versions.txt";
    const std::string configFile = "/artex/localfiles/ArtexBuild.jsonc";

    void ensureDirectoriesExist() {
        fs::create_directories(rootPath);
        fs::create_directories(gitSavePath);
        fs::create_directories(jsonSavesPath);
        fs::create_directories(rootPath + "/localfiles");
        fs::create_directories(rootPath + "/lastBackup");

        if (!fs::exists(gitSavePath + "/.git")) {
            printf("[Artex] Inicializando repositório Git em %s...\n", gitSavePath.c_str());
            std::system(("cd " + gitSavePath + " && git init && git config user.name 'Artex' && git config user.email 'artex@system.local'").c_str());
        }

        if (!fs::exists(versionsFile)) {
            std::ofstream file(versionsFile);
            file.close();
        }
    }

    std::string generateRandomCode(size_t length = 12) {
        const std::string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<> dist(0, chars.size() - 1);

        std::string code;
        for (size_t i = 0; i < length; ++i) {
            code += chars[dist(generator)];
        }
        return code;
    }

    // Navega com segurança no JSONC até 'limitepackages' dentro de "coonfig file" -> "configs"
    int getSnapshotLimit() {
        std::string jsonContent = parseJSONC(configFile);
        try {
            json j = json::parse(jsonContent);
            if (j.contains("config")) {
                return j["config"].get<int>();
            }
        } catch (const std::exception &e) {
            printf("[Artex] Erro ao ler limite de snapshots: %s. Usando padrão (5).\n", e.what());
        }
        return 5;
    }

    std::vector<std::string> getActiveVersions() {
        std::vector<std::string> versions;
        std::ifstream file(versionsFile);
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) versions.push_back(line);
        }
        return versions;
    }

    void writeActiveVersions(const std::vector<std::string> &versions) {
        std::ofstream file(versionsFile, std::ios::trunc);
        for (const auto &v: versions) {
            file << v << "\n";
        }
    }

public:
    VersionManager() {
        ensureDirectoriesExist();
    }

    std::string createVersion(const std::string &customName = "") {
        std::string code = generateRandomCode(12);
        int limit = getSnapshotLimit();

        printf("[Artex] Copy configs files configs");
        std::system("rm -rf /artex/gitsave/config");
        std::system("cp -r ~/.config /artex/gitsave/config");

        printf("[Artex] Copy localfiles (Artexbuild)\n");
        std::string cpCmd = "cp -r " + rootPath + "/localfiles/* " + gitSavePath + "/ 2>/dev/null";
        std::system(cpCmd.c_str());

        printf("[Artex] Update Last Backup...\n");
        std::string backupCmd = "cp -r " + gitSavePath + "/* " + rootPath + "/lastBackup/ 2>/dev/null";
        std::system(backupCmd.c_str());

        printf("[Artex] Save Backup...\n");
        std::string gitCmd = "cd " + gitSavePath + " && git add . && git commit -m '" + code + "'";
        int gitResult = std::system(gitCmd.c_str());

        if (gitResult != 0) {
            printf("[Warn] Commit no Git retornou status diferente de zero.\n");
        }

        // Lê a configuração atual para embutir na chave "jsons"
        std::string jsonRaw = parseJSONC(configFile);
        json parsedConfig;
        try {
            parsedConfig = json::parse(jsonRaw);
        } catch (...) {
            parsedConfig = json::object();
        }

        // Estrutura solicitada para o jsonsaves
        json stateJson;
        stateJson["code"] = code;
        stateJson["custom_name"] = customName.empty() ? "idk" : customName;
        stateJson["timestamp"] = std::time(nullptr);
        stateJson["jsons"] = parsedConfig;

        std::ofstream jsonFile(jsonSavesPath + "/" + code + ".json");
        jsonFile << stateJson.dump(2);
        jsonFile.close();

        // Gerencia rotação de snapshots
        auto versions = getActiveVersions();
        versions.push_back(code);

        if (limit != -1 && versions.size() > static_cast<size_t>(limit)) {
            std::string oldestCode = versions.front();
            fs::remove(jsonSavesPath + "/" + oldestCode + ".json");
            versions.erase(versions.begin());

            printf("[Artex] Limite de snapshots (%d) atingido. Snapshot removido: %s\n", limit, oldestCode.c_str());
        }

        writeActiveVersions(versions);
        printf("[Artex] Versão %s criada com sucesso!\n", code.c_str());
        return code;
    }

    void listVersionsGit() {
        printf("=== Histórico de Versões (Git) ===\n");
        std::string gitCmd = "cd " + gitSavePath + " && git log --oneline";
        std::system(gitCmd.c_str());
    }

    void listVersionsJson() {
        printf("=== Snapshots Salvos em JSON (%s) ===\n", jsonSavesPath.c_str());
        for (const auto &entry: fs::directory_iterator(jsonSavesPath)) {
            if (entry.path().extension() == ".json") {
                std::ifstream f(entry.path());
                try {
                    json j = json::parse(f);
                    std::string code = j.value("code", "N/A");
                    std::string name = j.value("custom_name", "N/A");
                    long long ts = j.value("timestamp", 0LL);
                    printf("- Code: %s | Name: %s | Timestamp: %lld\n", code.c_str(), name.c_str(), ts);
                } catch (...) {
                    printf("- [Erro ao ler %s]\n", entry.path().filename().string().c_str());
                }
            }
        }
    }

    bool rollbackGit(int index) {
        printf("[Artex] Realizando rollback via Git para o commit index ~ %d...\n", index);
        std::string gitCmd = "cd " + gitSavePath + " && git checkout HEAD~" + std::to_string(index);
        int result = std::system(gitCmd.c_str());

        if (result == 0) {
            std::string syncCmd = "cp -r " + gitSavePath + "/* " + rootPath + "/localfiles/";
            std::system(syncCmd.c_str());
            printf("[Artex] Rollback concluído! Executando build para aplicar atualizações...\n");
            buildSystem();
            return true;
        }

        printf("[Artex Erro] Falha ao executar rollback no Git para a posição relativa %d.\n", index);
        return false;
    }

    bool rollbackJson(int index) {
        auto versions = getActiveVersions();
        if (versions.empty()) {
            printf("[Artex Erro] Nenhum snapshot encontrado em versions.txt.\n");
            return false;
        }

        int targetIndex = static_cast<int>(versions.size()) - 1 - index;
        if (targetIndex < 0 || targetIndex >= static_cast<int>(versions.size())) {
            printf("[Artex Erro] Índice de snapshot inválido. Opções disponíveis: 0 até %zu\n", versions.size() - 1);
            return false;
        }

        std::string code = versions[targetIndex];
        printf("[Artex] Restaurando snapshot JSON code: %s...\n", code.c_str());

        std::string gitCmd = "cd " + gitSavePath + " && git checkout " + code;
        int result = std::system(gitCmd.c_str());

        if (result == 0) {
            std::string syncCmd = "cp -r " + gitSavePath + "/* " + rootPath + "/localfiles/";
            std::system(syncCmd.c_str());
            printf("[Artex] Restauração JSON concluída! Executando build...\n");
            buildSystem();
            return true;
        }

        printf("[Artex Erro] Código %s não localizado no repositório.\n", code.c_str());
        return false;
    }

    void rollbackLastBackup() {
        printf("[Artex] Restaurando o último backup local (%s/lastBackup)...\n", rootPath.c_str());
        std::string syncCmd = "cp -r " + rootPath + "/lastBackup/* " + rootPath + "/localfiles/ 2>/dev/null";
        std::system(syncCmd.c_str());
        printf("[Artex] Restauração concluída com sucesso.\n");
    }

    void buildSystem() {
        printf("[Artex] Salvando estado atual antes de aplicar build...\n");

        printf("[Artex] Lendo arquivo JSONC: %s...\n", configFile.c_str());
        std::string jsonRaw = parseJSONC(configFile);

        try {
            json config = json::parse(jsonRaw);

            // 1. Aplica hostname (está na raiz)
            if (config.contains("hostname") && config["hostname"].is_string()) {
                std::string hostname = config["hostname"].get<std::string>();
                printf("[Artex] Set host name %s\n", hostname.c_str());
                std::string hostCmd = "sudo hostnamectl set-hostname " + hostname;
                std::system(hostCmd.c_str());
            } else {
                printf("[Warn] Chave 'hostname' não encontrada na raiz do JSON.\n");
            }

            // 2. Instala pacotes via yay (está na raiz)
            if (config.contains("packages") && config["packages"].is_array()) {
                printf(" -> Instalando pacotes definidos via Yay...\n");
                for (const auto &pkg: config["packages"]) {
                    std::string pkgName = pkg.get<std::string>();
                    printf(" -> Instalando pacote: %s\n", pkgName.c_str());
                    std::string installCmd = "yay -S --noconfirm " + pkgName;
                    std::system(installCmd.c_str());
                }
            } else {
                printf("[AVISO] Lista 'packages' não encontrada ou inválida na raiz do JSON.\n");
            }

            // 3. Instala pacotes via flathub
            if (config.contains("flatpak") && config["flatpak"].is_array()) {
                printf(" -> Instalando pacotes definidos via Yay...\n");
                for (const auto &pkg: config["flatpak"]) {
                    std::string pkgName = pkg.get<std::string>();
                    printf(" -> Instalando pacote: %s\n", pkgName.c_str());
                    std::string installCmd = "flatpak install flathub " + pkgName;
                    std::system(installCmd.c_str());
                }
            } else {
                printf("[AVISO] Lista 'packages' não encontrada ou inválida na raiz do JSON.\n");
            }

            // 4. Lê as configurações internas (do bloco "config" se precisar)
            if (config.contains("config") && config["config"].is_object()) {
                auto innerConfig = config["config"];
                if (innerConfig.contains("limitepackages")) {
                    int limit = innerConfig["limitepackages"].get<int>();
                    printf(" -> Limite de snapshots configurado para: %d\n", limit);
                }
            }
        } catch (const json::exception &e) {
            printf("[ERRO] Falha ao processar o JSON: %s\n", e.what());
        }

        char updatesys = 'Y';
        
        if (!Noconfirmroot) {
            printf("[Artex] Do you want to update your sys? [Y/n]");
            try {
                updatesys = getch();
                printf("%c\n", updatesys); // Imprime a tecla que o usuário apertou e pula linha
            } catch (...) {
                updatesys = 'Y';
            }
        }

        
        if (updatesys != 'N' && updatesys != 'n') {
            system("yay -Syu --noconfirm");
        }

        if (!Noconfirmroot) {
            printf("[Artex] Do you want to update your flatpak apps? [Y/n]");
            try {
                updatesys = getch();
                printf("%c\n", updatesys);
            } catch (...) {
                updatesys = 'Y';
            }
        }

        if (updatesys != 'N' && updatesys != 'n') {
            system("flatpak update -y");
        }

        createVersion("build_auto_save");
        printf("[Artex] Processo de build finalizado!\n");
    }
};

// ==========================================
// CLI ENTRY POINT
// ==========================================
void HelpPrint() {
    printf("Artex Manager\n");
    printf("Options:\n");
    printf("--help    | -h       Show Help Painel\n");
    printf("--version | -v       Show Artex version\n");
    printf("--build              Save current state and apply JSONC configurations\n");
    printf("--build -y           --build + noconfirm\n");
    printf("--save [name]        Create a new snapshot and save history\n");
    printf("--lv-git             Show all saves/commits in Git\n");
    printf("--lv-json            Show all saves stored in the jsonsaves directory\n");
    printf("--rb                 Restore the state of the last local backup in lastBackup\n");
    printf("--rb-git n           Go back N commits in Git and run build\n");
    printf("--rb-json n          Go back N snapshots via JSON and run build\n");
    printf("--Ca                 Create / Compile a file or folder into .artex\n");
    printf("--Ra                 ExtrDact a .artex file\n");
    printf("--upd                Update Artex");
    printf("--uninstall          Uninstall Artex\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        HelpPrint();
        return 1;
    }

    bool sudo = (getuid() == 0);

    if (sudo) {
        printf("[Artex] running witg root (sudo)");
    }

    std::string command = argv[1];
    VersionManager manager;
    if (command == "--version") {
        printf("[Artex] : version 1.1.0");
        return 0;
    } if (command == "--help" || command == "-h"){
        HelpPrint();
        return 0;
    } if (command == "--lv-git") {
        manager.listVersionsGit();
        return 0;
    } if (command == "--lv-json") {
        manager.listVersionsJson();
        return 0;
    } if (command == "--Ca") {
        if (argc < 3) {
            printf("[Erro]: Informe o caminho para empacotar.\n");
            return 1;
        }
        if (sudo) {
            printf("[Warn] the file will be created by sudo, Do you really want to use sudo?");
            auto reposta = getch();
            printf("%c\n", reposta);
            if (reposta != 'Y' && reposta != 'y' ) {
                return 0;
            }
        }
        std::string targetPath = argv[2];
        ArtexBuilder::packToArtex(targetPath);
        return 0;
    } if (command == "--Ra") {
        if (argc < 3) {
            printf("[Erro]: Informe o caminho para empacotar.\n");
            return 1;
        }
        std::string targetPath = argv[2];
        ArtexUnpacker::unpackFromArtex(targetPath);
        return 0;
    }
    if (command == "--build") {
        if (argc >= 2 && std::string(argv[1]) == "-y") {
            Noconfirmroot = true;
        }

    	if (sudo) {
			manager.buildSystem();
        	return 0;
		} else {
		    printf("[Error] Permission deniedr");
		    return 1;
		}
    } if (command == "--save") {
        if (sudo) {
            std::string name = (argc >= 3) ? argv[2] : "";
            manager.createVersion(name);
            return 0;
        } else {
            printf("[Error] Permission deniedr");
            return 1; 
        }
    } if (command == "--rb") {
        if (sudo) {
            manager.rollbackLastBackup();
            return 0;
        } else {
            printf("[Error] Permission deniedr");
            return 1; 
        }
    } if (command == "--rb-git") {
        if (sudo) {
            if (argc < 3) {
                printf("[Erro] Argumento de índice ausente. Exemplo: ArtexRecovery --roolback-git 2\n");
                return 1;
            }
            int index = std::atoi(argv[2]);
            manager.rollbackGit(index);
            return 0;
        } else {
            printf("[Error] Permission deniedr");
            return 1; 
        }
    } if (command == "--rb-json") {
        if (sudo){
            if (argc < 3) {
                printf("[Erro] Argumento de índice ausente. Exemplo: ArtexRecovery --roolback-json 1\n");
                return 1;
            }
            int index = std::atoi(argv[2]);
            manager.rollbackJson(index);
            return 0;
        } else {
            printf("[Error] Permission deniedr");
            return 1; 
        }
    }
    if (command == "--upd") {
        printf("[1] - Use git pull  for update");
        printf("[2] - Use git clone for update");
        printf("[3] - Compiling Artex");
        printf("[0] - return");

        int option = 0;
        try {
            std::cin >> option;
        } catch (...) {}

        if (option == 1) {
            system("git pull");
        } else if (option == 2) {
            system("rm -rf ~/artex && git clone https://github.com/Dimitrof04/Artex.git");
        } else {
            printf("nothing happened");
            return 1;
        }

        system("cd ~/artex && g++ -std=c++17 main.cpp -I include -o Artex");

        printf("[Artex] Successfully updated");
        return 0;
    } if (command == "--uninstall") {
        if (sudo) {
            printf("[1] - Confirm \n");
            printf("[2] - Unistall but dont erase my data \n");
            printf("[0] - exit \n");

            std::string chose;
            std::cin >> chose;

            if (chose == "1") {
                system("rm -rf /artex");
                system("rm -rf /usr/local/bin/Artex");
                printf("[Artex] bye bye");
            } else if (chose == "2") {
                system("rm -rf /usr/local/bin/Artex ");
            } else {
                printf("nothing happened");
                return 0;
            }
            return 0;
        } else {
            printf("[Error] Permission deniedr");
            return 1; 
        }
    }

    std::cout << "[Artex] unrecognized or invalid command " << command << std::endl;
    printf("for help use Artex --help\n");

    return 0;
}