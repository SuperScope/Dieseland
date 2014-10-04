

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
	
	//here I set his base values
	BaseMoveSpeed = 400;
	BaseHealth = 400;
	BaseDamage = 25;
	BaseCooldownSpeed = 1.7;
	//here I set his base stats
	Strength = 16;
	Intelligence = 9;
	Dexterity = 10;
	Constitution = 14;

	//here I adjust those base values based on his stats
	//adjustments for health
	Health = BaseHealth + (Constitution * 20.0f) + (Strength * 3.0f);
	//show those adjustments
	PlayerLabel->SetText(FString::FromInt(Health));
	//adjustments for damage
	BasicAttackDamage = BaseDamage + (Strength * 1.5f) + (Dexterity * .5f) + (Intelligence * .5f);
	//adjusments for attackspeed
	BasicAttackCooldown = BaseCooldownSpeed / (1 + (Dexterity / 50));
	//adjustments for movement Speed
	MoveSpeed = BaseMoveSpeed + (Dexterity * 3);
	this->CharacterMovement->MaxWalkSpeed = MoveSpeed;
	//here I set melee to false so that Engleton only uses ranged attacks
	IsMelee = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> MayhemSkillOneParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_MayhemSkullCrusher_WIP.Unreal_Particle_MayhemSkullCrusher_WIP'"));
	SkillOneParticle = MayhemSkillOneParticleAsset.Object;

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
				PlayerActor->StatusEffects.Add(FString("Stunned"));
				PlayerActor->StunRemaining = 10.0f;

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
