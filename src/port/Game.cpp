#include <libultraship.h>

#include <Fast3D/interpreter.h>
#include "Engine.h"

extern "C" {
#include <sf64mesg.h>
    void Main_SetVIMode(void);
    void Main_Initialize(void);
    void Main_ThreadEntry(void* arg);
    void Lib_FillScreen(u8 setFill);
    void Graphics_ThreadUpdate();
    void AudioThread_CreateTask();
}

extern "C"
void Graphics_PushFrame(Gfx* data) {
    GameEngine::ProcessGfxCommands(data);
}

extern "C" void Timer_Update();

void push_frame() {
    Graphics_ThreadUpdate();
    GameEngine::StartAudioFrame();
    GameEngine::Instance->StartFrame();
    Timer_Update();
    // thread5_iteration();
    GameEngine::EndAudioFrame();
}

#ifdef _WIN32
int SDL_main(int argc, char **argv) {
#else
#if defined(__cplusplus) && defined(PLATFORM_IOS)
extern "C"
#endif
int main(int argc, char *argv[]) {
#endif
#if defined(__APPLE__)
    // Default the writable data/"library" folder to ~/Library/Application Support/com.starship.
    // This mirrors the SHIP_HOME value in the .app's Info.plist (LSEnvironment); overwrite=0
    // means a value provided by the bundle or the user's environment still takes precedence.
    setenv("SHIP_HOME", "~/Library/Application Support/com.starship", 0);
#endif
    GameEngine::Create();
    Main_SetVIMode();
    Lib_FillScreen(1);
    Main_Initialize();
    Main_ThreadEntry(NULL);
    while (WindowIsRunning()) {
        push_frame();
    }
    GameEngine::Instance->Destroy();
    return 0;
}