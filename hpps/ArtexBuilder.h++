//
// Created by lulu on 9/15/26.
//

#ifndef ARTEX_ARTEXBUILDER_H
#define ARTEX_ARTEXBUILDER_H

#include "Includes.h++"

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

#endif //ARTEX_ARTEXBUILDER_H