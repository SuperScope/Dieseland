

#include "Dieseland.h"
#include "EngletonCharacter.h"
#include "EngletonCrazyLaser.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"


AEngletonCrazyLaser::AEngletonCrazyLaser(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	ProjectileMovement->InitialSpeed = 1300.0f;
	InitialLifeSpan = 1.5f;

	//Make the radius larger for this collider
	ProjCollision->SetCapsuleHalfHeight(100.0f);
	ProjCollision->SetCapsuleRadius(100.0f);
	//ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetOwner());
	//ProjectileDamage = DieselanPawn->BasicAttackDamage;

	//TODO the correct particle effect here

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/Test/TempCrazylaser.TempCrazylaser'"));
	Particle->Template = ParticleSystemAsset.Object;
}

