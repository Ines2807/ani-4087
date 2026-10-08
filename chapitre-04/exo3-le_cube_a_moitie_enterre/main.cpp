#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

int main() {
    int N;
    // Nombre de cubes à traiter.
    cin >> N;

    int aCorriger = 0;
    // Nombre de cubes dont le verdict n'est pas "POSE".
    int pire = 0;
    // Plus grande distance entre le bas d'un cube et le sol, en valeur absolue.

    for (int i = 0; i < N; ++i) {
        string name;
        int e;
        int y;
        // Lecture du nom du cube, de son échelle e et de la hauteur y de son centre.
        cin >> name >> e >> y;

        // La hauteur du cube vaut e millimètres.
        // Son demi-côté est donc e / 2.
        const int demiHauteur = e / 2;

        // Le bas et le haut du cube sont calculés à partir du centre y.
        const int bas = y - demiHauteur;
        const int haut = y + demiHauteur;

        // Pour poser le cube sur le sol, il suffit que son centre soit à la moitié de sa hauteur.
        const int hauteurPose = demiHauteur;

        string verdict;
        // Les tests doivent suivre l'ordre imposé par l'énoncé.
        // SOUS LE SOL a priorité sur ENTERRE, car un cube est entièrement sous le sol
        // si son haut est négatif ou nul.
        if (haut <= 0) {
            verdict = "SOUS LE SOL";
        } else if (bas < 0) {
            verdict = "ENTERRE";
        } else if (bas == 0) {
            verdict = "POSE";
        } else {
            verdict = "FLOTTE";
        }

        // Les cubes non posés doivent être comptés dans le bilan final.
        if (verdict != "POSE") {
            ++aCorriger;
        }

        // Le pire écart correspond à la plus grande distance du bas au sol.
        pire = max(pire, abs(bas));

        // Affichage d'une ligne pour ce cube : nom, bas, haut, verdict, hauteur du centre pour le poser.
        cout << name << ' ' << bas << ' ' << haut << ' ' << verdict << ' ' << hauteurPose << '\n';
    }

    // Bilan final demandé par l'énoncé.
    cout << "A CORRIGER " << aCorriger << '\n';
    cout << "PIRE " << pire << '\n';

    return 0;
}
