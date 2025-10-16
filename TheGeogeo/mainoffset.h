#pragma once

#define M_PI 3.14159265358979323846

struct BonePair {
	std::string bone1;
	std::string bone2;
};

struct vec3
{
	float x, y, z;
};

class bones
{
public:
	static inline const std::string bottom = "Root";
	static inline const std::string pelvis = "Pelvis";
	static inline const std::string middleBody = "spine_02";
	static inline const std::string upBody = "spine_03";
	static inline const std::string leftElbow = "lowerarm_l";
	static inline const std::string leftHand = "hand_l";
	static inline const std::string leftShoulder = "upperarm_l";
	static inline const std::string rightShoulder = "upperarm_r";
	static inline const std::string rightElbow = "lowerarm_r";
	static inline const std::string rightHand = "hand_r";
	static inline const std::string neck = "neck_01";
	static inline const std::string head = "Head";
	static inline const std::string leftHip = "thigh_l";
	static inline const std::string leftKnee = "calf_l";
	static inline const std::string leftFeet = "ball_l";
	static inline const std::string rightHip = "thigh_r";
	static inline const std::string rightKnee = "calf_r";
	static inline const std::string rightFeet = "ball_r";
};

class huskyBones
{
public:
	static inline const std::string bottom = "Root";
	static inline const std::string thighBackRight = "thigh_b_R";
	static inline const std::string legBackRight = "leg_b_R";
	static inline const std::string shinBackRight = "shin_b_R";
	static inline const std::string footBackRight = "foot_b_R";
	static inline const std::string thighBackLeft = "thigh_b_L";
	static inline const std::string legBackLeft = "leg_b_L";
	static inline const std::string shinBackLeft = "shin_b_L";
	static inline const std::string footBackLeft = "foot_b_L";
	static inline const std::string thighFrontRight = "thigh_f_R";
	static inline const std::string legFrontRight = "leg_f_R";
	static inline const std::string shinFrontRight = "shin_f_R";
	static inline const std::string footFrontRight = "foot_f_R";
	static inline const std::string thighFrontLeft = "thigh_f_L";
	static inline const std::string legFrontLeft = "leg_f_L";
	static inline const std::string shinFrontLeft = "shin_f_L";
	static inline const std::string footFrontLeft = "foot_f_L";
	static inline const std::string head = "Head";
	static inline const std::string neck = "neck";
	static inline const std::string spine5 = "Spine_03";
	static inline const std::string spine4 = "Spine_04";
	static inline const std::string spine3 = "Spine_05";
	static inline const std::string tail1 = "Tail_01";
	static inline const std::string tail2 = "Tail_02";
	static inline const std::string tail3 = "Tail_03";
	static inline const std::string tail4 = "Tail_04";
	static inline const std::string tail5 = "Tail_05";
};

namespace offset
{
	const inline uint64_t G_WORLD = SDK::Offsets::GWorld;
	const inline uint64_t G_NAME = SDK::Offsets::GNames;
	const inline uint64_t G_OBJECT_ARRAY = SDK::Offsets::GObjects;
}

class UnrealEngine
{
public:
	static inline SDK::UEngine* Engine = nullptr;
	static inline SDK::UWorld** GWorld = nullptr;
	static inline SDK::UWorld* World = nullptr;
	static inline SDK::ULevel* level = nullptr;
	static inline SDK::ABP_FirstPersonGameMode_C* gameMode = nullptr;
	static inline SDK::ABP_FirstPersonGameState_C* GameState = nullptr;
	static inline SDK::UGameInstance* gameInstance = nullptr;
	static inline SDK::ULocalPlayer* LocalPlayer = nullptr;
	static inline SDK::ABP_NNFirstPersonPlayerController_C* PlayerController = nullptr;
	static inline SDK::APlayerCameraManager* PlayerCameraManager = nullptr;
	static inline SDK::ABP_FirstPersonCharacter_C* myCharacter = nullptr;
	static inline SDK::APawn* myPawn = nullptr;
	static inline SDK::FVector myLoc;
	static inline SDK::FRotator myRot;
	//-------
	static inline SDK::UCanvas* Canvas = nullptr;
	static inline SDK::FMinimalViewInfo* Camera = nullptr;
	static inline SDK::UKismetMathLibrary* MathLib = nullptr;
	static inline SDK::UKismetSystemLibrary* KismetSystemLib = nullptr;
	static inline SDK::UGameplayStatics* GPStatics = nullptr;
	static inline SDK::UFont* Font = nullptr;
	//-------
	static inline SDK::FVector kUp;

	void ReDefineAttribut()
	{
		//set MathLib for math functions
		if (!this->MathLib) this->MathLib = (SDK::UKismetMathLibrary*)SDK::UKismetMathLibrary::GetDefaultObj();

		//set SystemLib for math functions
		if (!this->KismetSystemLib) this->KismetSystemLib = (SDK::UKismetSystemLibrary*)SDK::UKismetSystemLibrary::GetDefaultObj();

		//Set GPStatics for game functions
		if (!this->GPStatics) this->GPStatics = (SDK::UGameplayStatics*)SDK::UGameplayStatics::GetDefaultObj();

		this->Engine = SDK::UEngine::GetEngine();

		this->GWorld = (SDK::UWorld**)((DWORD64)GetModuleHandleW(NULL) + offset::G_WORLD);
		if (this->GWorld) this->World = *this->GWorld;
		else this->World = nullptr;

		if (this->World) this->level = this->World->PersistentLevel;
		else this->level = nullptr;

		if (this->World) this->gameMode = reinterpret_cast<SDK::ABP_FirstPersonGameMode_C*>(this->World->AuthorityGameMode);
		else this->gameMode = nullptr;

		if (this->World) this->GameState = reinterpret_cast<SDK::ABP_FirstPersonGameState_C*>(this->World->GameState);
		else this->GameState = nullptr;

		if (this->World) this->gameInstance = this->World->OwningGameInstance;
		else this->gameInstance = nullptr;

		if (this->gameInstance && this->gameInstance->LocalPlayers.Num() > 0) this->LocalPlayer = this->gameInstance->LocalPlayers[0];
		else this->LocalPlayer = nullptr;

		if (this->LocalPlayer) this->PlayerController = reinterpret_cast<SDK::ABP_NNFirstPersonPlayerController_C*>(this->LocalPlayer->PlayerController);
		else this->PlayerController = nullptr;

		if (this->PlayerController) this->PlayerCameraManager = this->PlayerController->PlayerCameraManager;
		else this->PlayerCameraManager = nullptr;

		if (this->PlayerController) this->myPawn = this->PlayerController->K2_GetPawn();
		else this->myPawn = nullptr;

		if (this->PlayerController) this->myCharacter = reinterpret_cast<SDK::ABP_FirstPersonCharacter_C*>(this->PlayerController->Character);
		else this->myCharacter = nullptr;

		if (this->PlayerCameraManager) this->myLoc = this->PlayerCameraManager->GetCameraLocation();
		else this->myLoc = SDK::FVector();

		if (this->PlayerCameraManager) this->myRot = this->PlayerCameraManager->GetCameraRotation();
		else this->myRot = SDK::FRotator();

		kUp = SDK::FVector(0.0, 0.0, 1.0);
	}

	bool bFov = false;
	float Fov = 0.f;
	float Smoothing = 1.f;
	SDK::FVector2D w2s(SDK::FVector location)
	{
		SDK::FVector2D screen;

		if (this->PlayerController)
		{
			this->PlayerController->ProjectWorldLocationToScreen(location, &screen, false);
			return screen;
		}

		return { 0,0 };
	}

	inline void SetAim(SDK::APlayerCameraManager* MyCamera, SDK::ACharacter* Target, int bone)
	{
		if (this->PlayerController)
		{
			SDK::FVector CamLocation = MyCamera->GetCameraLocation();
			SDK::FRotator CamRotation = MyCamera->GetCameraRotation();
			SDK::FVector AimLocation;
			SDK::FRotator AimRotation, CurrentRotation;

			AimLocation = Target->Mesh->GetSocketLocation(Target->Mesh->GetBoneName(bone));

			//get aim rotation
			AimRotation = MathLib->FindLookAtRotation(CamLocation, AimLocation);

			//set aim
			this->PlayerController->SetControlRotation(AimRotation);
		}
	}

	float deg_2_rad(float degrees) {
		return degrees * (M_PI / 180);
	}

	SDK::FVector AngleToFVector(SDK::FVector location, SDK::FRotator rotator, float distance) {
		//float pitch = -deg_2_rad(rotator.Pitch); // depent camera
		float pitch = deg_2_rad(rotator.Pitch);
		float yaw = deg_2_rad(rotator.Yaw);
		float x = cos(yaw) * cos(pitch);
		float y = sin(yaw) * cos(pitch);
		float z = sin(pitch);

		return location + SDK::FVector(x, y, z) * distance;
	}

	void angle_vectors(const SDK::FVector& angles, SDK::FVector* forward) {
		float sp, sy, cp, cy;
		sy = sin(deg_2_rad(angles.Y));
		cy = cos(deg_2_rad(angles.Y));
		sp = sin(deg_2_rad(angles.X));
		cp = cos(deg_2_rad(angles.X));
		forward->X = cp * cy;
		forward->Y = cp * sy;
		forward->Z = -sp;
	}

	void __forceinline AnsiToWide(char* inAnsi, wchar_t* outWide)
	{
		int i = 0;
		for (; inAnsi[i] != '\0'; i++)
			outWide[i] = (wchar_t)(inAnsi)[i];
		outWide[i] = L'\0';
	}

	void SetRotation(SDK::FRotator TargetRotation, bool bWithRotationInput, float Smooth)
	{
		if (this->PlayerController && this->PlayerCameraManager) {
			SDK::FRotator v11 = this->PlayerController->GetControlRotation();
			SDK::FRotator v10 = this->PlayerCameraManager->GetCameraRotation();

			if (!bWithRotationInput)
				v11 = v10;

			SDK::FRotator currentRotation = v11;

			SDK::FRotator smoothRotation;
			smoothRotation.Pitch = currentRotation.Pitch + (TargetRotation.Pitch - currentRotation.Pitch) / Smooth;
			smoothRotation.Yaw = currentRotation.Yaw + (TargetRotation.Yaw - currentRotation.Yaw) / Smooth;
			smoothRotation.Roll = currentRotation.Roll + (TargetRotation.Roll - currentRotation.Roll) / Smooth;

			PlayerController->SetControlRotation(smoothRotation);
		}
	}

	SDK::FRotator ToRotator(SDK::FVector local, SDK::FVector target) {
		SDK::FVector rotation = local - target;

		SDK::FRotator newViewAngle = { 0 };

		float hyp = sqrt(rotation.X * rotation.X + rotation.Y * rotation.Y);

		newViewAngle.Pitch = -atan(rotation.Z / hyp) * (180.f / M_PI);
		newViewAngle.Yaw = atan(rotation.Y / rotation.X) * (180.f / M_PI);
		newViewAngle.Roll = (float)0.f;

		if (rotation.X >= 0.f)
			newViewAngle.Yaw += 180.0f;

		return newViewAngle;
	}

	SDK::FVector GetPlayerVelocity(SDK::ACharacter* p)
	{
		if (p->RootComponent)
			return p->RootComponent->ComponentVelocity;

		return { 0.f, 0.f, 0.f };
	}

	SDK::FVector OriginalLocation = { 0, 0, 0 };
	SDK::FRotator OriginalRotation = { 0, 0, 0 };

	void Aimbot(SDK::ACharacter* Target, SDK::FName& aimingAt, float Smooth)
	{
		if (Target && this->PlayerController && this->PlayerCameraManager)
		{
			SDK::FVector AimDirection = Target->Mesh->GetSocketLocation(aimingAt);
			SDK::FVector CameraCachePrivate_POV_Location = this->PlayerController->PlayerCameraManager->CameraCachePrivate.POV.Location;
			SDK::FRotator AimRotation = ToRotator(CameraCachePrivate_POV_Location, AimDirection);
			SetRotation(AimRotation, false, Smooth);
		}
	}

	static inline SDK::FVector RotateCorner(SDK::FVector origin, SDK::FVector corner, float theta)
	{
		float x = corner.X - origin.X;
		float y = corner.Y - origin.Y;

		return {
			origin.X + (x * cos(theta) - y * sin(theta)),
			origin.Y + (x * sin(theta) + y * cos(theta)),
			corner.Z
		};
	}

	static BOOL IsNotInScreen(int SizeX, int SizeY, SDK::FVector2D Pos, int maxOutOfTheBox = -200) {
		if (
			((Pos.X <= maxOutOfTheBox or Pos.X > SizeX + abs(maxOutOfTheBox)) and (Pos.Y <= maxOutOfTheBox or Pos.Y > SizeY + abs(maxOutOfTheBox)))
			or
			((Pos.X <= maxOutOfTheBox or Pos.X > SizeX + abs(maxOutOfTheBox)) or (Pos.Y <= maxOutOfTheBox or Pos.Y > SizeY + abs(maxOutOfTheBox))))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	float Distance(SDK::FVector target)
	{
		float x = this->myLoc.X - target.X;
		float y = this->myLoc.Y - target.Y;
		float z = this->myLoc.Z - target.Z;

		return sqrtf((x * x) + (y * y) + (z * z)) * 0.03048f;
	}

	SDK::FName getBoneByName(SDK::ACharacter* pawn, std::string nameBone)
	{
		SDK::FName baseReturn;
		for (int y = 0; y < pawn->Mesh->GetNumBones(); y++)
		{
			if (y == 0) baseReturn = pawn->Mesh->GetBoneName(y);
			if (pawn->Mesh->GetBoneName(y).ToString() != nameBone) continue;
			return pawn->Mesh->GetBoneName(y);
		}
		return baseReturn;
	}

	void SetLightShadow(bool disable)
	{
		if (this->Engine && this->Engine->GameViewport)
		{
			uintptr_t* viewModeIndex = (uintptr_t*)((uintptr_t)this->Engine->GameViewport + 0xb0); // 0xb0 = viewModeIndex
			if (disable) {
				if (*viewModeIndex == (uintptr_t)SDK::EViewModeIndex::VMI_Lit) // 0x3
				{
					*viewModeIndex = (uintptr_t)SDK::EViewModeIndex::VMI_Wireframe; // 0x1
				}
			}
			else {
				if (*viewModeIndex == (uintptr_t)SDK::EViewModeIndex::VMI_Wireframe) // 0x1
				{
					*viewModeIndex = (uintptr_t)SDK::EViewModeIndex::VMI_Lit; // 0x3
				}
			}
		}
	}

	//bool VisibleTraceLineCameraToFVector(SDK::FVector endLocation, SDK::UClass* staticActorToTounch)
	//{
	//	if (this->World && this->PlayerCameraManager && this->KismetSystemLib)
	//	{
	//		// TArray(ArrayElementType* Data, uint32 NumElements) : Data(Data), NumElements(NumElements), MaxElements(NumElements) {}
	//		// Add this in TArray
	//		SDK::TArray<SDK::AActor*> ignoreActor(reinterpret_cast<SDK::AActor**>(&this->myCharacter), 1);

	//		SDK::FHitResult hitResult;

	//		bool visisibleSocket = this->KismetSystemLib->LineTraceSingle(this->World, this->PlayerCameraManager->GetCameraLocation(), endLocation,
	//			SDK::ETraceTypeQuery::TraceTypeQuery1, false, ignoreActor, SDK::EDrawDebugTrace::ForDuration, &hitResult, true,
	//			SDK::FLinearColor(255, 255, 255), SDK::FLinearColor(0, 0, 255), 0.1f);

	//		SDK::AActor* actor = hitResult..Get();
	//		if (actor)
	//		{
	//			//ImGui::Text(actor->GetName().c_str());
	//			if (actor->IsA(staticActorToTounch))
	//			{
	//				return true;
	//			}
	//		}
	//	}

	//	return false;
	//}

	bool isValidAddress(void* address) {
		MEMORY_BASIC_INFORMATION mbi;
		if (VirtualQuery(address, &mbi, sizeof(mbi)) == 0) {
			return false;
		}

		if (mbi.State != MEM_COMMIT)
			return false;

		if (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD))
			return false;

		return true;
	}

	// Rend tout le squelette connecté au bone de départ (ex: "head")
	// - Ne trace aucune connexion qui implique un index présent dans bonnesToSkip
	// - Continue toutefois à TRAVERSER ces os pour atteindre les branches derrière
	void DrawSkeletonFrom(SDK::USkeletalMeshComponent* Mesh, SDK::FName StartBone, const std::vector<int32_t>& bonnesToSkip)
	{
		if (!Mesh) return;

		const int32_t boneCount = Mesh->GetNumBones();
		if (boneCount <= 0) return;

		// -> Set O(1) pour tester rapidement si un os est à skipper
		std::unordered_set<int32_t> skipSet(bonnesToSkip.begin(), bonnesToSkip.end());

		// Construire parentOf[i] et children[i]
		std::vector<int32_t> parentOf(boneCount, -1);
		std::vector<std::vector<int32_t>> children(boneCount);
		for (int32_t i = 0; i < boneCount; ++i)
		{
			const SDK::FName boneName = Mesh->GetBoneName(i);
			const SDK::FName parentName = Mesh->GetParentBone(boneName); // peut être "None"
			const int32_t pIdx = Mesh->GetBoneIndex(parentName);
			parentOf[i] = pIdx;                   // -1 si racine
			if (pIdx >= 0) children[pIdx].push_back(i);
		}

		auto BoneWorld = [&](int32_t idx)->SDK::FVector {
			// GetSocketLocation accepte aussi les noms d'os
			return Mesh->GetSocketLocation(Mesh->GetBoneName(idx));
			};

		auto W2Sok = [&](const SDK::FVector& w, SDK::FVector2D& out)->bool {
			out = this->w2s(w);
			return std::isfinite(out.X) && std::isfinite(out.Y);
			};

		const int32_t start = Mesh->GetBoneIndex(StartBone);
		if (start < 0) return;

		std::vector<uint8_t> visited(boneCount, 0);
		std::queue<int32_t> q;
		q.push(start);
		visited[start] = 1;

		// Pour éviter de tracer deux fois la même arête (parent-enfant)
		auto edgeKey = [](int32_t a, int32_t b)->uint64_t {
			if (a > b) std::swap(a, b);
			return (uint64_t(uint32_t(a)) << 32) | uint32_t(b);
			};
		std::unordered_set<uint64_t> drawn;

		while (!q.empty())
		{
			const int32_t cur = q.front(); q.pop();

			// Voisins = enfants + parent
			std::vector<int32_t> neigh = children[cur];
			if (parentOf[cur] >= 0) neigh.push_back(parentOf[cur]);

			// On calculera la position écran de cur seulement si on a une arête à dessiner
			SDK::FVector2D curS;
			bool curSok = false;

			for (const int32_t nb : neigh)
			{
				// Doit-on SKIP cette arête ?
				const bool skipEdge = (skipSet.count(cur) > 0) || (skipSet.count(nb) > 0);

				if (!skipEdge)
				{
					const uint64_t key = edgeKey(cur, nb);
					if (drawn.insert(key).second)
					{
						// Calcul paresseux de curS
						if (!curSok)
						{
							const SDK::FVector curW = BoneWorld(cur);
							if (!W2Sok(curW, curS)) { curSok = false; }
							else { curSok = true; }
						}

						if (curSok)
						{
							const SDK::FVector nbW = BoneWorld(nb);
							SDK::FVector2D nbS;
							if (W2Sok(nbW, nbS))
							{
								overlay::DrawLine(
									{ (float)curS.X, (float)curS.Y },
									{ (float)nbS.X,  (float)nbS.Y },
									CFG.skeletonColor, 1.0f
								);
							}
						}
					}
				}

				// On continue à TRAVERSER même si l'arête est "skippée"
				if (!visited[nb]) { visited[nb] = 1; q.push(nb); }
			}
		}
	}

	static inline SDK::FQuat MakeQuatFromRotator(const SDK::FRotator& R)
	{
		const double DegToRad = 3.14159265358979323846 / 180.0;

		const double HR = R.Roll * DegToRad * 0.5;  // X
		const double HP = R.Pitch * DegToRad * 0.5;  // Y
		const double HY = R.Yaw * DegToRad * 0.5;  // Z

		const double SR = std::sin(HR), CR = std::cos(HR);
		const double SP = std::sin(HP), CP = std::cos(HP);
		const double SY = std::sin(HY), CY = std::cos(HY);

		SDK::FQuat Q{};
		// Ordre Unreal: Yaw(Z) * Pitch(Y) * Roll(X)
		Q.X = SR * CP * CY - CR * SP * SY;
		Q.Y = CR * SP * CY + SR * CP * SY;
		Q.Z = CR * CP * SY - SR * SP * CY;
		Q.W = CR * CP * CY + SR * SP * SY;
		return Q;
	}

	static inline float DegToRad(float deg) {
		return deg * (3.14159265358979323846f / 180.0f);
	}

	static inline bool NearlyZero(float v, float eps = 1e-4f)
	{
		return (v > -eps) && (v < eps);
	}

	static inline bool IsDown(int vk)
	{
		return (GetAsyncKeyState(vk) & 0x8000) != 0;
	}

	// Adapt to each project with showBaseInfo
	void ToggleNoClip(bool bNoclip, bool showBaseInfo = false)
	{
		SDK::APlayerController* PC = this->PlayerController;
		if (!PC) return;

		SDK::ACharacter* Char = static_cast<SDK::ACharacter*>(PC->Pawn);
		if (!Char) return;

		SDK::UCharacterMovementComponent* Move = Char->CharacterMovement;
		SDK::UCapsuleComponent* Capsule = Char->CapsuleComponent;
		if (!Move || !Capsule) return;

		if (showBaseInfo)
		{
			std::string movementModeStr = "Movement mode: " + std::to_string((int)Move->MovementMode);
			ImGui::Text(movementModeStr.c_str());

			std::string gravityScaleStr = "Gravity scale: " + std::to_string(Move->GravityScale);
			ImGui::Text(gravityScaleStr.c_str());

			std::string BrakingFrictionFactorStr = "BrakingFrictionFactor: " + std::to_string(Move->BrakingFrictionFactor);
			ImGui::Text(BrakingFrictionFactorStr.c_str());

			std::string BrakingDecelerationFlyingStr = "BrakingDecelerationFlying: " + std::to_string(Move->BrakingDecelerationFlying);
			ImGui::Text(BrakingDecelerationFlyingStr.c_str());
			return;
		}
		else
		{
			if (bNoclip)
			{
				Move->SetMovementMode(SDK::EMovementMode::MOVE_Flying, 0);
				Move->GravityScale = 0.f;
				Move->MaxFlySpeed = 2400.f;
				Move->BrakingFrictionFactor = 0.f;
				Move->BrakingDecelerationFlying = 0.f;

				Capsule->SetCollisionEnabled(SDK::ECollisionEnabled::NoCollision);
				Char->SetActorEnableCollision(false);
			}
			else
			{
				Char->SetActorEnableCollision(true);
				Capsule->SetCollisionEnabled(SDK::ECollisionEnabled::QueryAndPhysics);
				Move->GravityScale = 1.f;
				Move->SetMovementMode(SDK::EMovementMode::MOVE_Walking, 0);
				Move->BrakingFrictionFactor = 2.f;
				Move->BrakingDecelerationFlying = 2048.f;
			}
		}
	}

	void NoclipTick()
	{
		if (!CFG.b_miscNoClip) return;

		SDK::APlayerController* PC = this->PlayerController;
		if (!PC) return;

		SDK::ACharacter* Char = static_cast<SDK::ACharacter*>(PC->Pawn);
		if (!Char) return;

		SDK::UCharacterMovementComponent* Move = Char->CharacterMovement;
		if (!Move || Move->MovementMode != SDK::EMovementMode::MOVE_Flying) return;

		float v = 0.f;

		if (IsDown(VK_SPACE)) {
			v += 1.f;
		}
		if (IsDown(VK_LCONTROL)) {
			v -= 1.f;
		}

		if (NearlyZero(v)) return;

		Char->AddMovementInput(this->kUp, v * 500.f, true);
	}
};

inline UnrealEngine* ue = new UnrealEngine();