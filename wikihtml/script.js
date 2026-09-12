const mainDiv = document.getElementById("main-div");
const summary = document.getElementById("summary");
const sidebarSummary = document.getElementById("sidebar-summary");

let idStruture = 1;

class ClasseStruture {
    constructor(name) {
        this.name = name;

        // Main Summary List
        this.list = document.createElement("ul");
        this.list.classList.add("estruture");

        let title = document.createElement("h2");
        title.classList.add("info-ul-title");
        title.innerText = idStruture + " - " + name;

        summary.appendChild(this.list);
        this.list.appendChild(title);

        // Sidebar Mini Summary List
        this.sidebarList = document.createElement("ul");
        this.sidebarList.classList.add("sidebar-ul");
        
        let sidebarTitle = document.createElement("h3");
        sidebarTitle.classList.add("sidebar-title");
        sidebarTitle.innerText = name;
        
        sidebarSummary.appendChild(sidebarTitle);
        sidebarSummary.appendChild(this.sidebarList);

        idStruture++;
    }

    createinfo(text, id) {
        if (!id) id = text;

        // Main summary item
        let li = document.createElement("li");
        li.innerText = "• " + text;

        // Sidebar mini-summary item
        let sidebarLi = document.createElement("li");
        sidebarLi.innerText = text;

        let targetElement = document.getElementById(id);

        if (targetElement) {
            const scrollAction = () => {
                targetElement.scrollIntoView({ behavior: "smooth", block: "start" });
            };
            li.addEventListener("click", scrollAction);
            sidebarLi.addEventListener("click", scrollAction);
        }

        this.list.appendChild(li);
        this.sidebarList.appendChild(sidebarLi);

        return li;
    }
}

let summarylists = {
    estrutura: {
        obj: new ClasseStruture("Structure"),
        list: [
            "Artex-Folder",
            "git-saves",
            "json-saves",
            "last-Backup",
            "local-files",
            "versions.txt"
        ]
    },
    comandos: {
        obj: new ClasseStruture("Commands"),
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
            "Artex-keys"
        ]
    }
};

// Generate summary items dynamically
for (let item of Object.values(summarylists)) {
    let obj = item.obj;
    for (let i of item.list) { 
        obj.createinfo(i, i); 
    }
}