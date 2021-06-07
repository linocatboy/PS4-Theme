#pragma once
#include <stdint.h>
#include "vector"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <string>

constexpr uint64_t CURRENT_GAME_STATE_ADDRESS = 0x0000000140EDA810;				// from TLAC
constexpr uint64_t CURRENT_GAME_SUB_STATE_ADDRESS = 0x0000000140EDA82C;			// from TLAC
constexpr uint64_t TaskInfoSetterAddress = 0x00000001403BB780;
constexpr uint64_t SelPvMainAddress = 0x00000001405BA2E0;

enum GameState : uint32_t					// from TLAC
{
	GS_STARTUP,
	GS_ADVERTISE,
	GS_GAME,
	GS_DATA_TEST,
	GS_TEST_MODE,
	GS_APP_ERROR,
	GS_MAX,
};

enum SubGameState : uint32_t				// from TLAC
{
	SUB_DATA_INITIALIZE,
	SUB_SYSTEM_STARTUP,
	SUB_SYSTEM_STARTUP_ERROR,
	SUB_WARNING,
	SUB_LOGO,
	SUB_RATING,
	SUB_DEMO,
	SUB_TITLE,
	SUB_RANKING,
	SUB_SCORE_RANKING,
	SUB_CM,
	SUB_PHOTO_MODE_DEMO,
	SUB_SELECTOR,
	SUB_GAME_MAIN,
	SUB_GAME_SEL,
	SUB_STAGE_RESULT,
	SUB_SCREEN_SHOT_SEL,
	SUB_SCREEN_SHOT_RESULT,
	SUB_GAME_OVER,
	SUB_DATA_TEST_MAIN,
	SUB_DATA_TEST_MISC,
	SUB_DATA_TEST_OBJ,
	SUB_DATA_TEST_STG,
	SUB_DATA_TEST_MOT,
	SUB_DATA_TEST_COLLISION,
	SUB_DATA_TEST_SPR,
	SUB_DATA_TEST_AET,
	SUB_DATA_TEST_AUTH_3D,
	SUB_DATA_TEST_CHR,
	SUB_DATA_TEST_ITEM,
	SUB_DATA_TEST_PERF,
	SUB_DATA_TEST_PVSCRIPT,
	SUB_DATA_TEST_PRINT,
	SUB_DATA_TEST_CARD,
	SUB_DATA_TEST_OPD,
	SUB_DATA_TEST_SLIDER,
	SUB_DATA_TEST_GLITTER,
	SUB_DATA_TEST_GRAPHICS,
	SUB_DATA_TEST_COLLECTION_CARD,
	SUB_TEST_MODE_MAIN,
	SUB_APP_ERROR,
	SUB_MAX,
};

void InjectCode(void* address, const std::vector<uint8_t> data)
{
	const size_t byteCount = data.size() * sizeof(uint8_t);

	DWORD oldProtect;
	VirtualProtect(address, byteCount, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(address, data.data(), byteCount);
	VirtualProtect(address, byteCount, oldProtect, nullptr);
}

std::wstring ExePath() {
	WCHAR buffer[MAX_PATH];
	GetModuleFileNameW(NULL, buffer, MAX_PATH);
	return std::wstring(buffer);
}

std::wstring DirPath() {
	std::wstring exepath = ExePath();
	std::wstring::size_type pos = exepath.find_last_of(L"\\/");
	return exepath.substr(0, pos);
}

std::wstring CONFIG_FILE_STRING = DirPath() + L"\\plugins\\PS4Theme.ini";
LPCWSTR CONFIG_FILE = CONFIG_FILE_STRING.c_str();