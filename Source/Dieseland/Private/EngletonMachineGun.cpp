

#include "Dieseland.h"
#include "EngletonMachineGun.h"
#include "EngletonCharacter.h"
#include "BaseProjectileOnHitEffect.h"
#include "DieselandCharacter.h"
#include "DieselandPlayerState.h"
#include "DieselandEnemyBot.h"
#include "ScrapBox.h"
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
	Particle->SetIsReplicated(true);
	
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
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		if (Role == ROLE_Authority && GetOwner() != nullptr && GetOwner() != OtherActor)
		{
			if (OtherActor->ActorHasTag(TEXT("Player")) &&
				Cast<ADieselandCharacter>(OtherActor)->GetTeamNumber() !=
				Cast<ADieselandCharacter>(Cast<ADieselandPlayerController>(GetOwner())->GetPawn())->GetTeamNumber())
			{
				ABaseProjectileOnHitEffect* const OnHitEffect = World->SpawnActor<ABaseProjectileOnHitEffect>(ABaseProjectileOnHitEffect::StaticClass(), this->GetActorLocation(), this->GetActorRotation(), SpawnParams);
				Cast<ADieselandCharacter>(OtherActor)->EditHealth(-1 * ProjectileDamage, Cast<ADieselandCharacter>(Cast<ADieselandPlayerController>(GetOwner())->GetPawn()));
				OnHitEffect->ServerActivateProjectile();
				this->Destroy();
			}
			else if (OtherActor->ActorHasTag(TEXT("Enemy")))
			{
				Cast<ADieselandEnemyBot>(OtherActor)->EditHealth(-1 * ProjectileDamage, Cast<ADieselandCharacter>(Cast<ADieselandPlayerController>(GetOwner())->GetPawn()));
				ABaseProjectileOnHitEffect* const OnHitEffect = World->SpawnActor<ABaseProjectileOnHitEffect>(ABaseProjectileOnHitEffect::StaticClass(), this->GetActorLocation(), this->GetActorRotation(), SpawnParams);
				OnHitEffect->ServerActivateProjectile();
				this->Destroy();
			}
			else if (OtherActor->ActorHasTag(TEXT("ScrapBox")))
			{
				Cast<AScrapBox>(OtherActor)->DestroyCrate(this);
			}
		}
	}
}
