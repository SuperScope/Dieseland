

#include "Dieseland.h"
#include "DieselandWalkerProjectile.h"
#include "DieselandEnemyBot.h"
#include "DieselandEnemyAI.h"
#include "DieselandCharacter.h"


ADieselandWalkerProjectile::ADieselandWalkerProjectile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	/* for editing stats of projectiel in the future
	ProjectileMovement->InitialSpeed = 1800.0f;
	InitialLifeSpan = 0.5f;

	//TODO reset attack damage for engleton machine gun

	//ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetOwner());
	//ProjectileDamage = DieselanPawn->BasicAttackDamage;

	//TODO the correct particle effect here

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/Test/LargerMachineGunFire.LargerMachineGunFire'"));
	Particle->Template = ParticleSystemAsset.Object;

	//ParticleSystemAsset = (TEXT("ParticleSystem'/Game/Particles/Test/MovingBulletTest_WIP.MovingBulletTest_WIP''"));*/

}
void ADieselandWalkerProjectile::ReceiveActorBeginOverlap(AActor* OtherActor)
{
	Super::ReceiveActorBeginOverlap(OtherActor);

	if (OtherActor == nullptr || OtherActor == NULL){
		return;
	}


	if (Role == ROLE_Authority && Cast<ADieselandEnemyAI>(GetOwner())->GetPawn() != OtherActor)
	{
		if (OtherActor->ActorHasTag(TEXT("Player")))
		{
			Cast<ADieselandCharacter>(OtherActor)->EditHealth(-1 * ProjectileDamage, this);
			if (!Piercing)
			{
				this->Destroy();
			}

		}
		else if (!OtherActor->ActorHasTag(TEXT("Projectile")) && !OtherActor->ActorHasTag(TEXT("Enemy")))
		{
				this->Destroy();
		}
	}
}
