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

	// Called by the server to edit client's speed, and damage
	UFUNCTION(reliable, Server, WithValidation)
		void ServerEditSpeedDamage(int32 Speed, int32 Damage, AActor* Target);


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
	void OnTaunt();

	UFUNCTION(Reliable, Server, WithValidation)
	void OnLaugh();

	UFUNCTION(Reliable, Server, WithValidation)
	void OnComment();

	UFUNCTION(Category = UserInterface)
	void OnEscape();

	UFUNCTION(Category = UserInterface)
	void OnShowScore();

	// Cheat code to change characters
	UFUNCTION(exec, BlueprintCallable, Category = Gameplay)
	void ChangeCharacter(FString CharacterName);

	UFUNCTION(Reliable, Server, WithValidation)
	void UpdateCooldownTimers(float DeltaSeconds);

	//Damage done to actors from fire traps upon exitting them
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Trap)
	int32 LingerDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Trap)
	int32 LingerCount; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Trap)
	int32 StatPlusCount;

	// The health regen of a character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float HealthRegenTimer;


	// Input events
	UFUNCTION(Reliable, Server, WithValidation)
	void OnAttackPress();
	UFUNCTION(Reliable, Server, WithValidation)
	void OnAttackRelease();

	UFUNCTION(Reliable, Server, WithValidation)
	void SwapMelee();

	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = Gameplay)
	void RespawnPawn();

protected:

	UClass* ScrapClass;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	virtual void ReceiveBeginPlay() override;

	// Movment input events
	void OnMoveForward(float Val);
	void OnMoveRight(float Val);
	void OnFaceNorth(float Val);
	void OnFaceEast(float Val);
	
};


