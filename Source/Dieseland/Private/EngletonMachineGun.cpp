

#include "Dieseland.h"
#include "EngletonMachineGun.h"
#include "EngletonCharacter.h"


AEngletonMachineGun::AEngletonMachineGun(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//Engleton's Projectiles are meant to move very quickly.
	ProjectileMovement->InitialSpeed = 1000.0f;
	InitialLifeSpan = 0.8f;

	//TODO reset attack damage for engleton machine gun

	//ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetOwner());
	//ProjectileDamage = DieselanPawn->BasicAttackDamage;

	//TODO the correct particle effect here
	/*
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/Test/MovingBulletTest_WIP.MovingBulletTest_WIP'"));
	Particle = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleSystem"));*/
}


