#include <iostream>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

// Cette fonction transforme les identifiants techniques (VULKAN, DX12, etc.)
// en un nom plus lisible pour l'utilisateur final. Par exemple :
// "DX12" devient "DirectX 12".
string readableName(const string& api) {
    if (api == "VULKAN") return "Vulkan";
    if (api == "DX12") return "DirectX 12";
    if (api == "DX11") return "DirectX 11";
    if (api == "OPENGL") return "OpenGL";
    if (api == "METAL") return "Metal";
    if (api == "SOFTWARE") return "Software";
    return api; // Si l'API n'est pas reconnue, on la renvoie telle quelle.
}

// Chaque plateforme a une liste d'API prioritaires.
// Exemple : sur Windows, Vulkan a plus de priorité que OpenGL, car il est
// généralement préféré lorsqu'il est disponible.
vector<string> orderForPlatform(const string& platform) {
    if (platform == "WINDOWS") {
        return {"VULKAN", "DX12", "DX11", "OPENGL", "SOFTWARE"};
    }
    if (platform == "MACOS") {
        return {"METAL", "OPENGL", "SOFTWARE"};
    }
    if (platform == "IOS") {
        return {"METAL", "SOFTWARE"};
    }
    if (platform == "ANDROID") {
        return {"VULKAN", "OPENGL", "SOFTWARE"};
    }
    // Plateforme inconnue : on prend un ordre simple par défaut.
    return {"VULKAN", "OPENGL", "SOFTWARE"};
}

int main() {
    // On désactive la synchronisation avec stdio pour accélérer les entrées/sorties.
    // Cela est utile quand le programme lit beaucoup de données.
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    // N = nombre total de machines à traiter.
    cin >> N;

    // Variables globales pour le résumé final.
    vector<string> selectedNames; // Contient les noms des API sélectionnées (non utilisé dans le calcul final, mais utile conceptuellement).
    set<string> distinctNames;    // Permet de compter les noms différents de APIs choisies.
    int ignoredTotal = 0;         // Somme des APIs ignorées pour toutes les machines.
    int softwareCount = 0;        // Nombre de machines qui utilisent uniquement le logiciel.

    // Traitement de chaque machine.
    for (int i = 0; i < N; ++i) {
        string machineName, platform;
        int k;
        // Lecture : nom de la machine, plateforme et nombre d'API listées.
        cin >> machineName >> platform >> k;

        // On lit les APIs de cette machine.
        vector<string> listed(k);
        unordered_set<string> present; // Contient uniquement les API présentes pour cette machine.
        for (int j = 0; j < k; ++j) {
            cin >> listed[j];
            present.insert(listed[j]);
        }

        // On récupère l'ordre de priorité des APIs selon la plateforme.
        vector<string> platformOrder = orderForPlatform(platform);
        unordered_set<string> platformSet(platformOrder.begin(), platformOrder.end());

        // On compte les APIs de la machine qui ne sont pas supportées par cette plateforme.
        int ignoredCount = 0;
        for (const string& api : listed) {
            if (platformSet.find(api) == platformSet.end()) {
                ++ignoredCount;
            }
        }
        ignoredTotal += ignoredCount;

        // On choisit l'API la plus prioritaire supportée et présente sur la machine.
        // Si aucune API de la plateforme n'est présente, on considère qu'on utilise le logiciel.
        string chosen = "SOFTWARE";
        for (const string& api : platformOrder) {
            if (present.find(api) != present.end()) {
                chosen = api;
                break;
            }
        }

        // Si l'API retenue est SOFTWARE, on incrémente le compteur.
        if (chosen == "SOFTWARE") {
            ++softwareCount;
        }

        // Affichage du résultat pour cette machine.
        cout << machineName << ' ' << readableName(chosen) << '\n';

        // On stocke l'API choisie pour savoir combien de valeurs différentes existent au total.
        distinctNames.insert(readableName(chosen));
    }

    // Impression du bilan final.
    // IGNOREES : nombre total d'API non supportées.
    cout << "IGNOREES " << ignoredTotal << '\n';
    // LOGICIEL : nombre de machines qui n'ont aucune API supportée.
    cout << "LOGICIEL " << softwareCount << '\n';
    // DIFFERENTES : nombre de résultats uniques affichés.
    cout << "DIFFERENTES " << distinctNames.size() << '\n';

    return 0;
}
