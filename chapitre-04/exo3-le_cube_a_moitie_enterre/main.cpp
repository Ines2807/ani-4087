#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

int main() {
    int N;
    cin >> N;

    int aCorriger = 0;
    int pire = 0;

    for (int i = 0; i < N; ++i) {
        string name;
        int e;
        int y;
        cin >> name >> e >> y;

        // Le cube est centre sur son origine : son bas est a y - e/2.
        const int demiHauteur = e / 2;
        const int bas = y - demiHauteur;
        const int haut = y + demiHauteur;

        string verdict;
        if (haut <= 0) {
            verdict = "SOUS LE SOL";
        } else if (bas < 0) {
            verdict = "ENTERRE";
        } else if (bas == 0) {
            verdict = "POSE";
        } else {
            verdict = "FLOTTE";
        }

        if (verdict != "POSE")
            ++aCorriger;

        // Pour poser ce cube, il faut monter son centre de la moitie de sa hauteur.
        const int hauteurPose = demiHauteur;
        pire = max(pire, abs(bas));

        cout << name << ' ' << bas << ' ' << haut << ' ' << verdict << ' ' << hauteurPose << '\n';
    }

    cout << "A CORRIGER " << aCorriger << '\n';
    cout << "PIRE " << pire << '\n';
    return 0;
}
