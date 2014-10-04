

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

	StunLength = 2.0f;

	IronArmorDuration = 4.0f;
	RageDuration = 3.0f;

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

void AMayhemCharacter::Tick(float DeltaSeconds)
{
	UpdateDurationTimers(DeltaSeconds);
	
	Super::Tick(DeltaSeconds);
}

void AMayhemCharacter::UpdateDurationTimers_Implementation(float DeltaSeconds)
{
	if (IronArmorTimer > 0.0f)
	{
		IronArmorTimer -= DeltaSeconds;
		if (IronArmorTimer <= 0.0f)
		{
			IronArmorTimer = 0.0f;
			//reset stats here
		}
	}
	if (RageTimer > 0.0f)
	{
		RageTimer -= DeltaSeconds;
		if (RageTimer <= 0.0f)
		{
			RageTimer = 0.0f;
			//reset stats here
		}
	}
}

bool AMayhemCharacter::UpdateDurationTimers_Validate(float DeltaSeconds)
{
	return true;
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

				PlayerActor->StatusEffects.Add(FString("Stunned"));
				PlayerActor->StunRemaining = StunLength;

				EditHealth(-1 * BasicAttackDamage, CurActor);
			}
			else if (CurActor->ActorHasTag(FName(TEXT("Enemy"))))
			{
				ADieselandEnemyBot* EnemyActor = Cast<ADieselandEnemyBot>(CurActor);

				EnemyActor->StatusEffects.Add(FString("Stunned"));
				EnemyActor->StunRemaining = StunLength;
				EnemyActor->CharacterMovement->MaxWalkSpeed = 0.0f;
				EnemyActor->CharacterMovement->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
			
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

void AMayhemCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMayhemCharacter, StunLength);
	
	DOREPLIFETIME(AMayhemCharacter, IronArmorTimer);
	DOREPLIFETIME(AMayhemCharacter, RageTimer);
}
