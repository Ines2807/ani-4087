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
#include "NKRenderer/Tools/Shadow/NkVirtualShadowMaps.h"

using namespace nkentseu;
using namespace nkentseu::renderer;

static void ConfigureAppData(NkAppData& data) {
    data.appName = "Exo3Salle";
}
NK_REGISTER_ENTRY_APPDATA_UPDATER(ConfigureAppData)

int nkmain(const NkEntryState& state) {
    (void)state;

    NkWindowConfig windowConfig;
    windowConfig.title = "Exercice 3 - Une salle aux vraies dimensions";
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

    // La salle n'a besoin que des maillages 3D et des ombres, pas de tous les modules.
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
    // On stabilise le cadrage des ombres pour eviter qu'elles ne scintillent.
    if (auto* shadow = renderer->GetShadow())
        shadow->GetConfig().autoFitDirectional = true;

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

        // On regarde la piece debout, a 1,70 m; le plan proche est a 5 cm.
        NkCamera3DData cameraData;
        cameraData.up = {0.f, 1.f, 0.f};
        cameraData.fovY = 60.f;
        cameraData.aspect = (float32)width / (float32)height;
        cameraData.nearPlane = 0.05f;
        cameraData.farPlane = 100.f;
        NkCamera3D camera(cameraData);
        camera.SetPosition({0.f, 1.7f, 1.5f});
        camera.SetTarget({0.f, 1.25f, -1.9f});

        NkSceneContext scene;
        scene.camera = camera;
        scene.time = totalTime;
        scene.deltaTime = deltaTime;
        scene.ambientIntensity = 0.15f;

        // Le soleil eclaire la salle et projette les ombres du cube.
        NkLightDesc sun;
        sun.type = NkLightType::NK_DIRECTIONAL;
        sun.direction = {-0.4f, -1.f, -0.3f};
        sun.color = {1.f, 0.95f, 0.85f};
        sun.intensity = 3.f;
        sun.castShadow = true;
        sun.shadowStatic = false;
        scene.lights.PushBack(sun);

        render3D->BeginScene(scene);

        // Le sol fait 4 m sur 4 m. Comme c'est un plan, il reste pile a y = 0.
        NkDrawCall3D floor;
        floor.mesh = plane;
        floor.transform = NkMat4f::Scale({4.f, 1.f, 4.f});
        floor.aabb = {{-2.f, 0.f, -2.f}, {2.f, 0.f, 2.f}};
        floor.tint = {0.62f, 0.60f, 0.55f};
        floor.roughness = 0.9f;
        floor.castShadow = false;
        render3D->Submit(floor);

        // On agrandit d'abord le cube unitaire, puis on place le mur a sa hauteur.
        NkDrawCall3D backWall;
        backWall.mesh = cube;
        backWall.transform = NkMat4f::Translate({0.f, 1.25f, -1.95f}) *
                             NkMat4f::Scale({4.f, 2.5f, 0.1f});
        backWall.aabb = {{-2.f, 0.f, -2.f}, {2.f, 2.5f, -1.9f}};
        backWall.tint = {0.72f, 0.70f, 0.66f};
        backWall.roughness = 0.85f;
        render3D->Submit(backWall);

        // Les deux murs lateraux ferment la piece sans ajouter d'epaisseur au sol.
        NkDrawCall3D leftWall;
        leftWall.mesh = cube;
        leftWall.transform = NkMat4f::Translate({-1.95f, 1.25f, 0.f}) *
                             NkMat4f::Scale({0.1f, 2.5f, 4.f});
        leftWall.aabb = {{-2.f, 0.f, -2.f}, {-1.9f, 2.5f, 2.f}};
        leftWall.tint = {0.68f, 0.72f, 0.70f};
        leftWall.roughness = 0.85f;
        render3D->Submit(leftWall);

        NkDrawCall3D rightWall;
        rightWall.mesh = cube;
        rightWall.transform = NkMat4f::Translate({1.95f, 1.25f, 0.f}) *
                              NkMat4f::Scale({0.1f, 2.5f, 4.f});
        rightWall.aabb = {{1.9f, 0.f, -2.f}, {2.f, 2.5f, 2.f}};
        rightWall.tint = {0.70f, 0.68f, 0.72f};
        rightWall.roughness = 0.85f;
        render3D->Submit(rightWall);

        // Ce cube de 70 cm sert de repere; son centre est a 35 cm pour qu'il repose au sol.
        NkDrawCall3D referenceCube;
        referenceCube.mesh = cube;
        referenceCube.transform = NkMat4f::Translate({0.f, 0.35f, -0.5f}) *
                                  NkMat4f::Scale({0.7f, 0.7f, 0.7f});
        referenceCube.aabb = {{-0.35f, 0.f, -0.85f}, {0.35f, 0.7f, -0.15f}};
        referenceCube.tint = {0.45f, 0.52f, 0.58f};
        referenceCube.roughness = 0.5f;
        render3D->Submit(referenceCube);

        renderer->Present();
        renderer->EndFrame();
    }

    device->WaitIdle();
    NkRenderer::Destroy(renderer);
    NkDeviceFactory::Destroy(device);
    window.Close();
    return 0;
}
