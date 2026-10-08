#include <algorithm>
#include <array>
#include <iostream>
#include <string>

using namespace std;

constexpr int kNombreTemps = 10;

struct Configuration {
    string nom;
    unsigned long long valeur = 0;
    array<long long, kNombreTemps> temps{};
    long long mediane = 0;
    long long moyenne = 0;
};

unsigned long long valeurDrapeau(const string& nom) {
    if (nom == "RENDER2D") return 1ULL;
    if (nom == "RENDER3D") return 2ULL;
    if (nom == "TEXT") return 4ULL;
    if (nom == "UI") return 8ULL;
    if (nom == "SHADOW") return 16ULL;
    if (nom == "POST_PROCESS") return 32ULL;
    if (nom == "ALL") return 4294967295ULL;
    return 0ULL;
}

Configuration lireConfiguration() {
    Configuration configuration;
    int nombreDrapeaux;
    cin >> configuration.nom >> nombreDrapeaux;

    for (int i = 0; i < nombreDrapeaux; ++i) {
        string nomDrapeau;
        cin >> nomDrapeau;
        configuration.valeur |= valeurDrapeau(nomDrapeau);
    }

    long long somme = 0;
    for (long long& temps : configuration.temps) {
        cin >> temps;
        somme += temps;
    }

    array<long long, kNombreTemps> tempsTries = configuration.temps;
    sort(tempsTries.begin(), tempsTries.end());
    configuration.mediane = (tempsTries[4] + tempsTries[5]) / 2;
    configuration.moyenne = somme / kNombreTemps;
    return configuration;
}

int main() {
    const Configuration premiere = lireConfiguration();
    const Configuration seconde = lireConfiguration();

    cout << premiere.nom << " VALEUR " << premiere.valeur << '\n';
    cout << premiere.nom << " MEDIANE " << premiere.mediane << '\n';
    cout << premiere.nom << " MOYENNE " << premiere.moyenne << '\n';
    cout << seconde.nom << " VALEUR " << seconde.valeur << '\n';
    cout << seconde.nom << " MEDIANE " << seconde.mediane << '\n';
    cout << seconde.nom << " MOYENNE " << seconde.moyenne << '\n';
    cout << "ECART MEDIANES " << premiere.mediane - seconde.mediane << '\n';
    cout << "ECART MOYENNES " << premiere.moyenne - seconde.moyenne << '\n';
    return 0;
}
