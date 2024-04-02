#include "Engine.h"
#include "ui/ImguiUI.h"
#include "ZAPDUtils/Utils/StringHelper.h"
#include "libultraship/src/Context.h"

#include <Fast3D/gfx_pc.h>
#include <Fast3D/gfx_rendering_api.h>
#include <SDL2/SDL_net.h>

#include <utility>

extern "C" {
float gInterpolationStep = 0.0f;
}

GameEngine* GameEngine::Instance;

GameEngine::GameEngine() {
    std::vector<std::string> OTRFiles;
    // if (const std::string cube_path = LUS::Context::GetPathRelativeToAppDirectory("lylat.otr"); std::filesystem::exists(cube_path)) {
    //     OTRFiles.push_back(cube_path);
    // }
    if (const std::string sm64_otr_path = LUS::Context::GetPathRelativeToAppBundle("sm64.otr"); std::filesystem::exists(sm64_otr_path)) {
        OTRFiles.push_back(sm64_otr_path);
    }
    if (const std::string patches_path = LUS::Context::GetPathRelativeToAppDirectory("mods"); !patches_path.empty() && std::filesystem::exists(patches_path)) {
        if (std::filesystem::is_directory(patches_path)) {
            for (const auto&p: std::filesystem::recursive_directory_iterator(patches_path)) {
                if (StringHelper::IEquals(p.path().extension().string(), ".otr")) {
                    OTRFiles.push_back(p.path().generic_string());
                }
            }
        }
    }
    this->context = LUS::Context::CreateInstance("Lylat64", "sf64", "lylat.cfg.json", OTRFiles,
                                                 {}, 3);
    // this->context->GetResourceManager()->GetResourceLoader()->RegisterResourceFactory(
    //     LUS::ResourceType::SAnim, "Animation", std::make_shared<CubeOS::AnimationFactory>());
}

void GameEngine::Create(){
    const auto instance = Instance = new GameEngine();
    GameUI::SetupGuiElements();
}

void GameEngine::Destroy(){

}

bool ShouldClearTextureCacheAtEndOfFrame = false;

void GameEngine::StartFrame() const{
    using LUS::KbScancode;
    const int32_t dwScancode = this->context->GetWindow()->GetLastScancode();
    this->context->GetWindow()->SetLastScancode(-1);

    switch (dwScancode) {
        case KbScancode::LUS_KB_TAB: {
            // Toggle HD Assets
            CVarSetInteger("gAltAssets", !CVarGetInteger("gAltAssets", 0));
            ShouldClearTextureCacheAtEndOfFrame = true;
            break;
        }
        default: break;
    }
    this->context->GetWindow()->StartFrame();
}

void GameEngine::ProcessFrame(void (*run_one_game_iter)()) const {
    this->context->GetWindow()->MainLoop(run_one_game_iter);
}

void GameEngine::RunCommands(Gfx* Commands) {
    gfx_run(Commands, {});
    gfx_end_frame();

    if (ShouldClearTextureCacheAtEndOfFrame) {
        gfx_texture_cache_clear();
        ShouldClearTextureCacheAtEndOfFrame = false;
    }
}

void GameEngine::ProcessGfxCommands(Gfx* commands) {
    RunCommands(commands);
    Instance->context->GetWindow()->SetTargetFps(30);
    Instance->context->GetWindow()->SetMaximumFrameLatency(1);
}

extern "C" uint32_t GameEngine_GetSampleRate() {
    auto player = LUS::Context::GetInstance()->GetAudio()->GetAudioPlayer();
    if (player == nullptr) {
        return 0;
    }

    if (!player->IsInitialized()) {
        return 0;
    }

    return player->GetSampleRate();
}

extern "C" uint32_t GameEngine_GetSamplesPerFrame(){
    return SAMPLES_PER_FRAME;
}

// End

extern "C" float GameEngine_GetAspectRatio() {
    return gfx_current_dimensions.aspect_ratio;
}

extern "C" uint32_t GameEngine_GetGameVersion() {
    return 0x00000001;
}

extern "C" int GameEngine_OTRSigCheck(const char* data) {
    static const char* sOtrSignature = "__OTR__";
    return strncmp(data, sOtrSignature, strlen(sOtrSignature)) == 0;
}