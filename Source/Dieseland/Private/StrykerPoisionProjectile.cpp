

#include "Dieseland.h"
#include "StrykerPoisionProjectile.h"
#include "UnrealNetwork.h"
#include "DieselandEnemyBot.h"
#include "DieselandEnemyAI.h"
#include "DieselandCharacter.h"
#include "DieselandPlayerController.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"


AStrykerPoisionProjectile::AStrykerPoisionProjectile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	ProjectileMovement->InitialSpeed = 1100.0f;
	InitialLifeSpan = 1.3f;
	IsPoison = true;

	//Make the radius larger for this collider
	ProjCollision->SetCapsuleHalfHeight(100.0f);
	ProjCollision->SetCapsuleRadius(100.0f);
	//ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetOwner());
	//ProjectileDamage = DieselanPawn->BasicAttackDamage;

	//TODO the correct particle effect here

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/Test/unreal_particle_strykerthrowingknife.unreal_particle_strykerthrowingknife'"));
	Particle->Template = ParticleSystemAsset.Object;
}


