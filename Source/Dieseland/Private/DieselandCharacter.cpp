// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Dieseland.h"
#include "DieselandCharacter.h"
#include "DieselandPlayerController.h"
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

	// Set the starting health value
	Health = 100;

	// Create the text component
	PlayerLabel = PCIP.CreateDefaultSubobject<UTextRenderComponent>(this, TEXT("PlayerLabel"));
	PlayerLabel->AttachTo(RootComponent);
	PlayerLabel->AddRelativeLocation(FVector(-80.0f, 0.0f, 0.0f), false);
	PlayerLabel->AddLocalRotation(FRotator(90.0f, 0.0f, -180.0f));
	PlayerLabel->Text = FString::FromInt(Health);

	// Find the mesh to use for AimMesh component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticAimMesh(TEXT("StaticMesh'/Game/Shapes/Shape_Cube.Shape_Cube'"));

	// Setup the AimMesh component
	AimMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("AimMesh"));
	AimMesh->AttachParent = (Mesh);
	AimMesh->AttachSocketName = FName(TEXT("AimSocket"));
	AimMesh->SetStaticMesh(StaticAimMesh.Object);
	AimMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	AimMesh->SetHiddenInGame(false);
	
	// Tag this character as a player
	Tags.Add(FName("Player"));

	// Ensure replication
	bReplicates = true;
	AimMesh->SetIsReplicated(true);
	Mesh->SetIsReplicated(true);
}

void ADieselandCharacter::Tick(float DeltaSeconds)
{
	// Every frame set the health display
	// TODO: Remove when UI is completed
	PlayerLabel->SetText(FString::FromInt(Health));
	
	Super::Tick(DeltaSeconds);
}

void ADieselandCharacter::EditHealth(int32 Amt, AActor* Target)
{
	Cast<ADieselandCharacter>(Target)->Health += Amt;

	PlayerLabel->SetText(FString::FromInt(Health));
}


void ADieselandCharacter::Attack()
{

}

void ADieselandCharacter::RangedAttack()
{

}

void ADieselandCharacter::MeleeAttack()
{

}

void ADieselandCharacter::SkillOne()
{

}

void ADieselandCharacter::SkillTwo()
{

}

void ADieselandCharacter::SkillThree()
{

}
void ADieselandCharacter::OnRep_AimRotation()
{

}

void ADieselandCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ADieselandCharacter, Health);
	DOREPLIFETIME(ADieselandCharacter, AimMesh);
	DOREPLIFETIME(ADieselandCharacter, AimRotation);
}
