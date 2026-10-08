#include <iostream>
#include <string>
#include <cstdlib>

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

        const int demiHauteur = e / 2;
        const int bas = y - demiHauteur;
        const int haut = y + demiHauteur;
        const int hauteurPose = demiHauteur;

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

        if (verdict != "POSE") {
            ++aCorriger;
        }

        pire = max(pire, abs(bas));

        cout << name << ' ' << bas << ' ' << haut << ' ' << verdict << ' ' << hauteurPose << '\n';
    }

    cout << "A CORRIGER " << aCorriger << '\n';
    cout << "PIRE " << pire << '\n';

    return 0;
}
