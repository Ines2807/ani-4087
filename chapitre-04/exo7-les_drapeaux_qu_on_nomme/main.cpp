#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool obtenirValeurDrapeau(const string& nom, uint32_t& valeur) {
    if (nom == "RENDER2D") valeur = 1u;
    else if (nom == "RENDER3D") valeur = 2u;
    else if (nom == "TEXT") valeur = 4u;
    else if (nom == "UI") valeur = 8u;
    else if (nom == "SHADOW") valeur = 16u;
    else if (nom == "POST_PROCESS") valeur = 32u;
    else if (nom == "VFX") valeur = 64u;
    else if (nom == "ANIMATION") valeur = 128u;
    else if (nom == "OVERLAY") valeur = 256u;
    else if (nom == "SIMULATION") valeur = 512u;
    else if (nom == "OFFSCREEN") valeur = 1024u;
    else if (nom == "RAYTRACING") valeur = 2048u;
    else if (nom == "GPU_CULLING") valeur = 4096u;
    else if (nom == "NONE") valeur = 0u;
    else if (nom == "2D_ESSENTIALS") valeur = 1u | 4u;
    else if (nom == "3D_BASE") valeur = 2u | 16u | 32u;
    else if (nom == "DEBUG") valeur = 256u | 512u;
    else if (nom == "ALL") valeur = 0xFFFFFFFFu;
    else return false;
    return true;
}

bool estAllume(uint32_t valeur, uint32_t drapeau) {
    return (valeur & drapeau) != 0;
}

int main() {
    int N;
    cin >> N;

    uint32_t valeur = 0u;
    vector<string> inconnus;

    for (int i = 0; i < N; ++i) {
        string nom;
        cin >> nom;

        uint32_t valeurDrapeau = 0u;
        if (obtenirValeurDrapeau(nom, valeurDrapeau))
            valeur |= valeurDrapeau;
        else
            inconnus.push_back(nom);
    }

    // Sans aucun nom, on conserve la configuration par defaut : tout est allume.
    if (N == 0)
        valeur = 0xFFFFFFFFu;

    for (const string& nom : inconnus)
        cout << "INCONNU " << nom << '\n';

    cout << "VALEUR " << valeur << '\n';
    cout << "HEXA 0x" << uppercase << hex << setw(8) << setfill('0') << valeur
         << dec << nouppercase << setfill(' ') << '\n';

    // Les dependances sont affichees dans l'ordre indique par l'enonce.
    if (estAllume(valeur, 4u) && !estAllume(valeur, 1u))
        cout << "MANQUE TEXT RENDER2D\n";
    if (estAllume(valeur, 8u)) {
        if (!estAllume(valeur, 1u))
            cout << "MANQUE UI RENDER2D\n";
        if (!estAllume(valeur, 4u))
            cout << "MANQUE UI TEXT\n";
    }
    if (estAllume(valeur, 16u) && !estAllume(valeur, 2u))
        cout << "MANQUE SHADOW RENDER3D\n";
    if (estAllume(valeur, 256u)) {
        if (!estAllume(valeur, 1u))
            cout << "MANQUE OVERLAY RENDER2D\n";
        if (!estAllume(valeur, 4u))
            cout << "MANQUE OVERLAY TEXT\n";
    }

    const array<uint32_t, 13> drapeauxSimples = {
        1u, 2u, 4u, 8u, 16u, 32u, 64u,
        128u, 256u, 512u, 1024u, 2048u, 4096u
    };
    int allumes = 0;
    for (uint32_t drapeau : drapeauxSimples) {
        if (estAllume(valeur, drapeau))
            ++allumes;
    }

    cout << "ALLUMES " << allumes << '\n';
    cout << "ETEINTS " << (13 - allumes) << '\n';
    return 0;
}
