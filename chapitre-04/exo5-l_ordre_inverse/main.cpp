#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

int main() {
    int N;
    cin >> N;

    int deplaces = 0;
    long long pire = 0;

    for (int i = 0; i < N; ++i) {
        string nom;
        long long tx, ty, tz;
        long long sx, sy, sz;
        cin >> nom >> tx >> ty >> tz >> sx >> sy >> sz;

        // Le mauvais ordre applique aussi l'echelle a la translation.
        // On multiplie avant de diviser pour garder la precision entiere.
        const long long xObtenu = sx * tx / 1000;
        const long long yObtenu = sy * ty / 1000;
        const long long zObtenu = sz * tz / 1000;

        // La bonne translation place le centre directement en (tx, ty, tz).
        const long long ecartX = llabs(tx - xObtenu);
        const long long ecartY = llabs(ty - yObtenu);
        const long long ecartZ = llabs(tz - zObtenu);
        const long long ecart = max(ecartX, max(ecartY, ecartZ));

        if (ecart != 0)
            ++deplaces;
        pire = max(pire, ecart);

        cout << nom << ' ' << xObtenu << ' ' << yObtenu << ' ' << zObtenu << ' ' << ecart << '\n';
    }

    cout << "DEPLACES " << deplaces << '\n';
    cout << "PIRE " << pire << '\n';
    return 0;
}
