# Exercice 1 - Projet minimal Jenga

## Fichier de projet Jenga

Fichier : `MaSalle/MaSalle.jenga`

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# MaSalle – Espace de travail Jenga
# Généré par `jenga workspace` le 2026-09-24 15:20:16

from Jenga import *

with workspace("MaSalle"):
    configurations(['Debug', 'Release'])
    targetoses([TargetOS.WINDOWS, TargetOS.LINUX, TargetOS.MACOS])
    targetarchs([TargetArch.X86_64])

    # Projet : ApplicationBonjour
    with project("ApplicationBonjour"):
        consoleapp()
        language("C++")
        location("ApplicationBonjour")
        files(["src/**.cpp", "include/**.hpp"])
```

## Programme minimal

Fichier : `MaSalle/ApplicationBonjour/src/main.cpp`

```cpp
#include <iostream>

int main() {
    std::cout << "bonjour" << std::endl;
    return 0;
}
```

## Sortie de `jenga build`

Commande exécutée :

```powershell
$env:Path = 'C:\Users\INES\AppData\Local\Temp\WinGet\MartinStorsjo.LLVM-MinGW.UCRT.22.1.8-20260616\extracted\llvm-mingw-20260616-ucrt-x86_64\bin;' + $env:Path
cd 'C:\Users\INES\Documents\chapitre-02\exo1-le_projet_minimal\MaSalle'
& 'C:\Users\INES\AppData\Local\Programs\Python\Python312\python.exe' -m Jenga install toolchain detect
& 'C:\Users\INES\AppData\Local\Programs\Python\Python312\python.exe' -m Jenga build --config Debug
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

Generated toolchain config: C:\Users\INES\Documents\Jenga\Jenga\Jenga\GlobalToolchains.py

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

Loading workspace...

Configuration: Debug
Target:        Windows x86_64
Toolchain:     clang-mingw

Build Order (1 projects):
  1. ApplicationBonjour [CONSOLE_APP]

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: ApplicationBonjour                                              Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
✓ All files up to date

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 5.48s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED                                 
════════════════════════════════════════════════════════════════════════════════
Projects Built:  1/1
Time:           5.50s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════
```

## Conclusion

Le projet minimal a bien été créé et compilé avec Jenga. La build est réussie et le binaire a été généré dans le dossier de sortie du projet.
