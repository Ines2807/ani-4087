#include "NKWindow/NKMain.h"
#include "NKWindow/Core/NkWindow.h"
#include "NKWindow/Core/NkWindowConfig.h"
#include "NKEvent/NkWindowEvent.h"
#include "NKEvent/NkKeyboardEvent.h"
#include "NKTime/NkTime.h"
#include "NKLogger/NkLog.h"
#include "NKRHI/Core/NkDeviceFactory.h"
#include "NKRenderer/NkRenderer.h"
#include "NKRenderer/Core/NkCamera.h"
#include "NKRenderer/Mesh/NkMeshSystem.h"
#include "NKRenderer/Tools/Render3D/NkRender3D.h"

using namespace nkentseu;
using namespace nkentseu::renderer;

static void ConfigureAppData(NkAppData& data) {
    data.appName = "Exo2Cube";
}
NK_REGISTER_ENTRY_APPDATA_UPDATER(ConfigureAppData)

int nkmain(const NkEntryState& state) {
    (void)state;

    NkWindowConfig windowConfig;
    windowConfig.title = "Exercice 2 - Un cube sur le sol";
    windowConfig.width = 1280;
    windowConfig.height = 720;
    windowConfig.centered = true;
    windowConfig.resizable = true;

    NkWindow window(windowConfig);
    if (!window.IsValid()) {
        logger.Error("Creation de la fenetre impossible");
        return 1;
    }

    NkDeviceInitInfo deviceInfo{};
    deviceInfo.surface = window.GetSurfaceDesc();
    deviceInfo.width = (uint32)window.GetSize().width;
    deviceInfo.height = (uint32)window.GetSize().height;

    NkIDevice* device = NkDeviceFactory::CreateAutoDetect(deviceInfo);
    if (!device || !device->IsValid()) {
        logger.Error("Creation du peripherique graphique impossible");
        NkDeviceFactory::Destroy(device);
        window.Close();
        return 2;
    }
    logger.Info("Backend graphique choisi : {0}", NkGraphicsApiName(device->GetApi()));

    // On part de presque rien et on n'allume que le rendu 3D et les ombres.
    NkRendererConfig rendererConfig = NkRendererConfig::ForMinimal(
        deviceInfo.api, deviceInfo.width, deviceInfo.height);
    rendererConfig.Enable(NK_SS_RENDER3D | NK_SS_SHADOW);

    NkRenderer* renderer = NkRenderer::Create(device, rendererConfig);
    if (!renderer || !renderer->Initialize()) {
        logger.Error("Initialisation de NKRenderer impossible");
        NkRenderer::Destroy(renderer);
        NkDeviceFactory::Destroy(device);
        window.Close();
        return 3;
    }

    NkRender3D* render3D = renderer->GetRender3D();
    NkMeshSystem* meshes = renderer->GetMeshSystem();
    if (!render3D || !meshes) {
        logger.Error("Le rendu 3D ou le systeme de maillages est indisponible");
        device->WaitIdle();
        NkRenderer::Destroy(renderer);
        NkDeviceFactory::Destroy(device);
        window.Close();
        return 4;
    }

    const NkMeshHandle cube = meshes->GetCube();
    const NkMeshHandle plane = meshes->GetPlane();

    bool running = true;
    uint32 width = deviceInfo.width;
    uint32 height = deviceInfo.height;
    NkEventSystem& events = NkEvents();
    events.AddEventCallback<NkWindowCloseEvent>([&](NkWindowCloseEvent*) {
        running = false;
    });
    events.AddEventCallback<NkKeyPressEvent>([&](NkKeyPressEvent* event) {
        if (event->GetKey() == NkKey::NK_ESCAPE)
            running = false;
    });
    events.AddEventCallback<NkWindowResizeEvent>([&](NkWindowResizeEvent* event) {
        const uint32 newWidth = (uint32)event->GetWidth();
        const uint32 newHeight = (uint32)event->GetHeight();
        // Une fenetre reduite peut avoir une taille nulle; on ignore ce cas.
        if (newWidth > 0 && newHeight > 0 && (newWidth != width || newHeight != height)) {
            width = newWidth;
            height = newHeight;
            renderer->OnResize(width, height);
        }
    });

    NkClock clock;
    float32 totalTime = 0.f;
    while (running && window.IsOpen()) {
        events.PollEvents();
        if (!running)
            break;

        const float32 deltaTime = clock.Tick().delta;
        totalTime += deltaTime;
        if (!renderer->BeginFrame())
            continue;

        // La camera est deja a hauteur d'yeux, et le plan proche reste a 5 cm.
        NkCamera3DData cameraData;
        cameraData.up = {0.f, 1.f, 0.f};
        cameraData.fovY = 60.f;
        cameraData.aspect = (float32)width / (float32)height;
        cameraData.nearPlane = 0.05f;
        cameraData.farPlane = 50.f;
        NkCamera3D camera(cameraData);
        camera.SetPosition({0.f, 1.7f, 4.f});
        camera.SetTarget({0.f, 0.35f, 0.f});

        NkSceneContext scene;
        scene.camera = camera;
        scene.time = totalTime;
        scene.deltaTime = deltaTime;
        scene.ambientIntensity = 0.15f;

        // Une seule lumiere directionnelle suffit pour voir le volume du cube.
        NkLightDesc sun;
        sun.type = NkLightType::NK_DIRECTIONAL;
        sun.direction = {-0.4f, -1.f, -0.3f};
        sun.color = {1.f, 0.95f, 0.85f};
        sun.intensity = 3.f;
        sun.castShadow = true;
        scene.lights.PushBack(sun);

        render3D->BeginScene(scene);

        // Le sol est un plan, donc on peut le laisser exactement a y = 0.
        NkDrawCall3D floor;
        floor.mesh = plane;
        floor.transform = NkMat4f::Scale({8.f, 1.f, 8.f});
        floor.aabb = {{-4.f, 0.f, -4.f}, {4.f, 0.f, 4.f}};
        floor.tint = {0.55f, 0.58f, 0.62f};
        floor.roughness = 0.9f;
        floor.castShadow = false;
        render3D->Submit(floor);

        NkDrawCall3D drawCube;
        drawCube.mesh = cube;
        // Le cube part de l'origine; on le monte de 35 cm pour qu'il touche le sol.
        // Dans ce produit, l'echelle s'applique d'abord, puis vient la translation.
        drawCube.transform = NkMat4f::Translate({0.f, 0.35f, 0.f}) *
                             NkMat4f::Scale({0.7f, 0.7f, 0.7f});
        drawCube.aabb = {{-0.35f, 0.f, -0.35f}, {0.35f, 0.7f, 0.35f}};
        drawCube.tint = {0.25f, 0.55f, 0.85f};
        drawCube.metallic = 0.f;
        drawCube.roughness = 0.45f;
        render3D->Submit(drawCube);

        // Submit a empile les objets; Present affiche ensuite l'image complete.
        renderer->Present();
        renderer->EndFrame();
    }

    device->WaitIdle();
    NkRenderer::Destroy(renderer);
    NkDeviceFactory::Destroy(device);
    window.Close();
    return 0;
}
