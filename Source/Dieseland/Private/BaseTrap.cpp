#include "Dieseland.h"
#include "BaseTrap.h"
#include "UnrealNetwork.h"
#include "DieselandCharacter.h"
#include "DieselandPlayerController.h"
#include "DieselandEnemyBot.h"


ABaseTrap::ABaseTrap(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
	TrapDamage = 40.f;
	TrapSlow = 2.f;
	LingerCoolDown = 5.f;
	LingerDamage = 5;

	SetActorTickEnabled(true);
    PrimaryActorTick.bCanEverTick = true;
    SetRemoteRoleForBackwardsCompat(ENetRole::ROLE_SimulatedProxy);
	bReplicates = true;
	bReplicateMovement = true;

	Tags.Add(FName("Trap"));

}

void ABaseTrap::ReceiveActorBeginOverlap(AActor* OtherActor)
{
	Super::ReceiveActorBeginOverlap(OtherActor);
	if (Role == ROLE_Authority)
	{
		if (OtherActor->ActorHasTag(TEXT("Player")))
		{
			Cast<ADieselandCharacter>(OtherActor)->CharacterMovement->MaxWalkSpeed = Cast<ADieselandCharacter>(OtherActor)->CharacterMovement->MaxWalkSpeed / TrapSlow;
			// TODO: Replace with better damage handling
			Cast<ADieselandCharacter>(OtherActor)->Health -= TrapDamage;

		}
		/*if (OtherActor->ActorHasTag(FName(TEXT("Enemy"))))
		{
			Cast<ADieselandEnemyBot>(OtherActor)->Health -= TrapDamage;
			Cast<ADieselandEnemyBot>(OtherActor)->CharacterMovement->MaxWalkSpeed = Cast<ADieselandEnemyBot>(OtherActor)->CharacterMovement->MaxWalkSpeed / TrapSlow;

			if (Cast<ADieselandEnemyBot>(OtherActor)->Health <= 0)
			{
				OtherActor->Destroy();
			}
		}*/
	}

}

void ABaseTrap::ReceiveActorEndOverlap(AActor* OtherActor)
{
	Super::ReceiveActorEndOverlap(OtherActor);
	if (Role == ROLE_Authority)
	{
		if (OtherActor->ActorHasTag(TEXT("Player")))
		{
			Cast<ADieselandCharacter>(OtherActor)->LingerTimer = LingerCoolDown;
			Cast<ADieselandCharacter>(OtherActor)->LingerDamage = LingerDamage;
			Cast<ADieselandCharacter>(OtherActor)->CharacterMovement->MaxWalkSpeed = Cast<ADieselandCharacter>(OtherActor)->CharacterMovement->MaxWalkSpeed * TrapSlow;
		}
		/*if (OtherActor->ActorHasTag(FName(TEXT("Enemy"))))
		{
			Cast<ADieselandEnemyBot>(OtherActor)->LingerTimer = LingerCoolDown;
			Cast<ADieselandEnemyBot>(OtherActor)->LingerDamage = LingerDamage;
			Cast<ADieselandEnemyBot>(OtherActor)->CharacterMovement->MaxWalkSpeed = Cast<ADieselandEnemyBot>(OtherActor)->CharacterMovement->MaxWalkSpeed * TrapSlow;
		}*/
	}

}

void ABaseTrap::ServerActivateTrap_Implementation()
{
	//Particle->ActivateSystem();
}

bool ABaseTrap::ServerActivateTrap_Validate()
{
	return true;
}