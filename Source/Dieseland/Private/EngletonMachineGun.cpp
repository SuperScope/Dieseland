

#include "Dieseland.h"
#include "EngletonMachineGun.h"
#include "EngletonCharacter.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"


AEngletonMachineGun::AEngletonMachineGun(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//Engleton's Projectiles are meant to move very quickly.
	ProjectileMovement->InitialSpeed = 1800.0f;
	InitialLifeSpan = 0.5f;

	//TODO reset attack damage for engleton machine gun

	//ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetOwner());
	//ProjectileDamage = DieselanPawn->BasicAttackDamage;

	//TODO the correct particle effect here
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/Test/LargerMachineGunFire.LargerMachineGunFire'"));
	Particle->Template = ParticleSystemAsset.Object;

	//ParticleSystemAsset = (TEXT("ParticleSystem'/Game/Particles/Test/MovingBulletTest_WIP.MovingBulletTest_WIP''"));


	
}

