

#include "Dieseland.h"
#include "DieselandWalkerProjectile.h"
#include "DieselandEnemyBot.h"
#include "DieselandEnemyAI.h"


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

	if (OtherActor == nullptr || OtherActor == NULL || Role == NULL){
		return;
	}


	if (Role == ROLE_Authority && Cast<ADieselandEnemyAI>(GetOwner())->GetPawn() != OtherActor)
	{
		if (OtherActor->ActorHasTag(TEXT("Player")) || OtherActor->ActorHasTag(TEXT("Enemy")) || OtherActor->ActorHasTag(TEXT("ScrapBox")))
		{
			Cast<ADieselandEnemyBot>(Cast<ADieselandEnemyAI>(GetOwner())->GetPawn())->EditHealth(-1 * ProjectileDamage, OtherActor);
			if (!Piercing)
			{
				this->Destroy();
			}

			}
		else if (!OtherActor->ActorHasTag(TEXT("Projectile")))
		{
				this->Destroy();
		}
	}
}
