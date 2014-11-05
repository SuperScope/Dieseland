

#include "Dieseland.h"
#include "EngletonMachineGun.h"
#include "EngletonCharacter.h"
#include "BaseProjectileOnHitEffect.h"
#include "DieselandCharacter.h"
#include "DieselandPlayerController.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"


AEngletonMachineGun::AEngletonMachineGun(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	InitialLifeSpan = 1.0f;
	//the penetration round is meant to be large and move very quickly
	ProjectileMovement->InitialSpeed = 700.0f;
	ProjCollision->SetCapsuleHalfHeight(150.0f);
	ProjCollision->SetCapsuleRadius(150.0f);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_Bullet1.Unreal_Particle_Bullet1'"));
	Particle = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleSystem"));
	Particle->Template = ParticleSystemAsset.Object;

	//temp meshscale
	FVector MeshScale;
	MeshScale = FVector(0.4f, 0.4f, 0.4f);

	this->Mesh->SetWorldScale3D(MeshScale);
}

void AEngletonMachineGun::ReceiveActorBeginOverlap(AActor* OtherActor)
{
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		AActor::ReceiveActorBeginOverlap(OtherActor);

		if (OtherActor == nullptr){
			return;
		}
		if (Role == ROLE_Authority && Cast<ADieselandPlayerController>(GetOwner())->GetPawn() != OtherActor)
		{
			if (OtherActor->ActorHasTag(TEXT("Player")) || (OtherActor->ActorHasTag(TEXT("Enemy"))))
			{
				ABaseProjectileOnHitEffect* const OnHitEffect = World->SpawnActor<ABaseProjectileOnHitEffect>(ABaseProjectileOnHitEffect::StaticClass(), this->GetActorLocation(), this->GetActorRotation(), SpawnParams);
				OnHitEffect->ServerActivateProjectile();
				this->Destroy();
			}
		}
	}
}
