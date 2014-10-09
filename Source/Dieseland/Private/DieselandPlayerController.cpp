// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Dieseland.h"
#include "DieselandPlayerController.h"
#include "DieselandGameMode.h"
#include "AI/Navigation/NavigationSystem.h"
#include "DieselandCharacter.h"
#include "UnrealNetwork.h"
#include "BaseTrap.h"
#include "Scrap.h"
#include "DieselandStaticLibrary.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

ADieselandPlayerController::ADieselandPlayerController(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Used for showing mouse cursor
	//bShowMouseCursor = true;
	//DefaultMouseCursor = EMouseCursor::Crosshairs;

	static ConstructorHelpers::FObjectFinder<UBlueprint> ScrapBlueprint(TEXT("Blueprint'/Game/Blueprints/Scrap_BP.Scrap_BP'"));
	if (ScrapBlueprint.Object)
	{
		ScrapClass = (UClass*)ScrapBlueprint.Object->GeneratedClass;
	}

	HealthRegenTimer = 0;
	bReplicates = true;
	LingerCount = 0;
	StatPlusCount = 0;
}

void ADieselandPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	UpdateCooldownTimers(DeltaTime);
	
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr){

		// Temporary on screen cooldown display
		/*GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor::Green, FString("Basic Attack Ammo: ") + FString::SanitizeFloat(DieselandPawn->BasicAttackAmmo));
		GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Blue, FString("Basic Attack: ") + FString::SanitizeFloat(DieselandPawn->BasicAttackTimer));
		GEngine->AddOnScreenDebugMessage(2, 10.0f, FColor::Red, FString("Skill One: ") + FString::SanitizeFloat(DieselandPawn->SkillOneTimer));
		GEngine->AddOnScreenDebugMessage(3, 10.0f, FColor::Green, FString("Skill Two: ") + FString::SanitizeFloat(DieselandPawn->SkillTwoTimer));
		GEngine->AddOnScreenDebugMessage(4, 10.0f, FColor::Yellow, FString("Skill Three: ") + FString::SanitizeFloat(DieselandPawn->SkillThreeTimer));
		GEngine->AddOnScreenDebugMessage(5, 10.0f, FColor::Black, FString("Trap Countdown: ") + FString::SanitizeFloat(DieselandPawn->LingerTimer));
		GEngine->AddOnScreenDebugMessage(20, 10.0f, FColor::Red, FString("Kills: ") + FString::FromInt(DieselandPawn->Kills));

		if (DieselandPawn->BasicAttackReloadTimer > 0.0f){
			GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Yellow, FString("Basic Attack Reload: ") + FString::SanitizeFloat(DieselandPawn->BasicAttackReloadTimer));
		}*/

		if (DieselandPawn->Health <= 0)
		{
			//Prevents multiple respawns
			DieselandPawn->Health = 1;

			RespawnPawn();
			DieselandPawn->LingerTimer = 0;

		}
	}
}

void ADieselandPlayerController::UpdateCooldownTimers_Implementation(float DeltaSeconds)
{
	if (Cast<ADieselandCharacter>(GetPawn()) != nullptr){
		ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());

		

		//reset to max health
		if (DieselandPawn->Health > DieselandPawn->MaxHealth){
			DieselandPawn->Health = DieselandPawn->MaxHealth;
		}
		HealthRegenTimer += DeltaSeconds;
		//health regeneration
		if (HealthRegenTimer >= 1){
			if (DieselandPawn->Health < DieselandPawn->MaxHealth){
				DieselandPawn->Health += DieselandPawn->HealthRegeneration;
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));
			}
			HealthRegenTimer = 0;
		}

		// Update all of the timers
		if (DieselandPawn->BasicAttackTimer > 0.0f)
		{
			DieselandPawn->BasicAttackTimer -= DeltaSeconds;
			if (DieselandPawn->BasicAttackTimer < 0.0f)
			{
				DieselandPawn->BasicAttackTimer = 0.0f;
			}
		}
		if (DieselandPawn->BasicAttackReloadTimer > 0.0f)
		{
			DieselandPawn->BasicAttackReloadTimer -= DeltaSeconds;
			if (DieselandPawn->BasicAttackReloadTimer < 0.0f)
			{
				DieselandPawn->BasicAttackReloadTimer = 0.0f;
			}
			if (DieselandPawn->BasicAttackReloadTimer == 0.0f && DieselandPawn->BasicAttackAmmo == 0)
			{
				DieselandPawn->BasicAttackAmmo = DieselandPawn->BasicAttackMag;
			}
		}
		if (DieselandPawn->LingerTimer > 0.0f)
		{
			DieselandPawn->LingerTimer -= DeltaSeconds;
			if (DieselandPawn->LingerTimer < 0.0f)
			{
				DieselandPawn->LingerTimer = 0;
				LingerCount = 0;
			}
			if ((((DieselandPawn->LingerTimer < 5.f) && (DieselandPawn->LingerTimer > 4.f)) || ((DieselandPawn->LingerTimer < 3.f) && (DieselandPawn->LingerTimer > 2.f)) || ((DieselandPawn->LingerTimer < 1.f) && (DieselandPawn->LingerTimer > 0.f))) && (LingerCount == 0))
			{
				DieselandPawn->Health = DieselandPawn->Health - DieselandPawn->LingerDamage;
				LingerCount = 1;
			}
			else if ((((DieselandPawn->LingerTimer < 4.f) && (DieselandPawn->LingerTimer > 3.f)) || ((DieselandPawn->LingerTimer < 2.f) && (DieselandPawn->LingerTimer > 1.f))) && (LingerCount == 1))
			{
				DieselandPawn->Health = DieselandPawn->Health - DieselandPawn->LingerDamage;
				LingerCount = 0;
			}
		}
		if (DieselandPawn->SkillOneTimer > 0.0f)
		{
			DieselandPawn->SkillOneTimer -= DeltaSeconds;
			if (DieselandPawn->SkillOneTimer < 0.0f)
			{
				DieselandPawn->SkillOneTimer = 0.0f;
			}
		}

		if (DieselandPawn->SkillTwoTimer > 0.0f)
		{
			DieselandPawn->SkillTwoTimer -= DeltaSeconds;
			if (DieselandPawn->SkillTwoTimer < 0.0f)
			{
				DieselandPawn->SkillTwoTimer = 0.0f;
			}
		}

		if (DieselandPawn->SkillThreeTimer > 0.0f)
		{
			DieselandPawn->SkillThreeTimer -= DeltaSeconds;
			if (DieselandPawn->SkillThreeTimer < 0.0f)
			{
				DieselandPawn->SkillThreeTimer = 0.0f;
			}
		}

		// Basic Attack actions
		if (DieselandPawn->BasicAttackTimer <= 0.0f && DieselandPawn->BasicAttackReloadTimer <= 0.0f && DieselandPawn->BasicAttackActive)
		{
			if (DieselandPawn->IsMelee)
			{
				ServerMeleeAttack();
				DieselandPawn->BasicAttackTimer = DieselandPawn->BasicAttackCooldown;
			}
			else
			{
				ServerRangedAttack();
				DieselandPawn->BasicAttackTimer = DieselandPawn->BasicAttackCooldown;
				DieselandPawn->BasicAttackAmmo -= 1;
				if (DieselandPawn->BasicAttackAmmo <= 0){
					DieselandPawn->BasicAttackReloadTimer = DieselandPawn->BasicAttackReloadSpeed;
				}
			}
		}
		if (DieselandPawn->StunRemaining > 0.0f)
		{
			DieselandPawn->StunRemaining -= DeltaSeconds;
			if (DieselandPawn->StunRemaining <= 0.0f)
			{
				DieselandPawn->StunRemaining = 0.0f;
				DieselandPawn->StatusEffects.Remove(FString("Stunned"));
			}
		}
	}
}

bool ADieselandPlayerController::UpdateCooldownTimers_Validate(float DeltaSeconds)
{
	return true;
}

void ADieselandPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ADieselandPlayerController::OnMoveForward);
	InputComponent->BindAxis("MoveRight", this, &ADieselandPlayerController::OnMoveRight);

	InputComponent->BindAxis("LookNorth", this, &ADieselandPlayerController::OnFaceNorth);
	InputComponent->BindAxis("LookEast", this, &ADieselandPlayerController::OnFaceEast);

	InputComponent->BindAction("Attack", IE_Pressed, this, &ADieselandPlayerController::OnAttackPress);
	InputComponent->BindAction("Attack", IE_Released, this, &ADieselandPlayerController::OnAttackRelease);

	InputComponent->BindAction("Skill_1", IE_Pressed, this, &ADieselandPlayerController::ServerSkillOne);
	InputComponent->BindAction("Skill_2", IE_Pressed, this, &ADieselandPlayerController::ServerSkillTwo);
	InputComponent->BindAction("Skill_3", IE_Pressed, this, &ADieselandPlayerController::ServerSkillThree);

	InputComponent->BindAction("Reload", IE_Pressed, this, &ADieselandPlayerController::ServerReload);

	InputComponent->BindAction("UpgradeStrength", IE_Pressed, this, &ADieselandPlayerController::UpgradeStrength);
	InputComponent->BindAction("UpgradeIntelligence", IE_Pressed, this, &ADieselandPlayerController::UpgradeIntelligence);
	InputComponent->BindAction("UpgradeDexterity", IE_Pressed, this, &ADieselandPlayerController::UpgradeDexterity);
	InputComponent->BindAction("UpgradeConstitution", IE_Pressed, this, &ADieselandPlayerController::UpgradeConstitution);

	InputComponent->BindAction("Debug_MeleeSwap", IE_Released, this, &ADieselandPlayerController::SwapMelee);
}

void ADieselandPlayerController::ReceiveBeginPlay()
{
	if (Role == ROLE_Authority)
	{
		Cast<ADieselandGameMode>(GetWorld()->GetAuthGameMode())->StartGame();
	}
}

bool ADieselandPlayerController::RespawnPawn_Validate()
{
	return true;
}

void ADieselandPlayerController::RespawnPawn_Implementation()
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());

	if (DieselandPawn != nullptr)
	{
		if (Role == ROLE_Authority)
		{
			FVector TempEnemyLoc = FVector(DieselandPawn->GetActorLocation().X, DieselandPawn->GetActorLocation().Y, DieselandPawn->GetActorLocation().Z);

			DieselandPawn->SetActorLocation(SpawnLocation);
			DieselandPawn->Health = DieselandPawn->MaxHealth;
			DieselandPawn->LingerTimer = 0;

			//Spawn Scrap pieces here
			UWorld* const World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;
				for (int32 x = 0; x < 5; x++)
				{
					UDieselandStaticLibrary::SpawnBlueprint<AActor>(World, ScrapClass, FVector(TempEnemyLoc.X, TempEnemyLoc.Y, TempEnemyLoc.Z + (70.0f * x)), FRotator(0.0f, 0.0f, 0.0f));

					//Alternatively used to spawn c++ class
					//AScrap* const Scrap = World->SpawnActor<AScrap>(AScrap::StaticClass(), FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + (70.0f * x)), FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
				}
			}
			
		}

		
	}
}

bool ADieselandPlayerController::ServerReload_Validate()
{
	return true;
}

void ADieselandPlayerController::ServerReload_Implementation()
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());

	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned")))
	{
		if (DieselandPawn->BasicAttackAmmo < DieselandPawn->BasicAttackMag && DieselandPawn->BasicAttackReloadTimer <= 0.0f)
		{
			DieselandPawn->BasicAttackReloadTimer = DieselandPawn->BasicAttackReloadSpeed;
			DieselandPawn->BasicAttackAmmo = 0;
		}
	}
}

bool ADieselandPlayerController::ServerEditHealth_Validate(int32 Amt, AActor* Target)
{
	return true;
}

void ADieselandPlayerController::ServerEditHealth_Implementation(int32 Amt, AActor* Target)
{
	// Edit the health of the specific pawn
	if (GetPawn() != nullptr)
	{
		Cast<ADieselandCharacter>(GetPawn())->EditHealth(Amt, Target);
	}
}

bool ADieselandPlayerController::ServerEditSpeedDamage_Validate(int32 Speed, int32 Damage, AActor* Target)
{
	return true;
}

void ADieselandPlayerController::ServerEditSpeedDamage_Implementation(int32 Speed, int32 Damage, AActor* Target)
{
	// Edit the health of the specific pawn
	if (GetPawn() != nullptr)
	{
		Cast<ADieselandCharacter>(GetPawn())->EditSpeedDamage(Speed, Damage, Target);
	}
}

void ADieselandPlayerController::OnMoveForward(float Val)
{
	if (GetPawn() != nullptr && !Cast<ADieselandCharacter>(GetPawn())->StatusEffects.Contains(FString("Stunned"))){
		GetPawn()->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Val);
	}
}

void ADieselandPlayerController::OnMoveRight(float Val)
{
	if (GetPawn() != nullptr && !Cast<ADieselandCharacter>(GetPawn())->StatusEffects.Contains(FString("Stunned"))){
		GetPawn()->AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Val);
	}
}

void ADieselandPlayerController::OnFaceNorth(float Val)
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	
	if (GetPawn() != nullptr && Val != 0.0f && DieselandPawn->StatusEffects.Contains(FString("Stunned"))){

		// Convert the joystick axis to a rotator
		FVector TempAxisVector = FVector(Val, (GetInputAxisValue("LookEast") * 1.0f), 0.0f);
		FacingRotation = FRotationMatrix::MakeFromX(TempAxisVector).Rotator();
		
		// If the rotation value is different from previous, change the rotation
		if (DieselandPawn->AimRotation != FacingRotation){
			// Edit the pawn's spine rotation
			DieselandPawn->AimRotation = FacingRotation;
			// If this is the client, send the the rotator to the server
			if (Role < ROLE_Authority){
				ServerOnAim(FacingRotation);
			}
		}
	}
}
void ADieselandPlayerController::OnFaceEast(float Val)
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());

	if (GetPawn() != nullptr && Val != 0.0f && !DieselandPawn->StatusEffects.Contains(FString("Stunned"))){
		
		// Convert the joystick axis to a rotator
		FVector TempAxisVector = FVector(GetInputAxisValue("LookNorth"), Val * 1.0f, 0.0f);
		FacingRotation = FRotationMatrix::MakeFromX(TempAxisVector).Rotator();

		// If the rotation value is different from previous, change the rotation
		if (DieselandPawn->AimRotation != FacingRotation){
			// Edit the pawn's spine rotation
			DieselandPawn->AimRotation = FacingRotation;
			// If this is the client, send the the rotator to the server
			if (Role < ROLE_Authority){
				ServerOnAim(FacingRotation);
			}
		}
	}
}

void ADieselandPlayerController::OnAttackPress_Implementation()
{
	if (GetPawn() != nullptr && !Cast<ADieselandCharacter>(GetPawn())->StatusEffects.Contains(FString("Stunned"))){
		Cast<ADieselandCharacter>(GetPawn())->BasicAttackActive = true;
	}
}

bool ADieselandPlayerController::OnAttackPress_Validate()
{
	return true;
}

void ADieselandPlayerController::OnAttackRelease_Implementation()
{
	if (GetPawn() != nullptr){
		Cast<ADieselandCharacter>(GetPawn())->BasicAttackActive = false;
	}
}

bool ADieselandPlayerController::OnAttackRelease_Validate()
{
	return true;
}

void ADieselandPlayerController::ServerSkillOne_Implementation()
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned"))){
		if (DieselandPawn->SkillOneTimer <= 0.0f)
		{
			DieselandPawn->SkillOne();
			DieselandPawn->SkillOneTimer = DieselandPawn->SkillOneCooldown;
		}
	}
}

bool ADieselandPlayerController::ServerSkillOne_Validate()
{
	return true;
}

void ADieselandPlayerController::ServerSkillTwo_Implementation()
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned"))){
		if (DieselandPawn->SkillTwoTimer <= 0.0f)
		{
			DieselandPawn->SkillTwo();
			DieselandPawn->SkillTwoTimer = DieselandPawn->SkillTwoCooldown;
		}
	}
}

bool ADieselandPlayerController::ServerSkillTwo_Validate()
{
	return true;
}

void ADieselandPlayerController::ServerSkillThree_Implementation()
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned"))){
		if (DieselandPawn->SkillThreeTimer <= 0.0f)
		{
			DieselandPawn->SkillThree();
			DieselandPawn->SkillThreeTimer = DieselandPawn->SkillThreeCooldown;
		}
	}
}

bool ADieselandPlayerController::ServerSkillThree_Validate()
{
	return true;
}

void ADieselandPlayerController::UpgradeStrength_Implementation()
{
	//here is the real level up function
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned")) && DieselandPawn->CharacterLevel < 20 && DieselandPawn->Scrap >= 3){
		DieselandPawn->Scrap -= 3;
		DieselandPawn->Strength += 3;
		DieselandPawn->CalculateStats();
		GEngine->AddOnScreenDebugMessage(5, 5.0f, FColor::Cyan, FString("Upgraded Strength!"));
		StatPlusCount++;
	}
}


bool ADieselandPlayerController::UpgradeStrength_Validate()
{
	return true;
}

void ADieselandPlayerController::UpgradeIntelligence_Implementation()
{
	//here is the real level up function
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned")) && DieselandPawn->CharacterLevel < 20 && DieselandPawn->Scrap >= 3){
		DieselandPawn->Scrap -= 3;
		DieselandPawn->Intelligence += 3;
		DieselandPawn->CalculateStats();
		GEngine->AddOnScreenDebugMessage(5, 5.0f, FColor::Cyan, FString("Upgraded Intelligence!"));
		StatPlusCount++;
	}
}

bool ADieselandPlayerController::UpgradeIntelligence_Validate()
{
	return true;
}
void ADieselandPlayerController::UpgradeDexterity_Implementation()
{
	//here is the real level up function
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned")) && DieselandPawn->CharacterLevel < 20 && DieselandPawn->Scrap >= 3){
		DieselandPawn->Scrap -= 3;
		DieselandPawn->Dexterity += 3;
		DieselandPawn->CalculateStats();
		GEngine->AddOnScreenDebugMessage(5, 5.0f, FColor::Cyan, FString("Upgraded Dexterity!"));
		StatPlusCount++;
	}
}

bool ADieselandPlayerController::UpgradeDexterity_Validate()
{
	return true;
}
void ADieselandPlayerController::UpgradeConstitution_Implementation()
{
	//here we upgrade the players constitution
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned")) && DieselandPawn->CharacterLevel < 20 && DieselandPawn->Scrap >= 3){
		DieselandPawn->Scrap -= 3;
		DieselandPawn->Constitution += 3;
		DieselandPawn->CalculateStats();
		GEngine->AddOnScreenDebugMessage(5, 5.0f, FColor::Cyan, FString("Upgraded Constitution!"));
		StatPlusCount++;
	}
}

bool ADieselandPlayerController::UpgradeConstitution_Validate()
{
	return true;
}


void ADieselandPlayerController::SwapMelee_Implementation()
{
	//Uncomment only if you need to test melee/ranged swapping
	/*if (Cast<ADieselandCharacter>(GetPawn())->IsMelee)
	{
		Cast<ADieselandCharacter>(GetPawn())->IsMelee = false;
		GEngine->AddOnScreenDebugMessage(6, 10.0f, FColor::Cyan, FString("Now using ranged attack"));
	}
	else
	{
		Cast<ADieselandCharacter>(GetPawn())->IsMelee = true;
		GEngine->AddOnScreenDebugMessage(6, 10.0f, FColor::Cyan, FString("Now using melee attack"));
	}*/
	
}

bool ADieselandPlayerController::SwapMelee_Validate()
{
	return true;
}

bool ADieselandPlayerController::ServerMeleeAttack_Validate()
{
	return true;
}

void ADieselandPlayerController::ServerMeleeAttack_Implementation()
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned"))){
		DieselandPawn->MeleeAttack();
	}
}

bool ADieselandPlayerController::ServerRangedAttack_Validate()
{
	return true;
}

void ADieselandPlayerController::ServerRangedAttack_Implementation()
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned"))){
		DieselandPawn->RangedAttack();
	}
}

bool ADieselandPlayerController::ServerOnAim_Validate(FRotator Rotation)
{
	return true;
}

void ADieselandPlayerController::ServerOnAim_Implementation(FRotator Rotation)
{
	if (GetPawn() != nullptr && !Cast<ADieselandCharacter>(GetPawn())->StatusEffects.Contains(FString("Stunned"))){
		Cast<ADieselandCharacter>(GetPawn())->AimRotation = Rotation;
	}
}
