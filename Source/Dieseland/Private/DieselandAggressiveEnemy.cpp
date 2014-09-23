

#include "Dieseland.h"
#include "DieselandAggressiveEnemy.h"


ADieselandAggressiveEnemy::ADieselandAggressiveEnemy(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//Set size for collision capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);

	//don't rotate when the conntroller rotates
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Configure character movement
	CharacterMovement->bOrientRotationToMovement = true; //makes the character face the direction their moving
	CharacterMovement->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	CharacterMovement->GravityScale = 2.f;
	CharacterMovement->AirControl = 0.80f;
	CharacterMovement->GroundFriction = 3.f;
	CharacterMovement->MaxWalkSpeed = 600.f;
	CharacterMovement->MaxFlySpeed = 600.f;
}


