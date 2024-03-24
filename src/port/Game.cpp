#include <libultraship.h>

#include <Fast3D/gfx_pc.h>
#include "Engine.h"

extern "C"
// void exec_display_list(SPTask *spTask) {
//     GameEngine::ProcessGfxCommands((Gfx *) spTask->task.t.data_ptr);
// }

void push_frame() {
    // GameEngine::StartAudioFrame();
    GameEngine::Instance->StartFrame();
    // thread5_iteration();
    // GameEngine::EndAudioFrame();
}

#ifdef _WIN32
int SDL_main(int argc, char **argv) {
#else
int main(){
#endif
    GameEngine::Create();
    // alloc_pool();
    // audio_init();
    // sound_init();
    // thread5_game_loop();
    GameEngine::Instance->ProcessFrame(push_frame);
    GameEngine::Instance->Destroy();
    return 0;
}