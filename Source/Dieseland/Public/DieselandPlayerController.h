// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "DieselandPlayerController.generated.h"

UCLASS()
class ADieselandPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

public:
	// Called by the server to edit client's health
	UFUNCTION(reliable, Server, WithValidation)
	void ServerEditHealth(int32 Amt, AActor* Target);

	// Calculated rotation of torso
	FRotator FacingRotation;

	// Called when aim input is recieved
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerOnAim(FRotator Rotation);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerMeleeAttack();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerRangedAttack();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSkillOne();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSkillTwo();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSkillThree();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerReload();

	UFUNCTION(Reliable, Server, WithValidation)
	void UpgradeStrength();

	UFUNCTION(Reliable, Server, WithValidation)
	void UpgradeDexterity();

	UFUNCTION(Reliable, Server, WithValidation)
	void UpgradeIntelligence();

	UFUNCTION(Reliable, Server, WithValidation)
	void UpgradeConstitution();


	UFUNCTION(Reliable, Server, WithValidation)
	void UpdateCooldownTimers(float DeltaSeconds);

	//Damage done to actors from fire traps upon exitting them
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Trap)
	int32 LingerDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Trap)
	int32 LingerCount;

	// Input events
	UFUNCTION(Reliable, Server, WithValidation)
	void OnAttackPress();
	UFUNCTION(Reliable, Server, WithValidation)
	void OnAttackRelease();

	UFUNCTION(Reliable, Server, WithValidation)
	void SwapMelee();

	UFUNCTION(Reliable, Server, WithValidation)
	void RespawnPawn();

protected:

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	// Movment input events
	void OnMoveForward(float Val);
	void OnMoveRight(float Val);
	void OnFaceNorth(float Val);
	void OnFaceEast(float Val);
	
};


