# Salle de reference

Cette scene unique sert de base aux exercices 8, 9 et 10 : piece de 4 x 4 x 2,5 m, table de 75 cm, pieds, cube repere, camera a 1,70 m et soleil directionnel.

`main.cpp` utilise Nkentseu. Pour le compiler, copiez-le dans `Nkentseu/Tutoriels3D/06-Salle/main.cpp`, puis ajoutez une cible dans `Tutoriels3D.jenga` : `tutoproject("SalleReference", ["06-Salle/main.cpp"])`. Cette cible doit lier les modules NKWindow, NKEvent, NKRHI et NKRenderer, comme les autres tutoriels 3D.

## Variantes

- Sous-systemes : `SALLE_SYSTEMES=ALL` active tout; toute autre valeur, ou variable absente, active seulement `RENDER3D` et `SHADOW`.
- Direction du soleil : `SALLE_SUN_X`, `SALLE_SUN_Y`, `SALLE_SUN_Z` (defaut : `-0.4`, `-1`, `-0.3`).
- Intensite : `SALLE_INTENSITE` (defaut : `3`).
- Ombres : `SALLE_CAST_SHADOW=0` desactive les ombres; toute autre valeur les active.

Pour les captures de l'exercice 10, ne changez qu'une seule de ces variables entre deux lancements. Ce depot ne contient pas Nkentseu/Jenga ni un compilateur C++; la scene n'a donc pas pu etre construite ni capturee ici.
