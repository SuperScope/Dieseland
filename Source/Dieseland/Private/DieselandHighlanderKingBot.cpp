


#include "Dieseland.h"
#include "UnrealNetwork.h"
#include "DieselandEnemyAI.h"
#include "DieselandHighlanderKingBot.h"




ADieselandHighlanderKingBot::ADieselandHighlanderKingBot(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	CannonRange = 800;

	//all of the variables needed for creating a collider
	CannonZoneCollision = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("CannonZone"));
	CannonZoneCollision->AttachParent = (Mesh);
	CannonZoneCollision->AttachSocketName = FName(TEXT("AimSocket"));
	CannonZoneCollision->AddLocalOffset(FVector(0.0f, 0.0f, (CannonRange / 2.0f) + 50.0f));
	//CannonZoneCollision->f
	//CannonZoneCollision->SetCapsuleHalfHeight(CannonRange / 2.0f);
	//CannonZoneCollision->SetCapsuleRadius(CannonRange / 2.0f);
	CannonZoneCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ADieselandHighlanderKingBot::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	// Replicate to everyone
	DOREPLIFETIME(ADieselandHighlanderKingBot, CannonZoneCollision);
	DOREPLIFETIME(ADieselandHighlanderKingBot, CannonRange);
	DOREPLIFETIME(ADieselandHighlanderKingBot, CannonDamagePerSecond);


}

void ADieselandHighlanderKingBot::CannonAttack()
{
	ADieselandEnemyAI* BotController = Cast<ADieselandEnemyAI>(Controller);

	CannonZoneCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CannonZoneCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	CannonZoneCollision->GetOverlappingActors(ActorsInZoneRange);
	CannonZoneCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AActor* CurActor = NULL;
	for (int32 b = 0; b < ActorsInZoneRange.Num(); b++)
	{
		CurActor = ActorsInZoneRange[b];
		if (!CurActor && CurActor->ActorHasTag(FName(TEXT("Player")))) continue;
		if (!CurActor->IsValidLowLevel()) continue;

		if (Role == ROLE_Authority && CurActor->ActorHasTag(FName(TEXT("Player")))){
			EditHealth(-1 * BasicAttackDamage, CurActor);
		}
	}
}