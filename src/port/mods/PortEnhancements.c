#include "PortEnhancements.h"
#include "port/hooks/Events.h"
#include "global.h"
#include "hit64.h"
#include "mods.h"

void OnDisplayUpdatePre(IEvent* event) {
#if DEBUG_BOSS_KILLER == 1
    KillBoss();
#endif

    if (CVarGetInteger("gDebugSpeedControl", 0) == 1) {
        Player* player = gPlayer;
        static s32 prevSpeed;
        static bool debugFreeze = false;

        if (gControllerPress[0].button & L_JPAD) {
            player->baseSpeed -= 50;
        } else if (gControllerPress[0].button & R_JPAD) {
            player->baseSpeed += 50;
        }

        if ((!debugFreeze) && (gControllerPress[0].button & D_JPAD)) {
            prevSpeed = player->baseSpeed;
            player->baseSpeed = 0;
            debugFreeze = true;
        } else if ((debugFreeze) && (gControllerPress[0].button & D_JPAD)) {
            player->baseSpeed = prevSpeed;
            debugFreeze = false;
        }
    }

    if (CVarGetInteger("gDebugJumpToMap", 0) == 1) {
        Player* pl = &gPlayer[0];

        if ((gGameState != GSTATE_PLAY) || (gPlayState <= PLAY_INIT)) {
            return;
        }

        if ((gControllerHold[0].button & Z_TRIG) && (gControllerHold[0].button & R_TRIG) &&
            (gControllerPress[0].button & U_CBUTTONS)) {
            gFillScreenAlphaTarget = 255;
            gFillScreenRed = gFillScreenGreen = gFillScreenBlue = 0;
            gFillScreenAlphaStep = 8;
            gShowLevelClearStatusScreen = false;
            pl->state = PLAYERSTATE_NEXT;
            pl->csTimer = 0;
            gFadeoutType = 4;
        }
    }

    if (CVarGetInteger("gDebugWarpZone", 0) == 1) {
        if ((gGameState != GSTATE_PLAY) || (gPlayState <= PLAY_INIT)) {
            return;
        }
        if (gControllerPress[0].button & L_TRIG) {
            if ((gCurrentLevel != LEVEL_SECTOR_X) && (gCurrentLevel != LEVEL_METEO)) {
                return;
            }
            if (gCurrentLevel == LEVEL_SECTOR_X) {
                gRingPassCount++;
                gPlayer[0].state = PLAYERSTATE_ENTER_WARP_ZONE;
                gPlayer[0].csState = 0;
                gSceneSetup = 1;
                AUDIO_PLAY_SFX(NA_SE_WARP_HOLE, gDefaultSfxSource, 0);
                gMissionStatus = MISSION_WARP;
                gLeveLClearStatus[gCurrentLevel] = 1;
            } else {
                gPlayer[0].state = PLAYERSTATE_ENTER_WARP_ZONE;
                gPlayer[0].csState = 0;
                AUDIO_PLAY_SFX(NA_SE_WARP_HOLE, gDefaultSfxSource, 0);
                gMissionStatus = MISSION_WARP;
                gLeveLClearStatus[gCurrentLevel] = 1;
            }
        }
    }

    if (CVarGetInteger("gDebugNoCollision", 0) == 1) {
        if ((gGameState != GSTATE_PLAY) || (gPlayState <= PLAY_INIT)) {
            return;
        }
        gPlayer->mercyTimer = 1000;
    }

    if (CVarGetInteger("gDebugLevelComplete", 0) == 1) {
        Player* pl = &gPlayer[0];
        if ((gGameState != GSTATE_PLAY) || (gPlayState <= PLAY_INIT)) {
            return;
        }

        if (gControllerPress[0].button & L_TRIG) {
            pl->state = PLAYERSTATE_LEVEL_COMPLETE;
            gMissionStatus = MISSION_ACCOMPLISHED;
        }
    }

    if (CVarGetInteger("gDebugJumpToAllRange", 0) == 1) {
        Player* pl2 = &gPlayer[0];

        if ((gLevelMode != LEVELMODE_ALL_RANGE) && (gControllerPress[0].button & L_TRIG)) {
            pl2->state = PLAYERSTATE_START_360;
        }
    }

    // Cheats start here

    if (CVarGetInteger("gInfiniteLives", 0) == 1) {
        if ((gGameState != GSTATE_PLAY) || (gPlayState <= PLAY_INIT)) {
            return;
        }
        gLifeCount[0] = 9;
    }

    if (CVarGetInteger("gInfiniteBombs", 0) == 1) {
        if ((gGameState != GSTATE_PLAY) || (gPlayState <= PLAY_INIT)) {
            return;
        }
        gBombCount[0] = 9;
    }

    if (CVarGetInteger("gHyperLaser", 0) == 1) {
        if ((gGameState != GSTATE_PLAY) || (gPlayState <= PLAY_INIT)) {
            return;
        }
        gLaserStrength[0] = LASERS_HYPER;
    }

    if (CVarGetInteger("gScoreEditor", 0) == 1) {
        if ((gGameState != GSTATE_PLAY) || (gPlayState <= PLAY_INIT)) {
            return;
        }
        gHitCount = CVarGetInteger("gScoreEditValue", 1);

    }
    Hit64_Main();
    // ground testing
#if 0
    RCP_SetupDL(&gMasterDisp, SETUPDL_83);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 0, 255);
    Graphics_DisplaySmallText(10, 210, 1.0f, 1.0f, "STICK_X:");
    Graphics_DisplaySmallNumber(60, 210, (int) ABS(gInputPress->stick_x));
    Graphics_DisplaySmallText(10, 220, 1.0f, 1.0f, "STICK_Y:");
    Graphics_DisplaySmallNumber(60, 220, (int) ABS(gInputPress->stick_y));
    if (gInputPress->stick_x < 0) Graphics_DisplaySmallText(110, 210, 1.0f, 1.0f, "NEG:");
    if (gInputPress->stick_y < 0) Graphics_DisplaySmallText(110, 220, 1.0f, 1.0f, "NEG:");
#endif
}

void OnGameUpdatePost(IEvent* event) {
#if MODS_RAM_MOD == 1
    RamMod_Update();
#endif
    if(CVarGetInteger("gSpawnerMod", 0) == 1){
        Spawner();
    }
}

void PortEnhancements_Init() {
    // Register event listeners
    EventSystem_RegisterListener(DisplayPreUpdateEvent, OnDisplayUpdatePre, EVENT_PRIORITY_NORMAL);
    EventSystem_RegisterListener(GamePostUpdateEvent, OnGameUpdatePost, EVENT_PRIORITY_NORMAL);
}

void PortEnhancements_Exit() {
    // TODO: Unregister event listeners
}