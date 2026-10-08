#include <iostream>
#include <string>

using namespace std;

int main() {
    int N;
    cin >> N;

    int visibles = 0;

    for (int i = 0; i < N; ++i) {
        string name;
        long long drapeaux;
        int sx, sy, sz;
        int distance;
        int lumieres;
        int ambiante;
        int proche;

        cin >> name >> drapeaux >> sx >> sy >> sz >> distance >> lumieres >> ambiante >> proche;

        string verdict;

        if ((drapeaux & 2LL) == 0) {
            verdict = "RENDER3D ETEINT";
        } else if (sx == 0 || sy == 0 || sz == 0) {
            verdict = "ECHELLE NULLE";
        } else {
            int faceAvant = distance - sz / 2;
            if (faceAvant <= 0) {
                verdict = "CAMERA DANS LE CUBE";
            } else if (faceAvant < proche) {
                verdict = "COUPE PAR LE PLAN PROCHE";
            } else if (lumieres == 0 && ambiante == 0) {
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
