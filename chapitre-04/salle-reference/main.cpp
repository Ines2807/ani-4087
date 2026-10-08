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
#include <chrono>
#include <cstdlib>
#include <cstring>

using namespace nkentseu;
using namespace nkentseu::renderer;

static void ConfigureAppData(NkAppData& data) {
    data.appName = "SalleReference";
}
NK_REGISTER_ENTRY_APPDATA_UPDATER(ConfigureAppData)

static float32 EnvironmentFloat(const char* name, float32 fallback) {
    const char* value = std::getenv(name);
    return value ? std::strtof(value, nullptr) : fallback;
}

static bool EnvironmentEnabled(const char* name, bool fallback) {
    const char* value = std::getenv(name);
    if (!value)
        return fallback;
    return std::strcmp(value, "0") != 0;
}

static NkVec3f PoserAuSol(float32 x, float32 z, float32 hauteur) {
    return {x, hauteur * 0.5f, z};
}

static NkVec3f PoserSurTable(float32 x, float32 z, float32 hauteur, float32 dessus) {
    return {x, dessus + hauteur * 0.5f, z};
}

static void SubmitCube(NkRender3D* render3D, NkMeshHandle cube, NkVec3f center,
                       NkVec3f size, NkVec3f tint, bool castShadow = true) {
    NkDrawCall3D draw;
    draw.mesh = cube;
    draw.transform = NkMat4f::Translate(center) * NkMat4f::Scale(size);
    draw.aabb = {
        {center.x - size.x * 0.5f, center.y - size.y * 0.5f, center.z - size.z * 0.5f},
        {center.x + size.x * 0.5f, center.y + size.y * 0.5f, center.z + size.z * 0.5f}
    };
    draw.tint = tint;
    draw.roughness = 0.8f;
    draw.castShadow = castShadow;
    render3D->Submit(draw);
}

int nkmain(const NkEntryState& state) {
    (void)state;
    const auto startupStart = std::chrono::steady_clock::now();

    NkWindowConfig windowConfig;
    windowConfig.title = "Salle de reference - chapitre 4";
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
    logger.Info("Backend graphique : {0}", NkGraphicsApiName(device->GetApi()));

    NkRendererConfig rendererConfig = NkRendererConfig::ForMinimal(
        deviceInfo.api, deviceInfo.width, deviceInfo.height);
    const char* profile = std::getenv("SALLE_SYSTEMES");
    if (profile && std::strcmp(profile, "ALL") == 0)
        rendererConfig.subsystems = NK_SS_ALL;
    else
        rendererConfig.Enable(NK_SS_RENDER3D | NK_SS_SHADOW);

    NkRenderer* renderer = NkRenderer::Create(device, rendererConfig);
    if (!renderer || !renderer->Initialize()) {
        logger.Error("Initialisation de NKRenderer impossible");
        NkRenderer::Destroy(renderer);
        NkDeviceFactory::Destroy(device);
        window.Close();
        return 3;
    }
    const auto startupEnd = std::chrono::steady_clock::now();
    const double startupMs = std::chrono::duration<double, std::milli>(startupEnd - startupStart).count();
    logger.Info("Initialisation fenetre + renderer : {0} ms", startupMs);

    NkRender3D* render3D = renderer->GetRender3D();
    NkMeshSystem* meshes = renderer->GetMeshSystem();
    if (!render3D || !meshes) {
        logger.Error("Rendu 3D ou maillages indisponibles");
        device->WaitIdle();
        NkRenderer::Destroy(renderer);
        NkDeviceFactory::Destroy(device);
        window.Close();
        return 4;
    }
    if (auto* shadow = renderer->GetShadow())
        shadow->GetConfig().autoFitDirectional = true;

    const NkMeshHandle cube = meshes->GetCube();
    const NkMeshHandle plane = meshes->GetPlane();
    const NkVec3f sunDirection = {
        EnvironmentFloat("SALLE_SUN_X", -0.4f),
        EnvironmentFloat("SALLE_SUN_Y", -1.f),
        EnvironmentFloat("SALLE_SUN_Z", -0.3f)
    };
    const float32 sunIntensity = EnvironmentFloat("SALLE_INTENSITE", 3.f);
    const bool castShadow = EnvironmentEnabled("SALLE_CAST_SHADOW", true);

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

        NkCamera3DData cameraData;
        cameraData.up = {0.f, 1.f, 0.f};
        cameraData.fovY = 60.f;
        cameraData.aspect = (float32)width / (float32)height;
        cameraData.nearPlane = 0.05f;
        cameraData.farPlane = 100.f;
        NkCamera3D camera(cameraData);
        camera.SetPosition({0.f, 1.7f, 1.7f});
        camera.SetTarget({0.f, 1.0f, -0.3f});

        NkSceneContext scene;
        scene.camera = camera;
        scene.time = totalTime;
        scene.deltaTime = deltaTime;
        scene.ambientIntensity = 0.15f;

        NkLightDesc sun;
        sun.type = NkLightType::NK_DIRECTIONAL;
        sun.direction = sunDirection;
        sun.color = {1.f, 0.95f, 0.85f};
        sun.intensity = sunIntensity;
        sun.castShadow = castShadow;
        sun.shadowStatic = false;
        scene.lights.PushBack(sun);
        render3D->BeginScene(scene);

        NkDrawCall3D floor;
        floor.mesh = plane;
        floor.transform = NkMat4f::Scale({4.f, 1.f, 4.f});
        floor.aabb = {{-2.f, 0.f, -2.f}, {2.f, 0.f, 2.f}};
        floor.tint = {0.58f, 0.60f, 0.62f};
        floor.roughness = 0.9f;
        floor.castShadow = false;
        render3D->Submit(floor);

        SubmitCube(render3D, cube, {0.f, 1.25f, -1.95f}, {4.f, 2.5f, 0.1f}, {0.72f, 0.70f, 0.66f});
        SubmitCube(render3D, cube, {-1.95f, 1.25f, 0.f}, {0.1f, 2.5f, 4.f}, {0.68f, 0.72f, 0.70f});
        SubmitCube(render3D, cube, {1.95f, 1.25f, 0.f}, {0.1f, 2.5f, 4.f}, {0.70f, 0.68f, 0.72f});

        // Le plateau repose sur les pieds; les objets se posent sur son dessus a 0,75 m.
        const float32 tableX = 0.35f;
        const float32 tableZ = -0.3f;
        const float32 tableTop = 0.75f;
        const float32 tableThickness = 0.06f;
        const float32 legHeight = tableTop - tableThickness;
        SubmitCube(render3D, cube,
                   PoserSurTable(tableX, tableZ, tableThickness, legHeight),
                   {1.2f, tableThickness, 0.8f}, {0.48f, 0.30f, 0.16f});
        for (float32 xSign : {-1.f, 1.f}) {
            for (float32 zSign : {-1.f, 1.f}) {
                SubmitCube(render3D, cube,
                           PoserAuSol(tableX + xSign * 0.54f, tableZ + zSign * 0.34f, legHeight),
                           {0.06f, legHeight, 0.06f}, {0.38f, 0.23f, 0.12f});
            }
        }

        // Un cube pose au sol sert de repere de taille et de lecture des ombres.
        SubmitCube(render3D, cube, PoserAuSol(-0.85f, -0.8f, 0.7f),
                   {0.7f, 0.7f, 0.7f}, {0.25f, 0.55f, 0.85f});
        // Un petit objet pose sur le plateau rend sa hauteur plus facile a juger.
        SubmitCube(render3D, cube, PoserSurTable(tableX, tableZ, 0.4f, tableTop),
                   {0.2f, 0.4f, 0.2f}, {0.86f, 0.72f, 0.28f});

        renderer->Present();
        renderer->EndFrame();
    }

    device->WaitIdle();
    NkRenderer::Destroy(renderer);
    NkDeviceFactory::Destroy(device);
    window.Close();
    return 0;
}
