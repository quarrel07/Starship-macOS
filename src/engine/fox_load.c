#include "global.h"
#include "sf64dma.h"

u8 sFillTimer = 3;

#include "fox_load_inits.c"

Scene sCurrentScene = {
    NO_OVERLAY,
    { /* 0x1 */ NO_SEGMENT,
      /* 0x2 */ NO_SEGMENT,
      /* 0x3 */ NO_SEGMENT,
      /* 0x4 */ NO_SEGMENT,
      /* 0x5 */ NO_SEGMENT,
      /* 0x6 */ NO_SEGMENT,
      /* 0x7 */ NO_SEGMENT,
      /* 0x8 */ NO_SEGMENT,
      /* 0x9 */ NO_SEGMENT,
      /* 0xA */ NO_SEGMENT,
      /* 0xB */ NO_SEGMENT,
      /* 0xC */ NO_SEGMENT,
      /* 0xD */ NO_SEGMENT,
      /* 0xE */ NO_SEGMENT,
      /* 0xF */ NO_SEGMENT },
};

void Load_RomFile(void* vRomAddress, void* dest, ptrdiff_t size) {
    s32 i;
    Lib_FillScreen(true);
    sFillTimer = 3;
    gGameStandby = true;
}

u8 Load_SceneFiles(Scene* scene) {

    sCurrentScene = *scene;
    return true;

    // u8* ramPtr = SEGMENT_VRAM_START(ovl_i1);
    // u8 segment;
    // u8 changeOvl = false;

    // if (ovlInit->ovl.rom.start == (0, sCurrentOverlay.ovl.rom.start)) { // fake because D_800CBDD4 is probably 2D array
    //     ramPtr = ramPtr + SEGMENT_SIZE(ovlInit->ovl.rom);
    //     ramPtr = ramPtr + SEGMENT_SIZE(ovlInit->ovl.bss);
    // } else {
    //     sCurrentOverlay.ovl.rom.start = ovlInit->ovl.rom.start;
    //     sCurrentOverlay.ovl.rom.end = ramPtr;
    //     if (ovlInit->ovl.rom.start != 0) {
    //         changeOvl = true;
    //         Overlay_LoadSegment(ovlInit->ovl.rom.start, ramPtr, SEGMENT_SIZE(ovlInit->ovl.rom));
    //         ramPtr = ramPtr + SEGMENT_SIZE(ovlInit->ovl.rom);
    //         bzero(ovlInit->ovl.bss.start, SEGMENT_SIZE(ovlInit->ovl.bss));
    //         ramPtr = ramPtr + SEGMENT_SIZE(ovlInit->ovl.bss);
    //     }
    // }
    // segment = 0;
    // while ((segment < 15) && (ovlInit->assets[segment].start == sCurrentOverlay.assets[segment].start) &&
    //        changeOvl == 0) {
    //     if (ovlInit->assets[segment].start != 0) {
    //         gSegments[segment + 1] = K0_TO_PHYS(ramPtr);
    //         gSPSegment(gUnkDisp1++, segment + 1, K0_TO_PHYS(ramPtr));
    //         ramPtr = ramPtr + SEGMENT_SIZE(ovlInit->assets[segment]);
    //     }
    //     segment += 1;
    // }
    // for (segment; segment < 15; segment += 1) {
    //     sCurrentOverlay.assets[segment].start = ovlInit->assets[segment].start;
    //     sCurrentOverlay.assets[segment].end = ramPtr;
    //     if (ovlInit->assets[segment].start != 0) {
    //         gSegments[segment + 1] = K0_TO_PHYS(ramPtr);
    //         gSPSegment(gUnkDisp1++, segment + 1, K0_TO_PHYS(ramPtr));
    //         Overlay_LoadSegment(ovlInit->assets[segment].start, ramPtr, SEGMENT_SIZE(ovlInit->assets[segment]));
    //         ramPtr = ramPtr + SEGMENT_SIZE(ovlInit->assets[segment]);
    //     }
    // }

    // if (sFillTimer != 0) {
    //     sFillTimer--;
    // } else if (gStartNMI == 0) {
    //     Lib_FillScreen(false);
    // }
    // return changeOvl;
}

u8 Load_SceneSetup(u8 sceneId, u8 sceneSetup) {
    u8 changeScene;

    switch (sceneId) {
        case SCENE_TITLE:
            changeScene = Load_SceneFiles(&sOvlmenu_Title[sceneSetup]);
            if (changeScene == true) {
                AUDIO_SET_SPEC(SFXCHAN_0, AUDIOSPEC_22);
            }
            break;
        case SCENE_MENU:
            changeScene = Load_SceneFiles(&sOvlmenu_Option[sceneSetup]);
            break;
        case SCENE_MAP:
            changeScene = Load_SceneFiles(&sOvlmenu_Map[sceneSetup]);
            break;
        case SCENE_GAME_OVER:
            changeScene = Load_SceneFiles(&sOvlmenu_GameOver[sceneSetup]);
            break;
        case SCENE_CORNERIA:
            changeScene = Load_SceneFiles(&sOvli1_Corneria[sceneSetup]);
            break;
        case SCENE_METEO:
            changeScene = Load_SceneFiles(&sOvli2_Meteo[sceneSetup]);
            break;
        case SCENE_TITANIA:
            changeScene = Load_SceneFiles(&sOvli5_Titania[sceneSetup]);
            break;
        case SCENE_SECTOR_X:
            changeScene = Load_SceneFiles(&sOvli2_SectorX[sceneSetup]);
            break;
        case SCENE_SECTOR_Z:
            changeScene = Load_SceneFiles(&sOvli4_SectorZ[sceneSetup]);
            break;
        case SCENE_AQUAS:
            changeScene = Load_SceneFiles(&sOvli3_Aquas[sceneSetup]);
            break;
        case SCENE_AREA_6:
            changeScene = Load_SceneFiles(&sOvli3_Area6[sceneSetup]);
            break;
        case SCENE_FORTUNA:
            changeScene = Load_SceneFiles(&sOvli4_Fortuna[sceneSetup]);
            break;
        case SCENE_UNK_4:
            changeScene = Load_SceneFiles(&sOvli3_Unk4[sceneSetup]);
            break;
        case SCENE_SECTOR_Y:
            changeScene = Load_SceneFiles(&sOvli6_SectorY[sceneSetup]);
            break;
        case SCENE_SOLAR:
            changeScene = Load_SceneFiles(&sOvli3_Solar[sceneSetup]);
            break;
        case SCENE_ZONESS:
            changeScene = Load_SceneFiles(&sOvli3_Zoness[sceneSetup]);
            break;
        case SCENE_VENOM_ANDROSS:
            changeScene = Load_SceneFiles(&sOvli6_Andross[sceneSetup]);
            break;
        case SCENE_TRAINING:
            changeScene = Load_SceneFiles(&sOvli1_Training[sceneSetup]);
            break;
        case SCENE_VENOM_1:
            changeScene = Load_SceneFiles(&sOvli1_Venom1[sceneSetup]);
            break;
        case SCENE_VENOM_2:
            changeScene = Load_SceneFiles(&sOvli6_Venom2[sceneSetup]);
            break;
        case SCENE_20:
            changeScene = Load_SceneFiles(&sOvli2_Setup20[sceneSetup]);
            break;
        case SCENE_BOLSE:
            changeScene = Load_SceneFiles(&sOvli4_Bolse[sceneSetup]);
            break;
        case SCENE_KATINA:
            changeScene = Load_SceneFiles(&sOvli4_Katina[sceneSetup]);
            break;
        case SCENE_MACBETH:
            changeScene = Load_SceneFiles(&sOvli5_Macbeth[sceneSetup]);
            break;
        case SCENE_VERSUS:
            changeScene = Load_SceneFiles(&sOvli2_Versus[sceneSetup]);
            if (changeScene == true) {
                // AUDIO_SET_SPEC_ALT(SFXCHAN_3, AUDIOSPEC_16);
            }
            break;
        case SCENE_LOGO:
            changeScene = Load_SceneFiles(&sNoOvl_Logo[sceneSetup]); // Logo does not load an overlay file
            if (changeScene == true) {
                // AUDIO_SET_SPEC(SFXCHAN_0, AUDIOSPEC_MA);
            }
            break;
        case SCENE_CREDITS:
            changeScene = Load_SceneFiles(&sOvlending_Ending[sceneSetup]);
            break;
        default:
            PRINTF("DMA MODE ERROR %d\n");
            changeScene = false;
            break;
    }
    return changeScene;
}

void Load_InitDmaAndMsg(void) {
    // Lib_DmaRead(SEGMENT_ROM_START(dma_table), SEGMENT_VRAM_START(dma_table), SEGMENT_ROM_SIZE(dma_table));
    // Load_RomFile(SEGMENT_ROM_START(ast_radio), SEGMENT_VRAM_START(ast_radio), SEGMENT_ROM_SIZE(ast_radio));
}
