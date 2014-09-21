// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Dieseland.h"
#include "DieselandPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "DieselandCharacter.h"
#include "UnrealNetwork.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

ADieselandPlayerController::ADieselandPlayerController(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Used for showing mouse cursor
	//bShowMouseCursor = true;
	//DefaultMouseCursor = EMouseCursor::Crosshairs;

	bReplicates = true;
	
}

void ADieselandPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	UpdateCooldownTimers(DeltaTime);
	
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr){
		// Temporary on screen cooldown display
		GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Blue, FString("Basic Attack: ") + FString::SanitizeFloat(DieselandPawn->BasicAttackTimer));
		GEngine->AddOnScreenDebugMessage(2, 10.0f, FColor::Red, FString("Skill One: ") + FString::SanitizeFloat(DieselandPawn->SkillOneTimer));
		GEngine->AddOnScreenDebugMessage(3, 10.0f, FColor::Green, FString("Skill Two: ") + FString::SanitizeFloat(DieselandPawn->SkillTwoTimer));
		GEngine->AddOnScreenDebugMessage(4, 10.0f, FColor::Yellow, FString("Skill Three: ") + FString::SanitizeFloat(DieselandPawn->SkillThreeTimer));


		if (DieselandPawn->Health <= 0)
		{
			RespawnPawn();

			
		}
	}
}

void ADieselandPlayerController::UpdateCooldownTimers_Implementation(float DeltaSeconds)
{
	if (Cast<ADieselandCharacter>(GetPawn()) != nullptr){
		ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());

		// Update all of the timers
		if (DieselandPawn->BasicAttackTimer > 0.0f)
		{
			DieselandPawn->BasicAttackTimer -= DeltaSeconds;
			if (DieselandPawn->BasicAttackTimer < 0.0f)
			{
				DieselandPawn->BasicAttackTimer = 0.0f;
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
		if (DieselandPawn->BasicAttackTimer <= 0.0f && DieselandPawn->BasicAttackActive)
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

	InputComponent->BindAction("UpgradeStrength", IE_Pressed, this, &ADieselandPlayerController::UpgradeStrength);

	InputComponent->BindAction("Debug_MeleeSwap", IE_Released, this, &ADieselandPlayerController::SwapMelee);
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
		DieselandPawn->SetActorLocation(SpawnLocation);
		DieselandPawn->Health = 100;
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

void ADieselandPlayerController::OnMoveForward(float Val)
{
	if (GetPawn() != nullptr){
		GetPawn()->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Val);
	}
}

void ADieselandPlayerController::OnMoveRight(float Val)
{
	if (GetPawn() != nullptr){
		GetPawn()->AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Val);
	}
}

void ADieselandPlayerController::OnFaceNorth(float Val)
{
	if (GetPawn() != nullptr && Val != 0.0f){
		ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());

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
	if (GetPawn() != nullptr && Val != 0.0f){
		ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
		
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
	Cast<ADieselandCharacter>(GetPawn())->BasicAttackActive = true;
}

bool ADieselandPlayerController::OnAttackPress_Validate()
{
	return true;
}

void ADieselandPlayerController::OnAttackRelease_Implementation()
{
	Cast<ADieselandCharacter>(GetPawn())->BasicAttackActive = false;
}

bool ADieselandPlayerController::OnAttackRelease_Validate()
{
	return true;
}

void ADieselandPlayerController::ServerSkillOne_Implementation()
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr){
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
	if (DieselandPawn != nullptr){
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
	if (DieselandPawn != nullptr){
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
	//TODO: Replace with real level up function
	//ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	//if (DieselandPawn != nullptr){
	//	DieselandPawn->BasicAttackDamage = DieselandPawn->BasicAttackDamage * 2;
	//	GEngine->AddOnScreenDebugMessage(5, 5.0f, FColor::Cyan, FString("Upgraded Strength!"));
	//}
}

bool ADieselandPlayerController::UpgradeStrength_Validate()
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
	if (DieselandPawn != nullptr){
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
	if (DieselandPawn != nullptr){
		DieselandPawn->RangedAttack();
	}
}

bool ADieselandPlayerController::ServerOnAim_Validate(FRotator Rotation)
{
	return true;
}

void ADieselandPlayerController::ServerOnAim_Implementation(FRotator Rotation)
{
	if (GetPawn() != nullptr){
		Cast<ADieselandCharacter>(GetPawn())->AimRotation = Rotation;
	}
}
