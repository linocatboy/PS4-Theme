// dllmain.cpp : Defines the entry point for the DLL application.

#include "framework.h"
#include "PluginConfigApi.h"
#include <detours.h>

#pragma comment(lib, "detours.lib")
#include "vector"


void(*TaskInfoSetLevel)(void* cls, int level) = (void(*)(void*, int))TaskInfoSetterAddress;
void InjectCode(void* address, const std::vector<uint8_t> data);
void(*SelPvMain)(void* cls) = (void(*)(void*))SelPvMainAddress;



void HookedSelPvMain(void* param_1) {

    int edition = *(int*)0x141197E04;
    int isRandom = *(int*)0x14CC12019;

    if (edition == 1) 
    {

        // disable difficulty text on exex
        InjectCode((void*)0x140A3E0C8, { 0x00 }); // easy
        InjectCode((void*)0x140A3E120, { 0x00 }); // normal
        InjectCode((void*)0x140A3E178, { 0x00 }); // hard
        
        // Change hardcoded ids
        InjectCode((void*)0x140A3A620, { 0x71, 0xF3, 0x05, 0x00 }); // 390001 = PV_LIST_BASE_EXTRA_EXTREME_01
        InjectCode((void*)0x140A3A624, { 0x72, 0xF3, 0x05, 0x00 }); // 390002 = PV_LIST_BASE_EXTRA_EXTREME_02
        InjectCode((void*)0x140A3A660, { 0x73, 0xF3, 0x05, 0x00 }); // 390003 = PV_LIST_BASE_SEL_EXTRA_EXTREME_01
        InjectCode((void*)0x140A3A664, { 0x74, 0xF3, 0x05, 0x00 }); // 390004 = PV_LIST_BASE_SEL_EXTRA_EXTREME_02

        InjectCode((void*)0x140A3BAF9, { 0x31 }); // list_cursor_loop_1xtreme

        InjectCode((void*)0x140A3E1B8, { 0x65, 0x78, 0x74 }); // ext_level_extreme_a
    }
    else 
    {

        // disable difficulty text on exex
        InjectCode((void*)0x140A3E0c8, { 0x62 }); // easy
        InjectCode((void*)0x140A3E120, { 0x62 }); // normal
        InjectCode((void*)0x140A3E178, { 0x62 }); // hard
                               
        // Change hardcoded ids
        InjectCode((void*)0x140A3A620, { 0x30, 0x1B, 0x00, 0x00 }); // PV_LIST_BASE_EXTRA_EXTREME_01
        InjectCode((void*)0x140A3A624, { 0x31, 0x1B, 0x00, 0x00 }); // PV_LIST_BASE_EXTRA_EXTREME_02
        InjectCode((void*)0x140A3A660, { 0x2F, 0x2E, 0x00, 0x00 }); // PV_LIST_BASE_SEL_EXTRA_EXTREME_01
        InjectCode((void*)0x140A3A664, { 0x30, 0x2E, 0x00, 0x00 }); // PV_LIST_BASE_SEL_EXTRA_EXTREME_02
                               
        InjectCode((void*)0x140A3BAF9, { 0x65 }); // list_cursor_loop_extreme

        InjectCode((void*)0x140A3E1B8, { 0x62, 0x74, 0x6e }); // btn_level_extreme_a
    }

    if (isRandom == 1) 
    {
        InjectCode((void*)0x140A3B991, { 0x72, 0x61, 0x6E, 0x64 });                   // list_cursor_loop_rand (easy)
        InjectCode((void*)0x140A3BA09, { 0x72, 0x61, 0x6E, 0x64, 0x00, 0x00 });       // list_cursor_loop_rand (normal)
        InjectCode((void*)0x140A3BA81, { 0x72, 0x61, 0x6E, 0x64 });                   // list_cursor_loop_rand (hard)
        InjectCode((void*)0x140A3BAF9, { 0x72, 0x61, 0x6E, 0x64, 0x00, 0x00, 0x00 }); // list_cursor_loop_rand (extreme)
    }
    else {
        InjectCode((void*)0x140A3b991, { 0x65, 0x61, 0x73, 0x79 });                   // list_cursor_loop_easy    (easy)
        InjectCode((void*)0x140A3BA09, { 0x6E, 0x6F, 0x72, 0x6D, 0x61, 0x6C });       // list_cursor_loop_normal  (normal)
        InjectCode((void*)0x140A3bA81, { 0x68, 0x61, 0x72, 0x64 });                   // list_cursor_loop_hard    (hard)
        InjectCode((void*)0x140A3BAF9, { 0x65, 0x78, 0x74, 0x72, 0x65, 0x6D, 0x65 }); // list_cursor_loop_extreme (extreme)
    }


    SelPvMain(param_1);
}


void HookedTaskInfo(void* param_1, int param_2) {
    SubGameState currentSubGameState = *(SubGameState*)CURRENT_GAME_SUB_STATE_ADDRESS;          // credit to somewhatlurker

    if (currentSubGameState == SubGameState::SUB_STAGE_RESULT) 
    {
        param_2 = 4;
    }
    if (currentSubGameState == SubGameState::SUB_TITLE || currentSubGameState == SubGameState::SUB_GAME_MAIN)
    {
        param_2 = 0;
    }

    TaskInfoSetLevel(param_1, param_2);
}

void Patches() {

    int gameVer = GetPrivateProfileIntW(L"General", L"theme", 0, CONFIG_FILE);
    int bgmVer = GetPrivateProfileIntW(L"General", L"bgm", 0, CONFIG_FILE);
    bool isFontResOn = GetPrivateProfileIntW(L"general", L"fontResSupport", 1, CONFIG_FILE);
    bool isRsltCardInfoOn = GetPrivateProfileIntW(L"general", L"resultCardInfo", 1, CONFIG_FILE);

    if (!isRsltCardInfoOn) {
        InjectCode((void*)0x140A30058, { 0x00 }); // rslt_in
        InjectCode((void*)0x140A30080, { 0x00 }); // rslt_loop
        InjectCode((void*)0x140A300A8, { 0x00 }); // rslt_out
    }
    
    // from no_sel_bg_scrolling.p credit to somewhatlurker
    InjectCode((void*)0x1405bbf40, { 0x90, 0x90, 0x90, 0x90, 0x90 });
    InjectCode((void*)0x1405bbf70, { 0x90, 0x90, 0x90, 0x90, 0x90 });
    InjectCode((void*)0x1405bbfa0, { 0x90, 0x90, 0x90, 0x90, 0x90 });
    InjectCode((void*)0x1405bbfd0, { 0x90, 0x90, 0x90, 0x90, 0x90 });
    InjectCode((void*)0x1405bc009, { 0x90, 0x90, 0x90, 0x90, 0x90 });
    InjectCode((void*)0x1405bc039, { 0x90, 0x90, 0x90, 0x90, 0x90 });

    InjectCode((void*)0x1405c7b7b, { 0x41, 0xc6, 0xc4, 0x01 });
    InjectCode((void*)0x1405c8917, { 0x41, 0xc6, 0xc5, 0x01 });

    // from ps4_font.p; (advance_width, line_height, box_width, box_height)    credit to somewhatlurker 
    // font id 13 (song percentage)
    InjectCode((void*)0x1401856C4, { 0x2e });
    InjectCode((void*)0x1401856C1, { 0x2e });
    InjectCode((void*)0x1401856BE, { 0x30 });
    InjectCode((void*)0x1401856B9, { 0x30 });

    // font id 13 (gold song percentage)
    InjectCode((void*)0x1401856F7, { 0x2e });
    InjectCode((void*)0x1401856F4, { 0x2e });
    InjectCode((void*)0x1401856F1, { 0x30 });
    InjectCode((void*)0x1401856EC, { 0x30 });

    // font id 4 (performance counts/percentages)
    // (does not match the font's actual size due to AFT partially not using AET sizing for this font)
    InjectCode((void*)0x140185691, { 0x18 });
    InjectCode((void*)0x14018568E, { 0x16 });
    InjectCode((void*)0x14018568B, { 0x1a });
    InjectCode((void*)0x140185686, { 0x18 });

    //disable the colour overlay for new record scores
    InjectCode((void*)0x1406596F4, { 0xff });
    InjectCode((void*)0x1406596F5, { 0xff });
    InjectCode((void*)0x1406596FA, { 0xff });

    if (isFontResOn) 
    {

        // from font_res_support.p          credit to somewhatlurker

        // Load font res info from fontmap
        InjectCode((void*)0x14018783F, { 0xe9, 0xb0, 0x03, 0x00, 0x00 }); // JMP 0x140187bf4

        InjectCode((void*)0x140187BF4, { 0xe8, 0x27, 0x02, 0x00, 0x00 }); // CALL FUN_140187e20
        InjectCode((void*)0x140187BF9, { 0x48, 0x8b, 0x46, 0x04 });       // MOV RAX, qword ptr [RSI + 0x04]
        InjectCode((void*)0x140187BFD, { 0xeb, 0x17 });                   // JMP 0x140187c16

        InjectCode((void*)0x140187C16, { 0x49, 0x89, 0x46, 0xdc });       // MOV qword ptr [R14 + -0x24], RAX
        InjectCode((void*)0x140187C1A, { 0xe9, 0x25, 0xfc, 0xff, 0xff }); // JMP 0x140187844

        // reinitialise dwgui drawing stuff after loading fontmap
        // ideally I should destruct old stuff too, but it's not strictly necessary and I'm lazy

        InjectCode((void*)0x140187EDC, { 0xe8, 0x9f, 0xa6, 0x17, 0x00 });                       // CALL  0x140302580 (reinit stuff)
        InjectCode((void*)0x140187EE1, { 0xeb, 0xa3 });                                         // JMP  0x140187e86
                                  
        InjectCode((void*)0x140187E86, { 0x32, 0xc0, 0x48, 0x83, 0xc4, 0x20, 0x5b, 0xc3 });     // relocated from 0x140187edc
        InjectCode((void*)0x140187EBD, { 0x75, 0xc7 });

    }

    switch (bgmVer) {

    case 0: // Future Tone

        // Sounds
        InjectCode((void*)0x140A2F595, { 0x30, 0x30 }); // result_00_clear
        InjectCode((void*)0x140A3390F, { 0x30, 0x30 }); // selector_00_lp

        // AET
        InjectCode((void*)0x140A2F8EB, { 0x30, 0x30 }); // credit_00
        InjectCode((void*)0x140A2F8FB, { 0x30, 0x30 }); // credit_00op
        InjectCode((void*)0x140A2F90B, { 0x30, 0x30 }); // credit_00t

        InjectCode((void*)0x140A2F766, { 0x30, 0x30 }); // rslt_00
        InjectCode((void*)0x140A2F7F6, { 0x30, 0x30 }); // rslt_00op
        InjectCode((void*)0x140A2F826, { 0x30, 0x30 }); // rslt_00t


        printf("[PS4 Theme] Using Future Tone BGM\n");
        break;

    case 1: // DIVA Arcade

        // Sounds
        InjectCode((void*)0x140A2F595, { 0x30, 0x31 }); // result_01_clear
        InjectCode((void*)0x140A3390F, { 0x30, 0x31 }); // selector_01_lp

        // AET
        InjectCode((void*)0x140A2F8EB, { 0x30, 0x31 }); // credit_01
        InjectCode((void*)0x140A2F8FB, { 0x30, 0x31 }); // credit_01op
        InjectCode((void*)0x140A2F90B, { 0x30, 0x31 }); // credit_01t

        InjectCode((void*)0x140A2F766, { 0x30, 0x31 }); // rslt_01
        InjectCode((void*)0x140A2F7F6, { 0x30, 0x31 }); // rslt_01op
        InjectCode((void*)0x140A2F826, { 0x30, 0x31 }); // rslt_01t

        printf("[PS4 Theme] Using DIVA Arcade BGM\n");
        break;

    case 2: // DIVA Arcade Ver.B

        // Sounds
        InjectCode((void*)0x140A2F595, { 0x30, 0x32 }); // result_02_clear
        InjectCode((void*)0x140A3390F, { 0x30, 0x32 }); // selector_02_lp

        // AET
        InjectCode((void*)0x140A2F8EB, { 0x30, 0x32 }); // credit_02
        InjectCode((void*)0x140A2F8FB, { 0x30, 0x32 }); // credit_02op
        InjectCode((void*)0x140A2F90B, { 0x30, 0x32 }); // credit_02t

        InjectCode((void*)0x140A2F766, { 0x30, 0x32 }); // rslt_02
        InjectCode((void*)0x140A2F7F6, { 0x30, 0x32 }); // rslt_02op
        InjectCode((void*)0x140A2F826, { 0x30, 0x32 }); // rslt_02t

        printf("[PS4 Theme] Using DIVA Arcade Ver.B BGM\n");
        break;

    }

    switch (gameVer) {

    case 0:

        // adv_cmn
        InjectCode((void*)0x1409A5944, { 0x74, 0x69, 0x5F, 0x66, 0x74, 0x00 }); // ti_ft
        InjectCode((void*)0x1409A5950, { 0x74, 0x69, 0x5F, 0x66, 0x74, 0x00 }); // ti_ft
                                
        // cmn_all              
        InjectCode((void*)0x1409A660c, { 0x62, 0x30 }); // b0

        // gam_cmn
        InjectCode((void*)0x1409A4068, { 0x30 }); // 0ong_energy_base / song_energy_base
        InjectCode((void*)0x1409A4080, { 0x30 }); // 0ong_energy_base_base_reach / song_energy_base_reach
        InjectCode((void*)0x1409A41F0, { 0x30 }); // 0rame_bottom / frame_bottom
        InjectCode((void*)0x1409A4210, { 0x30 }); // 0rame_bottom_danger / frame_bottom_danger
        InjectCode((void*)0x1409A4200, { 0x30 }); // 0rame_up / frame_up
        InjectCode((void*)0x1409A4228, { 0x30 }); // 0rame_up_danger / frame_up_danger


        printf("[PS4 Theme] Using Future Tone Theme\n");
        break;

    case 1:

        // adv_cmn
        InjectCode((void*)0x1409A5944, { 0x74, 0x69, 0x5F, 0x66, 0x73, 0x00 }); // ti_fs
        InjectCode((void*)0x1409A5950, { 0x74, 0x69, 0x5F, 0x66, 0x73, 0x00 }); // ti_fs
                                
        // cmn_all              
        InjectCode((void*)0x1409A660C, { 0x62, 0x31 }); // b1

        // gam_cmn
        InjectCode((void*)0x1409A4068, { 0x31 }); // 1ong_energy_base / song_energy_base
        InjectCode((void*)0x1409A4080, { 0x31 }); // 1ong_energy_base_base_reach / song_energy_base_reach
        InjectCode((void*)0x1409A41F0, { 0x31 }); // 1rame_bottom / frame_bottom
        InjectCode((void*)0x1409A4210, { 0x31 }); // 1rame_bottom_danger / frame_bottom_danger
        InjectCode((void*)0x1409A4200, { 0x31 }); // 1rame_up / frame_up
        InjectCode((void*)0x1409A4228, { 0x31 }); // 1rame_up_danger / frame_up_danger

        // loadscp
        InjectCode((void*)0x140a92e58, { 0x5F, 0x66, 0x73 }); // load_fs

        printf("[PS4 Theme] Using Future Sound Theme\n");
        break;

    case 2:

        // adv_cmn
        InjectCode((void*)0x1409A5944, { 0x74, 0x69, 0x5F, 0x63, 0x74, 0x00 }); // ti_ct
        InjectCode((void*)0x1409A5950, { 0x74, 0x69, 0x5F, 0x63, 0x74, 0x00 }); // ti_ct
                                
        // cmn_all              
        InjectCode((void*)0x1409A660C, { 0x62, 0x32 }); // b2

        // gam_cmn
        InjectCode((void*)0x1409A4068, { 0x32 }); // 2ong_energy_base / song_energy_base
        InjectCode((void*)0x1409A4080, { 0x32 }); // 2ong_energy_base_base_reach / song_energy_base_reach
        InjectCode((void*)0x1409A41F0, { 0x32 }); // 2rame_bottom / frame_bottom
        InjectCode((void*)0x1409A4210, { 0x32 }); // 2rame_bottom_danger / frame_bottom_danger
        InjectCode((void*)0x1409A4200, { 0x32 }); // 2rame_up / frame_up
        InjectCode((void*)0x1409A4228, { 0x32 }); // 2rame_up_danger / frame_up_danger

        // loadscp
        InjectCode((void*)0x140a92e58, { 0x5F, 0x63, 0x74 }); // load_ct

        printf("[PS4 Theme] Using Colorful Tone Theme\n");
        break;

    case 3:

        // adv_cmn
        InjectCode((void*)0x1409A5944, { 0x74, 0x69, 0x5F, 0x64, 0x78, 0x00 }); // ti_dx
        InjectCode((void*)0x1409A5950, { 0x74, 0x69, 0x5F, 0x64, 0x78, 0x00 }); // ti_dx
                                
        // cmn_all              
        InjectCode((void*)0x1409A660C, { 0x62, 0x30 }); // b0
                                
        // gam_cmn              
        InjectCode((void*)0x1409A4068, { 0x30 }); // 0ong_energy_base / song_energy_base
        InjectCode((void*)0x1409A4080, { 0x30 }); // 0ong_energy_base_base_reach / song_energy_base_reach
        InjectCode((void*)0x1409A41F0, { 0x30 }); // 0rame_bottom / frame_bottom
        InjectCode((void*)0x1409A4210, { 0x30 }); // 0rame_bottom_danger / frame_bottom_danger
        InjectCode((void*)0x1409A4200, { 0x30 }); // 0rame_up / frame_up
        InjectCode((void*)0x1409A4228, { 0x30 }); // 0rame_up_danger / frame_up_danger

        printf("[PS4 Theme] Using Future Tone Deluxe Theme\n");
        break;

    }
}

bool isExexMenuOn = GetPrivateProfileIntW(L"general", L"exexMenu", 1, CONFIG_FILE);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:

        printf("[PS4 Theme] Initialized\n");

        Patches();

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)TaskInfoSetLevel, HookedTaskInfo);
        DetourTransactionCommit();

        if (isExexMenuOn)
        {
            DetourTransactionBegin();
            DetourUpdateThread(GetCurrentThread());
            DetourAttach(&(PVOID&)SelPvMain, HookedSelPvMain);
            DetourTransactionCommit();
        }

    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

using namespace PluginConfig;

PluginConfigOption config[] = {
    { CONFIG_DROPDOWN_INDEX, new PluginConfigDropdownIndexData{L"theme", L"General", CONFIG_FILE, L"Menu Theme:", L"Sets the current Menu Theme to be used.", 0, std::vector<LPCWSTR>({ L"Future Tone", L"Future Sound", L"Colorful Tone", L"Future Tone DX" }) } },
    { CONFIG_DROPDOWN_INDEX, new PluginConfigDropdownIndexData{L"bgm", L"General", CONFIG_FILE, L"BGM Theme:", L"Sets the current BGM Theme to be used.", 0, std::vector<LPCWSTR>({ L"Future Tone", L"DIVA Arcade", L"DIVA Arcade Ver.B" }) } },
    { CONFIG_BOOLEAN, new PluginConfigBooleanData{L"fontResSupport", L"General", CONFIG_FILE, L"Font Res Support", L"Enable Font Res Support.", true, false } },
    { CONFIG_BOOLEAN, new PluginConfigBooleanData{L"exexMenu", L"General", CONFIG_FILE, L"Closely Accurate Extra Extreme Menu", L"Makes the Extra Extreme menu look closer to the original. (may degrade performance)", true, false } },
    { CONFIG_BOOLEAN, new PluginConfigBooleanData{L"resultCardInfo", L"General", CONFIG_FILE, L"Enable Result CardInfo", L"Enables the cardinfo composition when in gam_result.", true, false } },
};

extern "C" __declspec(dllexport) LPCWSTR GetPluginName(void)
{
    return L"PS4 Theme";
}

extern "C" __declspec(dllexport) LPCWSTR GetPluginDescription(void)
{
    return L"A plugin that assists MPS4.";
}

extern "C" __declspec(dllexport) PluginConfigArray GetPluginOptions(void)
{
    return PluginConfigArray{ _countof(config), config };
}

