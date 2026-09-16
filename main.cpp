#include "hpps/Includes.h++"

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
        printf("[Artex] : version 1.1.1");
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