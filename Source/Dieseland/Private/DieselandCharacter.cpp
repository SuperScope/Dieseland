// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Dieseland.h"
#include "DieselandCharacter.h"
#include "UnrealNetwork.h"

ADieselandCharacter::ADieselandCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Set size for player capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	CharacterMovement->bOrientRotationToMovement = true; // Rotate character to moving direction
	CharacterMovement->RotationRate = FRotator(0.f, 640.f, 0.f);
	CharacterMovement->bConstrainToPlane = true;
	CharacterMovement->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUseControllerViewRotation = false; // Camera does not rotate relative to arm

	Health = 100;

	// Create the text component
	PlayerLabel = PCIP.CreateDefaultSubobject<UTextRenderComponent>(this, TEXT("PlayerLabel"));
	PlayerLabel->AttachTo(RootComponent);
	PlayerLabel->AddRelativeLocation(FVector(-80.0f, 0.0f, 0.0f), false);
	PlayerLabel->AddLocalRotation(FRotator(90.0f, 0.0f, -180.0f));
	PlayerLabel->Text = FString::FromInt(Health);

	Tags.Add(FName("Player"));
}

void ADieselandCharacter::Tick(float DeltaSeconds)
{
	PlayerLabel->SetText(FString::FromInt(Health));

	Super::Tick(DeltaSeconds);
}

void ADieselandCharacter::EditHealth(int32 Amt)
{
	Health += Amt;
	//PlayerLabel->SetText(FString::FromInt(Health));

	if (Role < ROLE_Authority)
	{
		ServerEditHealth(Amt);
	}
}
bool ADieselandCharacter::ServerEditHealth_Validate(int32 Amt)
{
	return true;
}

void ADieselandCharacter::ServerEditHealth_Implementation(int32 Amt)
{
	EditHealth(Amt);
}

void ADieselandCharacter::BasicAttack(AActor* Target)
{
	//GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Red, FString("Clicked an opponent"));
	Cast<ADieselandCharacter>(Target)->EditHealth(-1);
}

void ADieselandCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ADieselandCharacter, Health);
	DOREPLIFETIME(ADieselandCharacter, PlayerLabel);
}
