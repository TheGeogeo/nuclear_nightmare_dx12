#include "includes.h"

namespace cheat
{
	void GettingActors()
	{
		if (nullptr == ue->World) return;
		if (CFG.debugNoclip)
		{
			ue->ToggleNoClip(false, true);
		}

		SDK::UClass* staticFirstPersonCharacter = SDK::ABP_FirstPersonCharacter_C::StaticClass();
		SDK::UClass* staticGenerator = SDK::ABP_GeneratorGameplayNew_C::StaticClass();
		SDK::UClass* staticLaserBomb = SDK::Abp_laser_C::StaticClass();
		SDK::UClass* staticItem = SDK::AItemActor::StaticClass();
		SDK::UClass* staticHusky = SDK::AHusky_AI_C::StaticClass();
		SDK::UClass* staticDoorBase = SDK::ABP_Door_Base_C::StaticClass();
		SDK::UClass* staticSky = SDK::AUltra_Dynamic_Sky_C::StaticClass();
		SDK::UClass* staticWeather = SDK::AUltra_Dynamic_Weather_C::StaticClass();
		SDK::UClass* staticSodaMachine = SDK::ASodaMachineBP_C::StaticClass();
		SDK::UClass* staticComputerRadio = SDK::ABP_Radio_C::StaticClass();

		for (SDK::ULevel* level : ue->World->Levels)
		{
			for (SDK::AActor* actor : level->Actors)
			{
				if (!actor)
					continue;

				if (CFG.debugAllActors)
				{
					if (CFG.debugAllActorsOnlyCharacter && actor->IsA(SDK::ACharacter::StaticClass()))
					{
						CFG.v_allActorsDebug.push_back(actor);
					}
					else if (!CFG.debugAllActorsOnlyCharacter)
					{
						CFG.v_allActorsDebug.push_back(actor);
					}
				}

				if (staticFirstPersonCharacter && actor->IsA(staticFirstPersonCharacter))
				{
					CFG.v_allFirstPersonCharacters.push_back(actor);
				}
				else if (staticGenerator && actor->IsA(staticGenerator))
				{
					CFG.v_allGenerators.push_back(actor);
				}
				else if (staticLaserBomb && actor->IsA(staticLaserBomb))
				{
					CFG.v_allLasersBombs.push_back(actor);
				}
				else if (staticItem && actor->IsA(staticItem))
				{
					CFG.v_allItems.push_back(actor);
				}
				else if (staticHusky && actor->IsA(staticHusky))
				{
					CFG.v_allHuskys.push_back(actor);
				}
				else if (staticDoorBase && actor->IsA(staticDoorBase))
				{
					CFG.v_allDoorBases.push_back(actor);
				}
				else if (staticSky && actor->IsA(staticSky))
				{
					CFG.v_allSkys.push_back(actor);
				}
				else if (staticWeather && actor->IsA(staticWeather))
				{
					CFG.v_allWeathers.push_back(actor);
				}
				else if (staticSodaMachine && actor->IsA(staticSodaMachine))
				{
					CFG.v_allSodaMachines.push_back(actor);
				}
				else if (staticComputerRadio && actor->IsA(staticComputerRadio))
				{
					CFG.v_allRadioComputers.push_back(actor);
				}
			}
		}
	}

	void ClearAllArray()
	{
		CFG.v_allActorsDebug.clear();
		CFG.v_allFirstPersonCharacters.clear();
		CFG.v_allGenerators.clear();
		CFG.v_allLasersBombs.clear();
		CFG.v_allItems.clear();
		CFG.v_allHuskys.clear();
		CFG.v_allDoorBases.clear();
		CFG.v_allSkys.clear();
		CFG.v_allWeathers.clear();
		CFG.v_allSodaMachines.clear();
		CFG.v_allRadioComputers.clear();
	}

	void Init()
	{
		// Test for few things
		if (CFG.debugAllActors && offset::G_WORLD && ue->World)
		{
			for (int i = 0; i < CFG.v_allActorsDebug.size(); i++)
			{
				SDK::AActor* actor = reinterpret_cast<SDK::AActor*>(CFG.v_allActorsDebug[i]);
				if (!actor) continue;
				actor->IsA(SDK::AActor::StaticClass());

				SDK::FVector2D relativePosToScreen = ue->w2s(actor->K2_GetActorLocation());

				if (!CFG.debugAllActorsOnlyCharacterBones)
				{
					overlay::DrawString({ (float)relativePosToScreen.X, (float)relativePosToScreen.Y }, actor->GetName(), ImColor{ 255,255,255 });
				}

				if (CFG.debugAllActorsOnlyCharacter && CFG.debugAllActorsOnlyCharacterBones)
				{
					SDK::ACharacter* characterActor = reinterpret_cast<SDK::ACharacter*>(actor);
					std::string nameActorStr = "Actor name: " + actor->GetName();
					ImGui::Text(nameActorStr.c_str());
					for (int j = 0; j < characterActor->Mesh->GetNumBones(); j++)
					{
						SDK::FName boneSocketName = characterActor->Mesh->GetBoneName(j);
						SDK::FVector boneSocketLocation = characterActor->Mesh->GetSocketLocation(boneSocketName);
						SDK::FVector2D boneSocketLocation2D = ue->w2s(boneSocketLocation);

						overlay::DrawString({ (float)boneSocketLocation2D.X, (float)boneSocketLocation2D.Y }, boneSocketName.ToString(), ImColor{ 255, 255, 255 });
						ImGui::Text(boneSocketName.ToString().c_str());
					}
					ImGui::Text("--------------------------------");
				}
			}
		}

		if (ue->World && ue->myPawn && ue->level && ue->PlayerController && ue->myCharacter && ue->GameState)
		{
			for (int i = 0; i < CFG.v_allFirstPersonCharacters.size(); i++)
			{
				SDK::ABP_FirstPersonCharacter_C* player = reinterpret_cast<SDK::ABP_FirstPersonCharacter_C*>(CFG.v_allFirstPersonCharacters[i]);
				if (!player) continue;
				if ((uintptr_t)player == (uintptr_t)ue->myCharacter) continue;
				if (0.f > ue->myCharacter->Health) continue;

				bool isVisible = ue->PlayerController->LineOfSightTo(player, { 0,0,0 }, false);

				SDK::FVector BottomCharacter = player->Mesh->GetSocketLocation(player->Mesh->GetBoneName(0));
				SDK::FVector TopCharacter = SDK::FVector();
				float distanceWithActor = ue->Distance(BottomCharacter);

				if (CFG.b_espGlobal && CFG.f_espMaxRender > distanceWithActor)
				{
					TopCharacter = player->Mesh->GetSocketLocation(ue->getBoneByName(player, bones::head));
					TopCharacter.Z = TopCharacter.Z + 25.0;

					//---------------------- pawn to screen
					//Bone Calculations
					const SDK::FVector BoneLoc1 = TopCharacter;
					const SDK::FVector BoneLoc2 = player->Mesh->GetSocketLocation(ue->getBoneByName(player, bones::leftFeet));
					const SDK::FVector BoneLoc3 = BottomCharacter;

					SDK::FVector WorldHead = BoneLoc1;
					SDK::FVector WorldBottom = BoneLoc2;
					SDK::FVector WorldRoot = BoneLoc3;
					WorldRoot.Z = WorldRoot.Z - 10.0;

					SDK::FVector2D head = ue->w2s(WorldHead);
					SDK::FVector2D Top = ue->w2s({ BoneLoc1.X, BoneLoc1.Y, BoneLoc1.Z + 20 });
					SDK::FVector2D Bottom = ue->w2s(WorldBottom);
					SDK::FVector2D Root = ue->w2s(WorldRoot);

					//Calculations for ESP Positioning

					float BoxHeight = (float)(Root.Y - head.Y);
					float BoxWidth = BoxHeight * 0.60f;

					float BottomLeftX = (float)head.X - BoxWidth / 2;
					float BottomLeftY = (float)Root.Y;

					float BottomRightX = (float)head.X + BoxWidth / 2;
					float BottomRightY = (float)Root.Y;

					float TopRightX = (float)head.X + BoxWidth / 2;
					float TopRightY = (float)head.Y;

					float TopLeftX = (float)head.X - BoxWidth / 2;
					float TopLeftY = (float)head.Y;
					//-------------------------

					if (CFG.b_espPlayers)
					{
						if (CFG.b_espPlayerRect)
						{
							if (head != SDK::FVector2D() && Root != SDK::FVector2D() && (!ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, head) || !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, Root)))
							{
								ImColor colorBoxESP = isVisible ? CFG.boxVisiblePlayerColor : CFG.boxNotVisiblePlayerColor;
								overlay::RectNotFilled({ BottomLeftX, BottomLeftY }, { BottomRightX, BottomRightY }, { TopRightX, TopRightY }, { TopLeftX, TopLeftY }, colorBoxESP, 1.f);
							}
						}

						if (CFG.b_espPlayerName)
						{
							if (head != SDK::FVector2D() && Root != SDK::FVector2D() && (!ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, head) || !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, Root)))
							{
								overlay::DrawString({ TopLeftX, TopLeftY - 15.f }, player->UserName.ToString(), ImColor{ 255, 255, 255 });
							}
						}

						if (CFG.b_espPlayerInfected && ue->myCharacter->StartedInfection_)
						{
							if (head != SDK::FVector2D() && Root != SDK::FVector2D() && (!ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, head) || !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, Root)))
							{
								overlay::DrawString({ BottomLeftX, BottomLeftY + 15.f }, "Infected", ImColor{ 255, 0, 0 });
							}
						}

						if (CFG.b_espPlayerSkeleton)
						{
							if (head != SDK::FVector2D() && Root != SDK::FVector2D() && (!ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, head) || !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, Root)))
							{
								std::vector<BonePair> bpair;

								bpair.push_back({ bones::head, bones::upBody });

								bpair.push_back({ bones::upBody, bones::leftShoulder });
								bpair.push_back({ bones::leftShoulder, bones::leftElbow });
								bpair.push_back({ bones::leftElbow, bones::leftHand });

								bpair.push_back({ bones::upBody, bones::rightShoulder });
								bpair.push_back({ bones::rightShoulder, bones::rightElbow });
								bpair.push_back({ bones::rightElbow, bones::rightHand });

								bpair.push_back({ bones::upBody, bones::pelvis });

								bpair.push_back({ bones::pelvis, bones::leftHip });
								bpair.push_back({ bones::leftHip, bones::leftKnee });
								bpair.push_back({ bones::leftKnee, bones::leftFeet });

								bpair.push_back({ bones::pelvis, bones::rightHip });
								bpair.push_back({ bones::rightHip, bones::rightKnee });
								bpair.push_back({ bones::rightKnee, bones::rightFeet });

								for (int y = 0; y < bpair.size(); y++)
								{
									SDK::FVector2D b1 = ue->w2s(player->Mesh->GetSocketLocation(ue->getBoneByName(player, bpair[y].bone1)));
									SDK::FVector2D b2 = ue->w2s(player->Mesh->GetSocketLocation(ue->getBoneByName(player, bpair[y].bone2)));
									if (b1 == SDK::FVector2D()) continue;
									if (b2 == SDK::FVector2D()) continue;
									overlay::DrawLine({ (float)b1.X,(float)b1.Y }, { (float)b2.X,(float)b2.Y }, CFG.skeletonColor, 1);
								}
							}
						}
					}
				}
			}

			for (int i = 0; i < CFG.v_allHuskys.size(); i++)
			{
				SDK::AHusky_AI_C* husky = reinterpret_cast<SDK::AHusky_AI_C*>(CFG.v_allHuskys[i]);
				if (!husky) continue;
				if (husky->Dead_) continue;

				bool isVisible = ue->PlayerController->LineOfSightTo(husky, { 0,0,0 }, false);

				SDK::FVector BottomCharacter = husky->Mesh->GetSocketLocation(husky->Mesh->GetBoneName(0));
				SDK::FVector TopCharacter = SDK::FVector();
				float distanceWithActor = ue->Distance(BottomCharacter);

				if (CFG.b_espGlobal && CFG.f_espMaxRender > distanceWithActor)
				{
					TopCharacter = husky->Mesh->GetSocketLocation(ue->getBoneByName(husky, huskyBones::head));
					TopCharacter.Z = TopCharacter.Z + 25.0;

					//---------------------- pawn to screen
					//Bone Calculations
					const SDK::FVector BoneLoc1 = TopCharacter;
					const SDK::FVector BoneLoc2 = husky->Mesh->GetSocketLocation(ue->getBoneByName(husky, huskyBones::tail5));
					const SDK::FVector BoneLoc3 = BottomCharacter;

					SDK::FVector WorldHead = BoneLoc1;
					SDK::FVector WorldBottom = BoneLoc2;
					SDK::FVector WorldRoot = BoneLoc3;
					WorldRoot.Z = WorldRoot.Z - 10.0;

					SDK::FVector2D head = ue->w2s(WorldHead);
					SDK::FVector2D Top = ue->w2s({ BoneLoc1.X, BoneLoc1.Y, BoneLoc1.Z + 20 });
					SDK::FVector2D Bottom = ue->w2s(WorldBottom);
					SDK::FVector2D Root = ue->w2s(WorldRoot);

					//Calculations for ESP Positioning

					float BoxHeight = (float)(Root.Y - head.Y);
					float BoxWidth = BoxHeight * 0.60f;

					float BottomLeftX = (float)head.X - BoxWidth / 2;
					float BottomLeftY = (float)Root.Y;

					float BottomRightX = (float)head.X + BoxWidth / 2;
					float BottomRightY = (float)Root.Y;

					float TopRightX = (float)head.X + BoxWidth / 2;
					float TopRightY = (float)head.Y;

					float TopLeftX = (float)head.X - BoxWidth / 2;
					float TopLeftY = (float)head.Y;
					//-------------------------

					if (CFG.b_espMobs)
					{
						if (CFG.b_espHuskyRect)
						{
							if (head != SDK::FVector2D() && Root != SDK::FVector2D() && (!ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, head) || !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, Root)))
							{
								ImColor colorBoxESP = isVisible ? CFG.boxVisiblePlayerColor : CFG.boxNotVisiblePlayerColor;
								overlay::RectNotFilled({ BottomLeftX, BottomLeftY }, { BottomRightX, BottomRightY }, { TopRightX, TopRightY }, { TopLeftX, TopLeftY }, colorBoxESP, 1.f);
							}
						}

						if (CFG.b_espHuskySkeleton)
						{
							if (head != SDK::FVector2D() && Root != SDK::FVector2D() && (!ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, head) || !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, Root)))
							{
								ue->DrawSkeletonFrom(husky->Mesh, ue->getBoneByName(husky, bones::head), CFG.huskyBonnesToSkip);
							}
						}
					}
				}
			}

			for (int i = 0; i < CFG.v_allGenerators.size(); i++)
			{
				SDK::ABP_GeneratorGameplayNew_C* generator = reinterpret_cast<SDK::ABP_GeneratorGameplayNew_C*>(CFG.v_allGenerators[i]);
				if (!generator) continue;

				bool isVisible = ue->PlayerController->LineOfSightTo(generator, { 0,0,0 }, false);

				SDK::FVector locationActor = generator->K2_GetActorLocation();
				float distanceWithActor = ue->Distance(locationActor);

				if (CFG.b_espGlobal && CFG.f_espMaxRender > distanceWithActor)
				{
					if (CFG.b_espGenerator)
					{
						SDK::FVector2D actorLocation2D = ue->w2s(locationActor);
						if (actorLocation2D != SDK::FVector2D() && !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, actorLocation2D))
						{
							overlay::DrawCircle({ (float)actorLocation2D.X, (float)actorLocation2D.Y }, 5.f, ImColor{ 245, 252, 15 }, 1.f);
							overlay::DrawString({ (float)actorLocation2D.X, (float)actorLocation2D.Y + 7.f }, std::string("Generator"), ImColor{ 255, 255, 255 });
						}
					}
				}
			}

			for (int i = 0; i < CFG.v_allRadioComputers.size(); i++)
			{
				SDK::ABP_Radio_C* computerRadio = reinterpret_cast<SDK::ABP_Radio_C*>(CFG.v_allRadioComputers[i]);
				if (!computerRadio) continue;

				//bool isVisible = ue->PlayerController->LineOfSightTo(computerRadio, { 0,0,0 }, false);

				SDK::FVector locationActor = computerRadio->K2_GetActorLocation();
				float distanceWithActor = ue->Distance(locationActor);

				if (CFG.b_espGlobal && CFG.f_espMaxRender > distanceWithActor)
				{
					if (CFG.b_espComputerRadio)
					{
						SDK::FVector2D actorLocation2D = ue->w2s(locationActor);
						if (actorLocation2D != SDK::FVector2D() && !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, actorLocation2D))
						{
							overlay::DrawHexagon({ (float)actorLocation2D.X, (float)actorLocation2D.Y }, 5.f, ImColor{ 245, 252, 15 }, 1.f);
							overlay::DrawString({ (float)actorLocation2D.X, (float)actorLocation2D.Y + 7.f }, std::string("Computer"), ImColor{ 255, 255, 255 });
						}
					}
				}
			}

			for (int i = 0; i < CFG.v_allLasersBombs.size(); i++)
			{
				SDK::Abp_laser_C* laserBomb = reinterpret_cast<SDK::Abp_laser_C*>(CFG.v_allLasersBombs[i]);
				if (!laserBomb) continue;

				bool isVisible = ue->PlayerController->LineOfSightTo(laserBomb, { 0,0,0 }, false);

				SDK::FVector locationActor = laserBomb->K2_GetActorLocation();
				float distanceWithActor = ue->Distance(locationActor);

				if (CFG.b_espGlobal && CFG.f_espMaxRender > distanceWithActor)
				{
					if (CFG.b_espLaserBomb)
					{
						SDK::FVector2D actorLocation2D = ue->w2s(locationActor);
						if (actorLocation2D != SDK::FVector2D() && !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, actorLocation2D))
						{
							overlay::DrawSquare({ (float)actorLocation2D.X, (float)actorLocation2D.Y }, 8.f, ImColor{ 245, 252, 15 }, 1.f);
							overlay::DrawString({ (float)actorLocation2D.X, (float)actorLocation2D.Y + 7.f }, std::string("Laser bomb"), ImColor{ 255, 255, 255 });
						}
					}
				}
			}

			std::vector<SDK::AItemActor*> itemsForRing;
			itemsForRing.reserve(CFG.v_allItems.size());

			for (int i = 0; i < CFG.v_allItems.size(); i++)
			{
				SDK::AItemActor* item = reinterpret_cast<SDK::AItemActor*>(CFG.v_allItems[i]);
				if (!item) continue;

				bool isVisible = ue->PlayerController->LineOfSightTo(item, { 0,0,0 }, false);

				SDK::FVector locationActor = item->K2_GetActorLocation();
				float distanceWithActor = ue->Distance(locationActor);

				if (CFG.b_espGlobal && CFG.f_espMaxRender > distanceWithActor && !item->PickedUp)
				{
					if (CFG.b_espItems)
					{
						SDK::FVector2D actorLocation2D = ue->w2s(locationActor);
						if (actorLocation2D != SDK::FVector2D() && !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, actorLocation2D))
						{
							overlay::DrawTriangle({ (float)actorLocation2D.X, (float)actorLocation2D.Y }, 5.f, ImColor{ 245, 252, 15 }, 1.f);
							overlay::DrawString({ (float)actorLocation2D.X, (float)actorLocation2D.Y + 7.f }, item->ItemName.ToString(), ImColor{ 255, 255, 255 });
						}
					}
				}

				if (CFG.b_miscTpItem && menu::isOpen)
				{
					ImGui::Begin("Tp specific item");

					std::string itemBtn = item->ItemName.ToString() + " tp item##tp_item_" + std::to_string(i);
					if (ImGui::Button(itemBtn.c_str()))
					{
						SDK::FVector actorLocation = ue->myCharacter->K2_GetActorLocation();
						SDK::FRotator actorRotator = ue->myCharacter->K2_GetActorRotation();
						actorLocation.Z = actorLocation.Z + 50.0;

						const SDK::FVector posToTp = ue->AngleToFVector(actorLocation, actorRotator, 150.f);

						item->K2_TeleportTo(posToTp, item->K2_GetActorRotation());
					}
					ImGui::End();
				}

				if (CFG.b_miscTpItemsAround)
				{
					if (!item->PickedUp)
					{
						itemsForRing.emplace_back(item);
					}
				}
			}

			// tp all items in circle (rotation fluide)
			if (CFG.b_miscTpItemsAround && !itemsForRing.empty())
			{
				// -- Delta time based on steady_clock (engine-independent)
				auto now = std::chrono::steady_clock::now();
				if (CFG.g_lastTick.time_since_epoch().count() == 0) CFG.g_lastTick = now;
				float dt = std::chrono::duration<float>(now - CFG.g_lastTick).count();
				CFG.g_lastTick = now;
				// Clamp to avoid jumps during pauses / alt-tab
				dt = std::clamp(dt, 0.0f, 0.05f);

				// Angular speed (rad/s). Put it in CFG if you want to tweak it on the fly.
				float angularSpeed = (CFG.f_itemRingAngularSpeedRad > 0.0f) ? CFG.f_itemRingAngularSpeedRad : 1.2f;

				// Increment the global rotation angle
				CFG.g_itemsRingAngle += angularSpeed * dt;
				const float twoPi = 2.0f * (float)M_PI;
				if (CFG.g_itemsRingAngle >= twoPi) CFG.g_itemsRingAngle -= twoPi;

				// Adaptive circle parameters
				const float minRadius = 120.0f;
				const float growthPerSqrt = 85.0f;
				const float maxRadius = 1600.0f;
				const float zLift = 30.0f;

				// Sort to ensure a stable order (avoids micro-jumps if the source order changes)
				std::sort(itemsForRing.begin(), itemsForRing.end());

				const size_t N = itemsForRing.size();

				float radius = minRadius + growthPerSqrt * std::sqrt(static_cast<float>(N));
				radius = std::clamp(radius, minRadius, maxRadius);

				SDK::FVector center = ue->myCharacter->K2_GetActorLocation();
				center.Z += zLift;

				SDK::FRotator rot = ue->myCharacter->K2_GetActorRotation();

				// Base angle = player direction + global rotation angle (spin)
				const float baseAngle = ue->DegToRad(rot.Yaw) + CFG.g_itemsRingAngle;

				// Uniform distribution
				const float step = twoPi / static_cast<float>(N);

				for (size_t idx = 0; idx < N; ++idx)
				{
					SDK::AItemActor* item = itemsForRing[idx];
					if (!item || item->PickedUp) continue;

					const float theta = baseAngle + step * static_cast<float>(idx);

					SDK::FVector target = center;
					target.X += std::cos(theta) * radius;
					target.Y += std::sin(theta) * radius;

					item->K2_TeleportTo(target, item->K2_GetActorRotation());
				}
			}

			for (int i = 0; i < CFG.v_allSodaMachines.size(); i++)
			{
				SDK::ASodaMachineBP_C* sodaMachine = reinterpret_cast<SDK::ASodaMachineBP_C*>(CFG.v_allSodaMachines[i]);
				if (!sodaMachine) continue;

				//bool isVisible = ue->PlayerController->LineOfSightTo(sodaMachine, { 0,0,0 }, false);

				SDK::FVector locationActor = sodaMachine->K2_GetActorLocation();
				float distanceWithActor = ue->Distance(locationActor);

				if (CFG.b_espGlobal && CFG.b_espSodaMachine && CFG.f_espMaxRender > distanceWithActor)
				{
					SDK::FVector2D location2D = ue->w2s(locationActor);
					if (location2D != SDK::FVector2D() && !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, location2D))
					{
						//ImColor colorBoxESP = isVisible ? CFG.boxVisibleColor : CFG.boxNotVisibleColor;
						overlay::Draw3dBox(sodaMachine, CFG.sodaMachineColor);
					}
				}
			}

			for (int i = 0; i < CFG.v_allDoorBases.size(); i++)
			{
				SDK::ABP_Door_Base_C* door = reinterpret_cast<SDK::ABP_Door_Base_C*>(CFG.v_allDoorBases[i]);
				if (!door) continue;

				if (CFG.b_miscUnlockAllDoor)
				{
					door->Locked_ = 0;
				}
			}

			for (int i = 0; i < CFG.v_allSkys.size(); i++)
			{
				SDK::AUltra_Dynamic_Sky_C* sky = reinterpret_cast<SDK::AUltra_Dynamic_Sky_C*>(CFG.v_allSkys[i]);
				if (!sky) continue;
			}

			for (int i = 0; i < CFG.v_allWeathers.size(); i++)
			{
				SDK::AUltra_Dynamic_Weather_C* weather = reinterpret_cast<SDK::AUltra_Dynamic_Weather_C*>(CFG.v_allWeathers[i]);
				if (!weather) continue;

				if (CFG.b_miscRemoveFog && weather->Fog != 0.0)
				{
					weather->Fog = 0.0;
				}
			}

			if (ue->myCharacter->Health > 0.f)
			{
				if (CFG.b_miscInfAmmo)
				{
					if (ue->myCharacter->RevolverPTR) {
						ue->myCharacter->RevolverPTR->UpdateAmmoAmount(100.0);
					}
					if (ue->myCharacter->ItemMP5) {
						ue->myCharacter->ItemMP5->UpdateAmmoAmount(100.0);
					}
					if (ue->myCharacter->ShotgunPTR) {
						ue->myCharacter->ShotgunPTR->UpdateAmmoAmount(100.0);
					}
					if (ue->myCharacter->Flare_Gun_Item) {
						ue->myCharacter->Flare_Gun_Item->Ammo = 10.0;
					}
					if (ue->myCharacter->SniperItem) {
						ue->myCharacter->SniperItem->UpdateAmmoAmount(100.0);
					}
					if (ue->myCharacter->FlameThrowerItem) {
						ue->myCharacter->FlameThrowerItem->UpdateItemFuel(100.0);
					}
				}

				if (CFG.b_miscInfInvAmmo)
				{
					if (ue->myCharacter->Buckshot < 300) {
						ue->myCharacter->AddBuckshotAll(300);
						ue->myCharacter->AddBuckshot();
						ue->myCharacter->AddBuckshotServer();
					}
					if (ue->myCharacter->NinemmCaliber < 300) {
						ue->myCharacter->Add9mmsHOTSALL(300);
						ue->myCharacter->add9mm();
						ue->myCharacter->Add9mmShots();
					}
					if (ue->myCharacter->SevenmmCaliber < 300) {
						ue->myCharacter->AddSniperShotsAll(300);
						ue->myCharacter->AddSniperShots();
					}
					if (ue->myCharacter->Three8Caliber < 300) {
						ue->myCharacter->AddReolverShotsAll(300);
						ue->myCharacter->AddReolverShots();
					}

					/*if (ImGui::Button("drop ammo"))
					{
						SDK::FVector actorLocation = ue->myCharacter->K2_GetActorLocation();
						SDK::FRotator actorRotator = ue->myCharacter->K2_GetActorRotation();
						actorLocation.Z = actorLocation.Z + 50.0;

						//const SDK::FVector posToSpawn = ue->AngleToFVector(actorLocation, actorRotator, 150.f);
						//ue->myCharacter->DropAmmoType(posToSpawn, SDK::EAmmoType::NewEnumerator0);
						//ue->myCharacter->DropAmmoType(posToSpawn, SDK::EAmmoType::NewEnumerator1);
						//ue->myCharacter->DropAmmoType(posToSpawn, SDK::EAmmoType::NewEnumerator2);
						//ue->myCharacter->DropAmmoType(posToSpawn, SDK::EAmmoType::NewEnumerator3);
					}*/
				}

				if (CFG.b_miscInfFlashlight)
				{
					if (ue->myCharacter->EquipedFlashlight) {
						ue->myCharacter->EquipedFlashlight->UpdateItemFuel(100.0);
					}
				}

				if (CFG.b_miscInfFuelCan)
				{
					if (ue->myCharacter->GasCainster) {
						ue->myCharacter->GasCainster->UpdateItemFuel(100.0);
					}
				}

				if (CFG.b_miscInfHealth && ue->myCharacter->Health < 1000.0)
				{
					ue->myCharacter->UpdateHealth(1000.0);
				}

				if (CFG.b_miscNoFallDamage)
				{
					ue->myCharacter->CanTakeFallDamage = 0;
				}

				if (CFG.b_miscNoTemperatureDecrease)
				{
					ue->myCharacter->CanTakeTemperature = 0;
					ue->myCharacter->Temperature = 0.0;
				}

				if (CFG.b_miscInfJump)
				{
					ue->myCharacter->JumpMaxCount = INT32_MAX;
				}

				if (CFG.b_miscNeverInfected && ue->myCharacter->StartedInfection_)
				{
					ue->myCharacter->UpdateInfectionStatus(false, ue->myCharacter);
					ue->myCharacter->UpdateInfectionStatServer(false, ue->myCharacter);
				}

				if (CFG.b_miscNoClip)
				{
					ue->ToggleNoClip(CFG.b_miscNoClip);
					ue->NoclipTick();
				}
			}

			ue->SetLightShadow(CFG.b_miscFullbright);
		}
	}
}