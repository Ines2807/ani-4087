#include <iostream>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

string readableName(const string& api) {
    if (api == "VULKAN") return "Vulkan";
    if (api == "DX12") return "DirectX 12";
    if (api == "DX11") return "DirectX 11";
    if (api == "OPENGL") return "OpenGL";
    if (api == "METAL") return "Metal";
    if (api == "SOFTWARE") return "Software";
    return api;
}

// On simule ici l'auto-detection : chaque plateforme a ses backends preferes.
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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    set<string> distinctNames;
    int ignoredTotal = 0;
    int softwareCount = 0;

    for (int i = 0; i < N; ++i) {
        string machineName, platform;
        int k;
        cin >> machineName >> platform >> k;

        vector<string> listed(k);
        unordered_set<string> present;
        for (int j = 0; j < k; ++j) {
            cin >> listed[j];
            present.insert(listed[j]);
        }

        const vector<string> platformOrder = orderForPlatform(platform);
        const unordered_set<string> supported(platformOrder.begin(), platformOrder.end());

        // Les API hors de la liste de la plateforme ne peuvent pas etre retenues.
        for (const string& api : listed) {
            if (supported.find(api) == supported.end())
                ++ignoredTotal;
        }

        // On prend le premier backend prefere qui figure parmi ceux disponibles.
        string chosen = "SOFTWARE";
        for (const string& api : platformOrder) {
            if (present.find(api) != present.end()) {
                chosen = api;
                break;
            }
        }

        if (chosen == "SOFTWARE")
            ++softwareCount;

        const string displayName = readableName(chosen);
        cout << machineName << ' ' << displayName << '\n';
        distinctNames.insert(displayName);
    }

    cout << "IGNOREES " << ignoredTotal << '\n';
    cout << "LOGICIEL " << softwareCount << '\n';
    cout << "DIFFERENTES " << distinctNames.size() << '\n';
    return 0;
}
