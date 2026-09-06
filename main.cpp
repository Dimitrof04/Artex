#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <unistd.h>
#include <cstdio>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

// ==========================================
// JSONC PARSER (REMOVE COMENTÁRIOS)
// ==========================================
std::string parseJSONC(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return "{}";

    std::string source((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
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
            if (j.contains("config file") &&
                j["config file"].contains("configs") &&
                j["config file"]["configs"].contains("limitepackages")) {
                return j["config file"]["configs"]["limitepackages"].get<int>();
            }
        } catch (const std::exception& e) {
            printf("[Artex Warning] Falha ao ler limite de snapshots no JSONC: %s. Usando o padrão (5).\n", e.what());
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

    void writeActiveVersions(const std::vector<std::string>& versions) {
        std::ofstream file(versionsFile, std::ios::trunc);
        for (const auto& v : versions) {
            file << v << "\n";
        }
    }

public:
    VersionManager() {
        ensureDirectoriesExist();
    }

    std::string createVersion(const std::string& customName = "") {
        std::string code = generateRandomCode(12);
        int limit = getSnapshotLimit();

        printf("[Artex] Copiando arquivos de localfiles para gitsave...\n");
        std::string cpCmd = "cp -r " + rootPath + "/localfiles/* " + gitSavePath + "/ 2>/dev/null";
        std::system(cpCmd.c_str());

        printf("[Artex] Atualizando último backup...\n");
        std::string backupCmd = "cp -r " + gitSavePath + "/* " + rootPath + "/lastBackup/ 2>/dev/null";
        std::system(backupCmd.c_str());

        printf("[Artex] Salvando alterações no repositório Git...\n");
        std::string gitCmd = "cd " + gitSavePath + " && git add . && git commit -m '" + code + "'";
        int gitResult = std::system(gitCmd.c_str());

        if (gitResult != 0) {
            printf("[Artex Warning] Commit no Git retornou status diferente de zero.\n");
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
        for (const auto& entry : fs::directory_iterator(jsonSavesPath)) {
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
        createVersion("build_auto_save");

        printf("[Artex] Lendo arquivo JSONC: %s...\n", configFile.c_str());
        std::string jsonRaw = parseJSONC(configFile);

        try {
            json config = json::parse(jsonRaw);

            // Acessa a raiz "coonfig file" configurada
            if (config.contains("config")) {
                auto targetConfig = config["config file"];

                // Aplica hostname
                if (targetConfig.contains("hostname")) {
                    std::string hostname = targetConfig["hostname"].get<std::string>();
                    printf(" -> Defina Hostname do sistema: %s\n", hostname.c_str());
                    std::string hostCmd = "sudo hostnamectl set-hostname " + hostname;
                    std::system(hostCmd.c_str());
                } else {
                    printf("[AVISO] Chave 'hostname' não encontrada dentro de 'config file'.\n");
                }

                // Instala pacotes via yay
                if (targetConfig.contains("packages") && targetConfig["packages"].is_array()) {
                    printf(" -> Instalando pacotes definidos via Yay...\n");
                    for (const auto& pkg : targetConfig["packages"]) {
                        std::string pkgName = pkg.get<std::string>();
                        printf(" -> Instalando pacote: %s\n", pkgName.c_str());
                        std::string installCmd = "yay -S --noconfirm " + pkgName;
                        std::system(installCmd.c_str());
                    }
                } else {
                    printf(" [AVISO] Lista 'packages' vazia ou não encontrada em 'config file'.\n");
                }
            } else {
                printf("[ERRO]  Chave raiz 'config file' não encontrada no arquivo JSONC.\n");
            }

        } catch (const std::exception& e) {
            printf("[Artex Erro] Falha no parseamento do JSONC durante o build: %s\n", e.what());
        }

        printf("[Artex] Processo de build finalizado!\n");
    }
};

// ==========================================
// CLI ENTRY POINT
// ==========================================
void printUsage() {
    printf("Artex System Recovery Manager\n");
    printf("Uso: ArtexRecovery [opção]\n");
    printf("Opções:\n");
    printf("--build              Salva o estado atual e aplica as configurações do JSONC\n");
    printf("--save [nome]        Cria um novo snapshot e salva histórico\n");
    printf("--listversions-git   Mostra todos os saves/commits no Git\n");
    printf("--listversions-json  Mostra todos os saves salvos no diretório jsonsaves\n");
    printf("--roolback           Restaura o estado do último backup local em lastBackup\n");
    printf("--roolback-git <N>   Retorna N commits atrás no Git e roda o build\n");
    printf("--roolback-json <N>  Retorna N snapshots atrás via JSON e roda o build\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::string command = argv[1];
    VersionManager manager;


    if (command == "--listversions-git") {
        manager.listVersionsGit();
    }
    else if (command == "--listversion-json") {
        manager.listVersionsJson();
    }

    if (getuid() != 0) {
        printf("[Artex Erro] Acesso root necessário. Por favor, execute como sudo.\n");
        return 1;
    }

    if (command == "--build") {
        manager.buildSystem();
    }
    else if (command == "--save") {
        std::string name = (argc >= 3) ? argv[2] : "";
        manager.createVersion(name);
    }
    else if (command == "--roolback") {
        manager.rollbackLastBackup();
    }
    else if (command == "--roolback-git") {
        if (argc < 3) {
            printf("[Artex Erro] Argumento de índice ausente. Exemplo: ArtexRecovery --roolback-git 2\n");
            return 1;
        }
        int index = std::atoi(argv[2]);
        manager.rollbackGit(index);
    }
    else if (command == "--roolback-json") {
        if (argc < 3) {
            printf("[Artex Erro] Argumento de índice ausente. Exemplo: ArtexRecovery --roolback-json 1\n");
            return 1;
        }
        int index = std::atoi(argv[2]);
        manager.rollbackJson(index);
    }
    else {
        printf("[Artex Erro] Comando desconhecido: %s\n", command.c_str());
        printUsage();
        return 1;
    }

    return 0;
}