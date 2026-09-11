const body = document.getElementById("main-div"); // Adicionado o [0] no final
const summary = document.getElementById("summary");

let idStruture = 1

class ClasseStruture {
    // O constructor recebe o que antes era o parâmetro da função
    constructor(name) {
        this.name = name;

        // Criamos a lista e o título como propriedades do objeto (usando this)
        this.list = document.createElement("ul");
        this.list.classList.add("estruture");

        let title = document.createElement("h2");
        title.classList.add("info-ul-title");
        title.innerText = idStruture + " - " + name;

        summary.appendChild(this.list);
        this.list.appendChild(title);

        idStruture++;

        console.log(name + " foi criado com sucesso");
    }

    // A função interna vira um método da classe
    createinfo(text, id) {
        let li = document.createElement("li");
        li.innerText = "* " + text;

        if (!id){
            id = text;
        }

        // 1. Busca o elemento de destino na página pelo ID
        let idforsearch = document.getElementById(id);

        if (idforsearch) {
            // Opcional: Adiciona um clique na LI caso o usuário queira voltar lá depois
            li.addEventListener("click", () => {
                idforsearch.scrollIntoView({ behavior: "smooth", block: "start" });
            });
        }

        this.list.appendChild(li);

        // 2. Quando terminar tudo, faz a viagem automática até o destino
        if (idforsearch) {
            idforsearch.scrollIntoView({ behavior: "smooth", block: "start" });
        }

        console.log(text + " foi criado com sucesso");
        return li;
    }
}

let summarylists = {
    estrutura: {
        obj: new ClasseStruture("Estrutura"),
        list: [
            "Artex-Folder",
            "git-saves",
            "json-saves",
            "last-Backup",
            "local-files",
            "local-saves",
            "versions.txt"
        ]
    },
    // Nova coluna com os comandos do programa
    comandos: {
        obj: new ClasseStruture("Comandos"),
        list: [
            "--version | -v",
            "--build",
            "--save",
            "--listversions-git",
            "--listversions-json",
            "--rollback",
            "--rollback-git",
            "--rollback-json",
            "--Create-artex",
            "--Rebuild-artex",
            "--uninstall"
        ]
    },
    artexbuild: {
        obj: new ClasseStruture("ArtexBuild"),
        list: [
            "ArtexBuild",
            "processo",
            "keys"
        ]
    }
};

// O seu mesmo loop automático vai ler as duas colunas perfeitamente!
for (let item of Object.values(summarylists)) {
    let obj = item.obj;
    for (let i of item.list) { 
        obj.createinfo(i, i); 
    }
}
