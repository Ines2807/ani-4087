#include <iostream>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

// Convertit le code technique d'une API en libellé lisible pour l'affichage.
string readableName(const string& api) {
    if (api == "VULKAN") return "Vulkan";
    if (api == "DX12") return "DirectX 12";
    if (api == "DX11") return "DirectX 11";
    if (api == "OPENGL") return "OpenGL";
    if (api == "METAL") return "Metal";
    if (api == "SOFTWARE") return "Software";
    return api;
}

// Donne l'ordre de priorité des APIs selon la plateforme cible.
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
    return {"VULKAN", "OPENGL", "SOFTWARE"};
}

int main() {
    // Accélère la lecture/écriture standard pour les entrées sorties volumineuses.
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    // Stocke les données globales du traitement.
    vector<string> selectedNames;
    set<string> distinctNames;
    int ignoredTotal = 0;
    int softwareCount = 0;

    // Traite chaque machine de la liste.
    for (int i = 0; i < N; ++i) {
        string machineName, platform;
        int k;
        cin >> machineName >> platform >> k;

        // Lit les APIs de cette machine et garde celles qu'on a vues.
        vector<string> listed(k);
        unordered_set<string> present;
        for (int j = 0; j < k; ++j) {
            cin >> listed[j];
            present.insert(listed[j]);
        }

        // On récupère l'ordre de priorité de la plateforme.
        vector<string> platformOrder = orderForPlatform(platform);
        unordered_set<string> platformSet(platformOrder.begin(), platformOrder.end());

        // Compte les APIs qui ne sont pas prises en charge par la plateforme.
        int ignoredCount = 0;
        for (const string& api : listed) {
            if (platformSet.find(api) == platformSet.end()) {
                ++ignoredCount;
            }
        }
        ignoredTotal += ignoredCount;

        // Choisit la première API supportée qui est présente.
        // Sinon, on considère que le logiciel est utilisé.
        string chosen = "SOFTWARE";
        for (const string& api : platformOrder) {
            if (present.find(api) != present.end()) {
                chosen = api;
                break;
            }
        }

        if (chosen == "SOFTWARE") {
            ++softwareCount;
        }

        // Affiche le nom de la machine et l'API sélectionnée.
        cout << machineName << ' ' << readableName(chosen) << '\n';
        distinctNames.insert(readableName(chosen));
    }

    // Résumé final: APIs ignorées, machines logicielles et valeurs distinctes.
    cout << "IGNOREES " << ignoredTotal << '\n';
    cout << "LOGICIEL " << softwareCount << '\n';
    cout << "DIFFERENTES " << distinctNames.size() << '\n';

    return 0;
}
