// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Dieseland.h"
#include "DieselandPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "DieselandCharacter.h"
#include "UnrealNetwork.h"

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

}

void ADieselandPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ADieselandPlayerController::OnMoveForward);
	InputComponent->BindAxis("MoveRight", this, &ADieselandPlayerController::OnMoveRight);

	InputComponent->BindAxis("LookNorth", this, &ADieselandPlayerController::OnFaceNorth);
	InputComponent->BindAxis("LookEast", this, &ADieselandPlayerController::OnFaceEast);

	InputComponent->BindAction("Attack", IE_Released, this, &ADieselandPlayerController::OnAttack);

	InputComponent->BindAction("Skill_1", IE_Released, this, &ADieselandPlayerController::OnSkillOne);
	InputComponent->BindAction("Skill_2", IE_Released, this, &ADieselandPlayerController::OnSkillTwo);
	InputComponent->BindAction("Skill_3", IE_Released, this, &ADieselandPlayerController::OnSkillThree);
}

bool ADieselandPlayerController::ServerEditHealth_Validate(int32 Amt, AActor* Target)
{
	return true;
}

void ADieselandPlayerController::ServerEditHealth_Implementation(int32 Amt, AActor* Target)
{
	Cast<ADieselandCharacter>(this->GetPawn())->EditHealth(Amt, Target);
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

		FVector TempAxisVector = FVector(Val, (GetInputAxisValue("LookEast") * 1.0f), 0.0f);
		AimRotation = FRotationMatrix::MakeFromX(TempAxisVector).Rotator();
		
		if (DieselandPawn->AimMesh->GetComponentRotation() != AimRotation){
			DieselandPawn->AimMesh->SetWorldRotation(AimRotation);
			if (Role < ROLE_Authority){
				ServerOnAim(AimRotation);
			}
		}
	}
}
void ADieselandPlayerController::OnFaceEast(float Val)
{
	if (GetPawn() != nullptr && Val != 0.0f){
		ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
		
		FVector TempAxisVector = FVector(GetInputAxisValue("LookNorth"), Val * 1.0f, 0.0f);
		AimRotation = FRotationMatrix::MakeFromX(TempAxisVector).Rotator();

		if (DieselandPawn->AimMesh->GetComponentRotation() != AimRotation){
			DieselandPawn->AimMesh->SetWorldRotation(AimRotation);
			if (Role < ROLE_Authority){
				ServerOnAim(AimRotation);
			}
		}
	}
}

void ADieselandPlayerController::OnAttack()
{

}

void ADieselandPlayerController::OnSkillOne()
{

}

void ADieselandPlayerController::OnSkillTwo()
{

}

void ADieselandPlayerController::OnSkillThree()
{

}

bool ADieselandPlayerController::ServerOnAim_Validate(FRotator Rotation)
{
	return true;
}

void ADieselandPlayerController::ServerOnAim_Implementation(FRotator Rotation)
{
	if (GetPawn() != nullptr){
		Cast<ADieselandCharacter>(GetPawn())->AimMesh->SetWorldRotation(Rotation);
	}
}
