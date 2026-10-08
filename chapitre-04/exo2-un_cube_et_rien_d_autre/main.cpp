#include <iostream>
#include <string>

using namespace std;

int main() {
    int N;
    cin >> N;

    // On ne compte que les cubes qui passent tous les controles.
    int visibles = 0;

    for (int i = 0; i < N; ++i) {
        string name;
        long long drapeaux;
        int sx, sy, sz;
        int distance;
        int lumieres;
        int ambiante;
        int proche;

        // On lit les options de rendu, la taille du cube et sa position par rapport a la camera.
        cin >> name >> drapeaux >> sx >> sy >> sz >> distance >> lumieres >> ambiante >> proche;

        string verdict;

        // Le bit 2 indique si le rendu 3D est actif. Sinon, inutile de verifier le cube.
        if ((drapeaux & 2LL) == 0) {
            verdict = "RENDER3D ETEINT";
        } else if (sx == 0 || sy == 0 || sz == 0) {
            verdict = "ECHELLE NULLE";
        } else {
            // Le cube est centre : son bord avant se trouve a distance - sa demi-profondeur.
            const int faceAvant = distance - sz / 2;
            if (faceAvant <= 0) {
                verdict = "CAMERA DANS LE CUBE";
            } else if (faceAvant < proche) {
                // Le cube commence avant le plan proche : la camera ne peut pas le dessiner entier.
                verdict = "COUPE PAR LE PLAN PROCHE";
            } else if (lumieres == 0 && ambiante == 0) {
                // Sans lumiere directe ni lumiere ambiante, le cube reste invisible.
                verdict = "PAS DE LUMIERE";
            } else {
                verdict = "VISIBLE";
                ++visibles;
            }
        }

        cout << name << ' ' << verdict << '\n';
    }

    cout << "VISIBLES " << visibles << '\n';
    cout << "EN PANNE " << (N - visibles) << '\n';
    return 0;
}
