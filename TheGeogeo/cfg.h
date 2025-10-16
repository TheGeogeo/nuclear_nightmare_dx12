#pragma once

inline namespace Configuration
{
	class Settings : public Singleton<Settings>
	{
	public:
		bool b_MenuShow = true;

		// esp
		bool b_espGlobal = false;
		bool b_espPlayers = false;
		bool b_espPlayerRect = false;
		bool b_espPlayerSkeleton = false;
		bool b_espPlayerName = false;
		bool b_espPlayerInfected = false;

		bool b_espMobs = false;
		bool b_espHuskyRect = false;
		bool b_espHuskySkeleton = false;

		bool b_espGenerator = false;
		bool b_espLaserBomb = false;
		bool b_espItems = false;
		bool b_espSodaMachine = false;
		bool b_espComputerRadio = false;

		// esp value
		float f_espMaxRender = 500.f;

		//color esp
		ImColor boxVisiblePlayerColor = ImColor{ 55,255,0 };
		float f_boxVisiblePlayerColor[3] = { 0.21568627f, 1.f, 0.21568627f };

		ImColor boxNotVisiblePlayerColor = ImColor{ 255,55,0 };
		float f_boxNotVisiblePlayerColor[3] = { 1.f, 0.21568627f, 0.21568627f };

		ImColor boxVisibleHuskyColor = ImColor{ 55,255,0 };
		float f_boxVisibleHuskyColor[3] = { 0.21568627f, 1.f, 0.21568627f };

		ImColor boxNotVisibleHuskyColor = ImColor{ 255,55,0 };
		float f_boxNotVisibleHuskyColor[3] = { 1.f, 0.21568627f, 0.21568627f };

		ImColor skeletonColor = ImColor{ 0, 234, 255 };
		float f_skeletonColor[3] = { 0.f, 0.917647f, 1.f };

		ImColor sodaMachineColor = ImColor{ 203, 33, 255 };
		float f_sodaMachineColor[3] = { 0.796078f, 0.129412f, 1.f };

		// misc
		bool b_miscRemoveFog = false;
		bool b_miscFullbright = false;
		bool b_miscInfAmmo = false;
		bool b_miscInfInvAmmo = false;
		bool b_miscInfFuelCan = false;
		bool b_miscInfFlashlight = false;
		bool b_miscInfHealth = false;
		bool b_miscNoFallDamage = false;
		bool b_miscNoTemperatureDecrease = false;
		bool b_miscInfJump = false;
		bool b_miscNeverInfected = false;
		bool b_miscUnlockAllDoor = false;
		bool b_miscNoClip = false;
		bool b_miscTpItem = false;
		bool b_miscTpItemsAround = false;

		//misc value
		int i_miscPointsToAdd = 500;
		float g_itemsRingAngle = 0.0f;
		float f_itemRingAngularSpeedRad = 0.0f;
		std::chrono::steady_clock::time_point g_lastTick;

		//troll
		bool b_tpAllPlayersToYou = false;

		//debug
		int tabIndex = 0;
		bool debugNoclip = false;
		bool debugAllActors = false;
		bool debugAllActorsOnlyCharacter = false;
		bool debugAllActorsOnlyCharacterBones = false;

		// Vector of actors
		std::vector<SDK::AActor*> v_allActorsDebug = {};
		std::vector<SDK::AActor*> v_allFirstPersonCharacters = {};
		std::vector<SDK::AActor*> v_allGenerators = {};
		std::vector<SDK::AActor*> v_allLasersBombs = {};
		std::vector<SDK::AActor*> v_allItems = {};
		std::vector<SDK::AActor*> v_allExameModularClassifieds = {};
		std::vector<SDK::AActor*> v_allHuskys = {};
		std::vector<SDK::AActor*> v_allDoorBases = {};
		std::vector<SDK::AActor*> v_allSkys = {};
		std::vector<SDK::AActor*> v_allWeathers = {};
		std::vector<SDK::AActor*> v_allSodaMachines = {};
		std::vector<SDK::AActor*> v_allRadioComputers = {};

		// const
		const std::vector<int32_t> playerBonnesToSkip = { 0 };
		const std::vector<int32_t> huskyBonnesToSkip = { 0, 1 };

		//GAMEVARS
		const char* dwProcessName = "NuclearNightmare-Win64-Shipping.exe";
		DWORD dwProcessId = NULL;
		uint64_t dwProcess_Base = NULL;
		HWND gameHWND = NULL;

		int ScreenHeight = NULL;
		int ScreenWidth = NULL;

		HMODULE hModMain = NULL;

		// color
		ImColor colorWhite = ImColor{ 255,255,255 };
		ImColor colorBlack = ImColor{ 0,0,0 };
		ImColor colorRed = ImColor{ 255,0,0 };
		ImColor colorGreen = ImColor{ 0,255,0 };
		ImColor colorBlue = ImColor{ 0,0,255 };
	};
#define CFG Configuration::Settings::Get()
}