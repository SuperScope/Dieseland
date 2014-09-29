

#include "Dieseland.h"
#include "MayhemCharacter.h"
#include "DieselandCharacter.h"
#include "DieselandPlayerController.h"
#include "DieselandEnemyBot.h"
#include "DieselandEnemyAI.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

AMayhemCharacter::AMayhemCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	IsMelee = true;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MayhemSkillOneParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_MayhemSkullCrusher_WIP.Unreal_Particle_MayhemSkullCrusher_WIP'"));

	SkillOneParticle = MayhemSkillOneParticleAsset.Object;

	// TODO: Edit attributes for Mayhem
}

void AMayhemCharacter::MeleeAttack()
{
	MeleeCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeleeCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	MeleeCollision->GetOverlappingActors(ActorsInMeleeRange);
	MeleeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AActor* CurActor = NULL;
	for (int32 b = 0; b < ActorsInMeleeRange.Num(); b++)
	{
		CurActor = ActorsInMeleeRange[b];
		if (!CurActor && (CurActor->ActorHasTag(FName(TEXT("Player"))) || CurActor->ActorHasTag(FName(TEXT("Enemy"))))) continue;
		if (!CurActor->IsValidLowLevel()) continue;

		if (Role == ROLE_Authority && CurActor != this)
		{
			EditHealth(-1 * BasicAttackDamage, CurActor);
		}
	}
}

void AMayhemCharacter::SkillOne()
{
	ServerActivateParticle(SkillOneParticle);

	AOECollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AOECollision->SetCollisionProfileName(TEXT("OverlapAll"));
	AOECollision->GetOverlappingActors(ActorsInAOERange);
	AOECollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AActor* CurActor = NULL;
	for (int32 b = 0; b < ActorsInAOERange.Num(); b++)
	{
		CurActor = ActorsInAOERange[b];
		if (!CurActor->IsValidLowLevel()) continue;

		if (Role == ROLE_Authority && CurActor != this)
		{
			EditHealth(-1 * BasicAttackDamage, CurActor);
			if (CurActor->ActorHasTag(FName(TEXT("Player"))))
			{
				ADieselandCharacter* PlayerActor = Cast<ADieselandCharacter>(CurActor);
				//PlayerActor->DisableInput(Cast<ADieselandPlayerController>(PlayerActor->Controller));
				
				EditHealth(-1 * BasicAttackDamage, CurActor);
			}
			else if (CurActor->ActorHasTag(FName(TEXT("Enemy"))))
			{
				ADieselandEnemyBot* EnemyActor = Cast<ADieselandEnemyBot>(CurActor);
				//Cast<ADieselandEnemyAI>(EnemyActor->Controller)->StopMovement();
			
				EditHealth(-1 * BasicAttackDamage, CurActor);
			}
		}
	}
}

void AMayhemCharacter::SkillTwo()
{
	
}

void AMayhemCharacter::SkillThree()
{
	
}
