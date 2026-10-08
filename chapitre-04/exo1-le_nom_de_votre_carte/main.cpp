#include "NKWindow/NKMain.h"
#include "NKWindow/Core/NkWindow.h"
#include "NKWindow/Core/NkWindowConfig.h"
#include "NKEvent/NkWindowEvent.h"
#include "NKEvent/NkKeyboardEvent.h"
#include "NKTime/NkTime.h"
#include "NKLogger/NkLog.h"
#include "NKRHI/Core/NkDeviceFactory.h"

using namespace nkentseu;

static void ConfigureAppData(NkAppData& data) {
    data.appName = "Exo1NomCarte";
}
NK_REGISTER_ENTRY_APPDATA_UPDATER(ConfigureAppData)

int nkmain(const NkEntryState& state) {
    (void)state;

    NkWindowConfig windowConfig;
    windowConfig.title = "Exercice 1 - Backend graphique";
    windowConfig.width = 960;
    windowConfig.height = 540;
    windowConfig.centered = true;
    windowConfig.resizable = true;

    NkWindow window(windowConfig);
    if (!window.IsValid()) {
        logger.Error("Creation de la fenetre impossible");
        return 1;
    }

    // La fenetre donne a NKRHI la surface ou il pourra dessiner.
    NkDeviceInitInfo deviceInfo{};
    deviceInfo.surface = window.GetSurfaceDesc();
    deviceInfo.width = (uint32)window.GetSize().width;
    deviceInfo.height = (uint32)window.GetSize().height;

    // Nkentseu choisit tout seul le backend qui convient a cette machine.
    NkIDevice* device = NkDeviceFactory::CreateAutoDetect(deviceInfo);
    if (!device || !device->IsValid()) {
        logger.Error("Creation du peripherique graphique impossible");
        NkDeviceFactory::Destroy(device);
        window.Close();
        return 2;
    }

    logger.Info("Backend graphique choisi : {0}", NkGraphicsApiName(device->GetApi()));

    bool running = true;
    NkEventSystem& events = NkEvents();
    events.AddEventCallback<NkWindowCloseEvent>([&](NkWindowCloseEvent*) {
        running = false;
    });
    events.AddEventCallback<NkKeyPressEvent>([&](NkKeyPressEvent* event) {
        if (event->GetKey() == NkKey::NK_ESCAPE)
            running = false;
    });

    // Ici, on teste seulement le peripherique : aucune image n'est encore dessinee.
    while (running && window.IsOpen()) {
        events.PollEvents();
        NkClock::Sleep(10);
    }

    // On libere d'abord le peripherique, puis on ferme la fenetre.
    device->WaitIdle();
    NkDeviceFactory::Destroy(device);
    window.Close();
    return 0;
}
