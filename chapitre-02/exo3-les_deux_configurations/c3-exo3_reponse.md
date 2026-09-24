# Exercice 3 - Comparaison Debug / Release

## 1. Commandes exécutées

### Debug

```powershell
cd "C:\Users\INES\Documents\ani-4087\chapitre-02\exo3-les_deux_configurations\MaSalle"
& "C:\Users\INES\AppData\Local\Programs\Python\Python312\python.exe" -m Jenga build --config Debug
```

### Release

```powershell
cd "C:\Users\INES\Documents\ani-4087\chapitre-02\exo3-les_deux_configurations\MaSalle"
& "C:\Users\INES\AppData\Local\Programs\Python\Python312\python.exe" -m Jenga build --config Release
```

## 2. Valeurs mesurées

### Temps de construction

- Debug : 7.5881788 s
- Release : 6.4917281 s

### Taille des exécutables

- Debug : 166912 octets
- Release : 166912 octets

## 3. Comparaison

Les deux exécutables ont exactement la même taille dans ce cas très simple : 166912 octets.
La version Release est néanmoins un peu plus rapide : 6.4917281 s contre 7.5881788 s pour la version Debug.

## 4. Les quatre nombres

- 166912
- 166912
- 7.5881788
- 6.4917281
