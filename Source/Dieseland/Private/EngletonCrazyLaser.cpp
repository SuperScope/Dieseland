

#include "Dieseland.h"
#include "EngletonCharacter.h"
#include "EngletonCrazyLaser.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"


AEngletonCrazyLaser::AEngletonCrazyLaser(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	ProjectileMovement->InitialSpeed = 0.0f;
	InitialLifeSpan =12.0f;

	//TODO reset attack damage for engleton machine gun

	//ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetOwner());
	//ProjectileDamage = DieselanPawn->BasicAttackDamage;

	//TODO the correct particle effect here

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_EngletonCrazyLaser_WIP.Unreal_Particle_EngletonCrazyLaser_WIP'"));
	Particle->Template = ParticleSystemAsset.Object;
}


