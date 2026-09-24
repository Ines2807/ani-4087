# Exercice 2 - Jenga info avant build

## Structure du projet

```text
exo2-info_avant_build/
└── MaSalle/
    ├── MaSalle.jenga
    ├── pyrightconfig.json
    ├── .vscode/
    │   └── settings.json
    └── ApplicationBonjour/
        └── src/
            └── main.cpp
```

## Fichier de projet Jenga

```python
#!/usr/bin/env python3
# pyright: reportUndefinedVariable=false
# pyright: reportMissingImports=false
# pyright: reportGeneralTypeIssues=false
# pyright: reportAttributeAccessIssue=false

from Jenga import *

with workspace("MaSalle"):
    configurations(['Debug', 'Release'])
    targetoses([TargetOS.WINDOWS, TargetOS.LINUX, TargetOS.MACOS])
    targetarchs([TargetArch.X86_64])

    with project("ApplicationBonjour"):
        consoleapp()
        language("C++")
        location("ApplicationBonjour")
        files(["src/**.cpp", "include/**.hpp"])
```

## Programme minimal

```cpp
#include <iostream>

int main() {
    std::cout << "bonjour" << std::endl;
    return 0;
}
```

## Sortie de `jenga info`

Commande exécutée :

```powershell
cd "C:\Users\INES\Documents\ani-4087\chapitre-02\exo2-info_avant_build\MaSalle"
python -m Jenga info
```

Sortie réelle :

```text
╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║                ██╗███████╗███╗   ██╗ ██████╗  █████╗             ║
║                ██║██╔════╝████╗  ██║██╔════╝ ██╔══██╗            ║
║                ██║█████╗  ██╔██╗ ██║██║  ███╗███████║            ║
║           ██   ██║██╔══╝  ██║╚██╗██║██║   ██║██╔══██║            ║
║           ╚█████╔╝███████╗██║ ╚████║╚██████╔╝██║  ██║            ║
║            ╚════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝            ║
║                                                                  ║
║             Multi-platform C/C++ Build System v2.8.0             ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝

=========================== Jenga Workspace: MaSalle ===========================

Location: C:\Users\INES\Documents\ani-4087\chapitre-02\exo2-info_avant_build\MaSalle
Entry file: C:\Users\INES\Documents\ani-4087\chapitre-02\exo2-info_avant_build\MaSalle\MaSalle.jenga
Configurations: Debug, Release
Platforms: Windows
Target OSes: Windows, Linux, macOS
Target Architectures: x86_64


Projects
------------------------------------------------------------
Name                 Kind         Language   Test   External
============================================================
ApplicationBonjour   ConsoleApp   C++        No     No


Available Toolchains
------------------------------------------------------------
Name                Family   Target OS   Arch     Env  
=======================================================
host-clang          clang    Windows     x86_64   mingw
clang-mingw         clang    Windows     x86_64   mingw
clang-cross-linux   clang    Linux       x86_64   gnu


Daemon
------------------------------------------------------------
Status: Not running
```

## Ce que `jenga info` nous apprend que le fichier de projet ne dit pas explicitement

Le fichier Jenga donne la structure de base du projet, mais `jenga info` révèle le point de vue réel que Jenga a du projet avant la compilation.

Il nous apprend notamment :

- le nom exact du workspace : `MaSalle`
- le dossier racine analysé par Jenga
- le fichier d’entrée utilisé : `MaSalle.jenga`
- les configurations effectivement reconnues : `Debug` et `Release`
- les plateformes cibles réellement prises en compte : `Windows`, avec les OS de destination `Windows`, `Linux` et `macOS`
- l’architecture ciblée : `x86_64`
- le projet détecté : `ApplicationBonjour`, de type `ConsoleApp`, en `C++`
- les toolchains disponibles dans l’environnement : `host-clang`, `clang-mingw` et `clang-cross-linux`
- l’état du daemon Jenga : `Not running`

Le fichier de projet dit ce que l’on veut construire ; `jenga info` montre ce que Jenga a réellement compris et configuré avant la build.

C’est la différence entre la déclaration du projet et l’analyse effective du système de build.
