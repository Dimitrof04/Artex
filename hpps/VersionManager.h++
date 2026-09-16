//
// Created by lulu on 9/15/26.
//

#include "Includes.h++"

#ifndef ARTEX_VERSIONMANAGER_H
#define ARTEX_VERSIONMANAGER_H

// ==========================================
// VERSION MANAGER CLASS
// ==========================================

namespace fs = std::filesystem;
using json = nlohmann::json;

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

#endif //ARTEX_VERSIONMANAGER_H