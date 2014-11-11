

#include "Dieseland.h"
#include "EngletonMachineGun.h"
#include "EngletonCharacter.h"
#include "BaseProjectileOnHitEffect.h"
#include "DieselandCharacter.h"
#include "DieselandPlayerState.h"
#include "DieselandEnemyBot.h"
#include "DieselandPlayerController.h"
#include "UnrealNetwork.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"


AEngletonMachineGun::AEngletonMachineGun(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//SetRemoteRoleForBackwardsCompat(ENetRole::ROLE_AutonomousProxy);
	InitialLifeSpan = .6f;
	//the penetration round is meant to be large and move very quickly
	ProjectileMovement->InitialSpeed = 1400.0f;
	ProjCollision->SetCapsuleHalfHeight(150.0f);
	ProjCollision->SetCapsuleRadius(150.0f);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_Bullet1.Unreal_Particle_Bullet1'"));
	Particle->Template = ParticleSystemAsset.Object;



	//temp meshscale
	FVector MeshScale;
	MeshScale = FVector(0.4f, 0.4f, 0.4f);

	this->Mesh->SetWorldScale3D(MeshScale);
}

void AEngletonMachineGun::ReceiveActorBeginOverlap(AActor* OtherActor)
{
	AActor::ReceiveActorBeginOverlap(OtherActor);

	UWorld* const World = GetWorld();
	if (World)
	{
		if (OtherActor->ActorHasTag(TEXT("Player")) || OtherActor->ActorHasTag(TEXT("Enemy"))){
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			if (Role == ROLE_Authority /*&& Cast<ADieselandPlayerController>(GetOwner())->GetPawn() != OtherActor*/)
			{
				if (OtherActor->ActorHasTag(TEXT("Player")) &&
					Cast<ADieselandPlayerState>(Cast<ADieselandCharacter>(OtherActor))->GetTeamNum() !=
					Cast<ADieselandPlayerState>(Cast<ADieselandCharacter>(Cast<ADieselandPlayerController>(GetOwner())))->GetTeamNum())
				{
				//	ABaseProjectileOnHitEffect* const OnHitEffect = World->SpawnActor<ABaseProjectileOnHitEffect>(ABaseProjectileOnHitEffect::StaticClass(), this->GetActorLocation(), this->GetActorRotation(), SpawnParams);
					Cast<ADieselandCharacter>(OtherActor)->EditHealth(-1 * ProjectileDamage, this);
			//		OnHitEffect->ServerActivateProjectile();
					this->Destroy();
				}
				if (OtherActor->ActorHasTag(TEXT("Enemy")))
				{
				//	ABaseProjectileOnHitEffect* const OnHitEffect = World->SpawnActor<ABaseProjectileOnHitEffect>(ABaseProjectileOnHitEffect::StaticClass(), this->GetActorLocation(), this->GetActorRotation(), SpawnParams);
					Cast<ADieselandEnemyBot>(OtherActor)->EditHealth(-1 * ProjectileDamage, this);
				//	OnHitEffect->ServerActivateProjectile();
					this->Destroy();
				}
			}
		}
	}
}
