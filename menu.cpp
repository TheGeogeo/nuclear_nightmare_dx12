#include "includes.h"

namespace menu {
	bool isOpen = false;

	void Init() {
		ImGui::GetIO().MouseDrawCursor = isOpen;

		if (isOpen)
		{
			ImGui::SetNextWindowSize(ImVec2(675, 530));
			ImGui::Begin("Nuclear Nightmare", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

			overlay::TabButton("Visuals", &CFG.tabIndex, 0, true);
			overlay::TabButton("Misc", &CFG.tabIndex, 1, true);
			overlay::TabButton("Troll (host)", &CFG.tabIndex, 2, true, ImVec2(110, 25));
			overlay::TabButton("Settings", &CFG.tabIndex, 3, true);

			overlay::TabButton("About", &CFG.tabIndex, 8, true);
			//overlay::TabButton("Debug", &CFG.tabIndex, 9, true);

			ImGui::Separator();

			//stuff GUI
			if (CFG.tabIndex == 0) // ESP
			{
				ImGui::Checkbox("ESP Global", &CFG.b_espGlobal);
				if (CFG.b_espGlobal)
				{
					ImGui::Dummy(ImVec2{ 0.f, 10.f });
					ImGui::SliderFloat("Max render distance", &CFG.f_espMaxRender, 1.f, 3000.f);
					ImGui::Dummy(ImVec2{ 0.f, 10.f });

					ImGui::Checkbox("Players ESP", &CFG.b_espPlayers);
					if (CFG.b_espPlayers)
					{
						ImGui::Checkbox("Player name", &CFG.b_espPlayerName);
						ImGui::Checkbox("Player infected", &CFG.b_espPlayerInfected);
						ImGui::Checkbox("Player 2D Box", &CFG.b_espPlayerRect);
						ImGui::Checkbox("Player skeleton", &CFG.b_espPlayerSkeleton);
					}
					ImGui::Dummy(ImVec2{ 0.f, 10.f });

					ImGui::Checkbox("Mobs ESP", &CFG.b_espMobs);
					if (CFG.b_espMobs)
					{
						ImGui::Checkbox("Husky 2D Box", &CFG.b_espHuskyRect);
						ImGui::Checkbox("Husky skeleton", &CFG.b_espHuskySkeleton);
					}
					ImGui::Dummy(ImVec2{ 0.f, 10.f });

					ImGui::Checkbox("Generator ESP (circle)", &CFG.b_espGenerator);
					ImGui::Checkbox("Laser bomb ESP (square)", &CFG.b_espLaserBomb);
					ImGui::Checkbox("Items ESP (triangle)", &CFG.b_espItems);
					ImGui::Checkbox("Computer ESP (hexagon)", &CFG.b_espComputerRadio);
					ImGui::Checkbox("Soda machine ESP", &CFG.b_espSodaMachine);
				}
			}

			if (CFG.tabIndex == 1) // Misc
			{
				ImGui::InputInt("Points to add", &CFG.i_miscPointsToAdd, 100, 10000);
				ImGui::SameLine();
				if (ImGui::Button("Add points") && ue->myCharacter)
				{
					ue->myCharacter->AddPoints(CFG.i_miscPointsToAdd);
				}
				ImGui::Dummy(ImVec2{ 0.f, 10.f });

				ImGui::Checkbox("Full bright (remove fog)", &CFG.b_miscFullbright);
				if (ImGui::Checkbox("Remove fog", &CFG.b_miscRemoveFog))
				{
					SDK::AActor* weather = nullptr;
					if (!CFG.b_miscRemoveFog && !CFG.v_allWeathers.empty() && (weather = CFG.v_allWeathers.front()) != nullptr) {
						SDK::AUltra_Dynamic_Weather_C* customWeather = reinterpret_cast<SDK::AUltra_Dynamic_Weather_C*>(weather);
						customWeather->Fog = 8.0; // reset fog
					}
				}
				ImGui::Checkbox("Unlock all doors (host)", &CFG.b_miscUnlockAllDoor);
				if (ImGui::Button("Reactive all soda machines") && ue->World && ue->myCharacter)
				{
					for (int i = 0; i < CFG.v_allSodaMachines.size(); i++)
					{
						SDK::ASodaMachineBP_C* sodaMachine = reinterpret_cast<SDK::ASodaMachineBP_C*>(CFG.v_allSodaMachines[i]);
						if (!sodaMachine) continue;

						sodaMachine->Re_EnablePerkMachine();
					}
				}
				if (ImGui::Button("Self kill") && ue->myCharacter)
				{
					ue->myCharacter->KillAll();
				}
				if (ImGui::Button("Respawn first dead player (host)") && ue->World && CFG.v_allFirstPersonCharacters.size() > 0)
				{
					SDK::ABP_Radio_C* closestComputerRadio = nullptr;
					float closestDist = D3D12_FLOAT32_MAX;

					for (int i = 0; i < CFG.v_allRadioComputers.size(); i++)
					{
						SDK::ABP_Radio_C* computerRadio = reinterpret_cast<SDK::ABP_Radio_C*>(CFG.v_allRadioComputers[i]);
						if (!computerRadio) continue;

						SDK::FVector locationActor = computerRadio->K2_GetActorLocation();
						float distanceWithActor = ue->Distance(locationActor);

						if (closestDist > distanceWithActor)
						{
							closestDist = distanceWithActor;
							closestComputerRadio = computerRadio;
						}
					}

					if (closestComputerRadio)
					{
						closestComputerRadio->SpawnReinformant();
					}
				}
				if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort)) {
					ImGui::BeginTooltip();
					ImGui::TextUnformatted("Force respawn player was dead first, respawn to closest computer");
					ImGui::EndTooltip();
				}
				ImGui::Dummy(ImVec2{ 0.f, 10.f });

				if (ImGui::Checkbox("Inf health (it's not a god mode)", &CFG.b_miscInfHealth) && ue->myCharacter && ue->myCharacter->Health > 0.f) {
					ue->myCharacter->UpdateHealth(100.0);
				}
				ImGui::Checkbox("Never infected (host)", &CFG.b_miscNeverInfected);
				if (ImGui::Checkbox("No fall damage", &CFG.b_miscNoFallDamage) && ue->myCharacter && ue->myCharacter->Health > 0.f && !CFG.b_miscNoFallDamage)
				{
					ue->myCharacter->CanTakeFallDamage = 1;
				}
				if (ImGui::Checkbox("No Temperature decrease", &CFG.b_miscNoTemperatureDecrease) && ue->myCharacter && ue->myCharacter->Health > 0.f && !CFG.b_miscNoTemperatureDecrease)
				{
					ue->myCharacter->CanTakeTemperature = 1;
				}
				if (ImGui::Checkbox("Inf jump (host)", &CFG.b_miscInfJump) && ue->myCharacter && ue->myCharacter->Health > 0.f && !CFG.b_miscInfJump)
				{
					ue->myCharacter->JumpMaxCount = 1;
				}
				ImGui::Dummy(ImVec2{ 0.f, 10.f });

				ImGui::Checkbox("Inf ammo", &CFG.b_miscInfAmmo);
				ImGui::Checkbox("Inf inventory ammo", &CFG.b_miscInfInvAmmo);
				ImGui::Checkbox("Inf flashlight", &CFG.b_miscInfFlashlight);
				ImGui::Checkbox("Inf gas canster", &CFG.b_miscInfFuelCan);
				ImGui::Dummy(ImVec2{ 0.f, 10.f });

				ImGui::Checkbox("Tp specific item front of you (host)", &CFG.b_miscTpItem);
				ImGui::Checkbox("Tp all items around you (host)", &CFG.b_miscTpItemsAround);
				ImGui::Dummy(ImVec2{ 0.f, 10.f });

				if (ImGui::Checkbox("NoClip (host)", &CFG.b_miscNoClip) && ue->PlayerController)
				{
					if (!CFG.b_miscNoClip)
					{
						ue->ToggleNoClip(CFG.b_miscNoClip);
					}
				}
			}

			if (CFG.tabIndex == 2) // Troll
			{
				if (ue->World && ue->myCharacter && ue->PlayerController)
				{
					if (ImGui::Button("Tp all players to you"))
					{
						CFG.b_tpAllPlayersToYou = true;
					}
					ImGui::Dummy({ 0.f, 10.f });

					for (int i = 0; i < CFG.v_allFirstPersonCharacters.size(); i++)
					{
						SDK::ABP_FirstPersonCharacter_C* player = reinterpret_cast<SDK::ABP_FirstPersonCharacter_C*>(CFG.v_allFirstPersonCharacters[i]);
						if (!player) continue;
						if ((uintptr_t)player == (uintptr_t)ue->myCharacter) continue;
						if (0.f > ue->myCharacter->Health) continue;

						if (CFG.b_tpAllPlayersToYou)
						{
							player->K2_TeleportTo(ue->myCharacter->K2_GetActorLocation(), player->K2_GetActorRotation());
						}

						std::string playerName = player->UserName.ToString();
						ImGui::Text(playerName.c_str());

						std::string tpToPlayerStr = "Tp to " + playerName + "##tp_to_" + std::to_string(i);
						std::string tpPlayerToYouStr = "Tp " + playerName + " to You##tp_" + std::to_string(i) + "_to_you";
						if (ImGui::Button(tpToPlayerStr.c_str()))
						{
							ue->myCharacter->K2_TeleportTo(player->K2_GetActorLocation(), ue->myCharacter->K2_GetActorRotation());
						}
						if (ImGui::Button(tpPlayerToYouStr.c_str()))
						{
							player->K2_TeleportTo(ue->myCharacter->K2_GetActorLocation(), player->K2_GetActorRotation());
						}

						ImGui::Dummy({ 0.f, 10.f });
					}

					if (CFG.b_tpAllPlayersToYou)
					{
						CFG.b_tpAllPlayersToYou = false;
					}
				}
			}

			if (CFG.tabIndex == 3) // Settings
			{
				if (ImGui::ColorEdit3("Player visible Color", CFG.f_boxVisiblePlayerColor, ImGuiColorEditFlags_NoDragDrop))
				{
					CFG.boxVisiblePlayerColor = ImColor(CFG.f_boxVisiblePlayerColor[0], CFG.f_boxVisiblePlayerColor[1], CFG.f_boxVisiblePlayerColor[2]);
				}

				if (ImGui::ColorEdit3("Player not visible Color", CFG.f_boxNotVisiblePlayerColor, ImGuiColorEditFlags_NoDragDrop))
				{
					CFG.boxNotVisiblePlayerColor = ImColor(CFG.f_boxNotVisiblePlayerColor[0], CFG.f_boxNotVisiblePlayerColor[1], CFG.f_boxNotVisiblePlayerColor[2]);
				}
				ImGui::Dummy(ImVec2{ 0.f, 10.f });

				if (ImGui::ColorEdit3("Husky visible Color", CFG.f_boxVisibleHuskyColor, ImGuiColorEditFlags_NoDragDrop))
				{
					CFG.boxVisibleHuskyColor = ImColor(CFG.f_boxVisibleHuskyColor[0], CFG.f_boxVisibleHuskyColor[1], CFG.f_boxVisibleHuskyColor[2]);
				}
				if (ImGui::ColorEdit3("Husky not visible Color", CFG.f_boxNotVisibleHuskyColor, ImGuiColorEditFlags_NoDragDrop))
				{
					CFG.boxNotVisibleHuskyColor = ImColor(CFG.f_boxNotVisibleHuskyColor[0], CFG.f_boxNotVisibleHuskyColor[1], CFG.f_boxNotVisibleHuskyColor[2]);
				}
				ImGui::Dummy(ImVec2{ 0.f, 10.f });

				if (ImGui::ColorEdit3("Skeleton Color", CFG.f_skeletonColor, ImGuiColorEditFlags_NoDragDrop))
				{
					CFG.skeletonColor = ImColor(CFG.f_skeletonColor[0], CFG.f_skeletonColor[1], CFG.f_skeletonColor[2]);
				}
				ImGui::Dummy(ImVec2{ 0.f, 10.f });

				if (ImGui::ColorEdit3("Soda machine Color", CFG.f_sodaMachineColor, ImGuiColorEditFlags_NoDragDrop))
				{
					CFG.sodaMachineColor = ImColor(CFG.f_sodaMachineColor[0], CFG.f_sodaMachineColor[1], CFG.f_sodaMachineColor[2]);
				}
			}

			if (CFG.tabIndex == 8) // About
			{
				ImGui::Text("Made by TheGeogeo");
				ImGui::Dummy(ImVec2{ 0.f, 10.f });
				ImGui::Text("This cheat is free, if you buy it... well...");
			}

			if (CFG.tabIndex == 9) // Debug
			{
				std::string gworldDebugStr = "GWorld: " + utilFunction::AddressForImGui((uintptr_t)ue->GWorld);
				ImGui::Text(gworldDebugStr.c_str());

				std::string worldDebugStr = "World: " + utilFunction::AddressForImGui((uintptr_t)ue->World);
				ImGui::Text(worldDebugStr.c_str());

				std::string levelDebugStr = "Level: " + utilFunction::AddressForImGui((uintptr_t)ue->level);
				ImGui::Text(levelDebugStr.c_str());

				std::string gameStateDebugStr = "GameState: " + utilFunction::AddressForImGui((uintptr_t)ue->GameState);
				ImGui::Text(gameStateDebugStr.c_str());

				std::string gameInstanceDebugStr = "gameInstance: " + utilFunction::AddressForImGui((uintptr_t)ue->gameInstance);
				ImGui::Text(gameInstanceDebugStr.c_str());

				std::string localPlayerDebugStr = "LocalPlayer: " + utilFunction::AddressForImGui((uintptr_t)ue->LocalPlayer);
				ImGui::Text(localPlayerDebugStr.c_str());

				std::string playerControllerDebugStr = "PlayerController: " + utilFunction::AddressForImGui((uintptr_t)ue->PlayerController);
				ImGui::Text(playerControllerDebugStr.c_str());

				std::string playerCameraManagerDebugStr = "PlayerCameraManager: " + utilFunction::AddressForImGui((uintptr_t)ue->PlayerCameraManager);
				ImGui::Text(playerCameraManagerDebugStr.c_str());

				std::string myPawnDebugStr = "myPawn: " + utilFunction::AddressForImGui((uintptr_t)ue->myPawn);
				ImGui::Text(myPawnDebugStr.c_str());

				std::string myCharacterDebugStr = "myCharacter: " + utilFunction::AddressForImGui((uintptr_t)ue->myCharacter);
				ImGui::Text(myCharacterDebugStr.c_str());

				ImGui::Text("--------------------------------");

				SDK::FRotator cameraAngleDebug = ue->PlayerCameraManager->GetCameraRotation();
				std::string cameraAngleDebugStr = "Camera angles: " + std::to_string(cameraAngleDebug.Pitch) + " " + std::to_string(cameraAngleDebug.Roll) + " " + std::to_string(cameraAngleDebug.Yaw);
				ImGui::Text(cameraAngleDebugStr.c_str());

				SDK::FVector cameraLocationDebug = ue->PlayerCameraManager->GetCameraLocation();
				std::string cameraLocationDebugStr = "Camera Location: " + std::to_string(cameraLocationDebug.X) + " " + std::to_string(cameraLocationDebug.Y) + " " + std::to_string(cameraLocationDebug.Z);
				ImGui::Text(cameraLocationDebugStr.c_str());

				ImGui::Text("--------------------------------");

				ImGui::Checkbox("Noclip info", &CFG.debugNoclip);
				ImGui::Checkbox("All actors", &CFG.debugAllActors);
				if (CFG.debugAllActors)
				{
					ImGui::Checkbox("Only Characters", &CFG.debugAllActorsOnlyCharacter);
					if (CFG.debugAllActorsOnlyCharacter)
					{
						ImGui::Checkbox("Only Characters Bones", &CFG.debugAllActorsOnlyCharacterBones);
					}
				}
			}

			ImGui::End();
		}
	}
}