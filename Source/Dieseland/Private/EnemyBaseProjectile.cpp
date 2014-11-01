

#include "Dieseland.h"
#include "DieselandEnemyBot.h"
#include "DieselandEnemyAI.h"
#include "EnemyBaseProjectile.h"


AEnemyBaseProjectile::AEnemyBaseProjectile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

void AEnemyBaseProjectile::ReceiveActorBeginOverlap(AActor* OtherActor)
{
	if ((Cast<ADieselandEnemyAI>(GetOwner())->GetPawn() == nullptr)){
		return;
	}
	AActor::ReceiveActorBeginOverlap(OtherActor);
	if (Role == ROLE_Authority && Cast<ADieselandEnemyAI>(GetOwner())->GetPawn() != OtherActor)
	{
		if (Cast<ADieselandEnemyAI>(GetOwner())->GetPawn() != OtherActor && (OtherActor->ActorHasTag(TEXT("Player")) || OtherActor->ActorHasTag(FName(TEXT("Enemy")))))
		{

			Cast<ADieselandEnemyBot>(Cast<ADieselandEnemyAI>(GetOwner())->GetPawn())->EditHealth(-1 * ProjectileDamage, OtherActor);
			if (!Piercing)
			{
				this->Destroy();
			}

		}
	}

}


