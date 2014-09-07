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

protected:

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	// Input events
	void OnAttack();
	void OnSkillOne();
	void OnSkillTwo();
	void OnSkillThree();

	// Movment input events
	void OnMoveForward(float Val);
	void OnMoveRight(float Val);
	void OnFaceNorth(float Val);
	void OnFaceEast(float Val);
	
};


