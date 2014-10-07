

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
	MaxHealth = BaseHealth + (Constitution * 20.0f) + (Strength * 3.0f);
	Health = MaxHealth;
	HealthRegeneration = 1.0f + (Constitution / 10.0f) + (Strength / 20.0f);
	//show those adjustments
	PlayerLabel->SetText(FString::FromInt(Health));
	//adjustments for damage
	BasicAttackDamage = BaseDamage + (Strength * 1.5f) + (Dexterity * .5f) + (Intelligence * .5f);

	BaseSkillOneCooldown = 25.0f;
	BaseSkillTwoCooldown = 16.0f;
	BaseSkillThreeCooldown = 12.0f;

	//adjusments for attackspeed
	BasicAttackCooldown = BaseCooldownSpeed / (1 + (Dexterity / 50));
	SkillOneCooldown = BaseSkillOneCooldown / (1 + Intelligence / 100);
	SkillTwoCooldown = BaseSkillTwoCooldown / (1 + Intelligence / 100);
	SkillThreeCooldown = BaseSkillThreeCooldown / (1 + Intelligence / 100);

	//adjustments for movement Speed
	MoveSpeed = BaseMoveSpeed + (Dexterity * 3);
	this->CharacterMovement->MaxWalkSpeed = MoveSpeed;
	//here I set melee to true so that Mayhem only uses ranged attacks
	IsMelee = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> MayhemSkillOneParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_MayhemSkullCrusher_WIP.Unreal_Particle_MayhemSkullCrusher_WIP'"));
	SkillOneParticle = MayhemSkillOneParticleAsset.Object;

	IronArmorSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Iron Armor Sound"));
	IronArmorSound->AttachParent = RootComponent;
	IronArmorSound->bAutoActivate = true;

	PunchSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Punch Sound"));
	PunchSound->AttachParent = RootComponent;
	PunchSound->bAutoActivate = false;

	UltimateSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Ultimate Sound"));
	UltimateSound->AttachParent = RootComponent;
	UltimateSound->bAutoActivate = false;


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
			MaxHealth = BaseHealth + (Constitution * 20.0f) + (Strength * 3.0f);
			HealthRegeneration -= IronArmorRegenBuff;

			if (Health > MaxHealth){
				Health = MaxHealth;
			}

			IronArmorHealthBuff = 0.0f;
			IronArmorRegenBuff = 0.0f;
			
		}
	}
	if (RageTimer > 0.0f)
	{
		RageTimer -= DeltaSeconds;
		if (RageTimer <= 0.0f)
		{
			RageTimer = 0.0f;

			//reset stats here
			BasicAttackCooldown += RageAttkSpeedBuff;
			CharacterMovement->MaxWalkSpeed -= RageMoveSpeedBuff;

			RageAttkSpeedBuff = 0.0f;
			RageMoveSpeedBuff = 0.0f;
			
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
		if (!CurActor && (CurActor->ActorHasTag(FName(TEXT("Player"))) || CurActor->ActorHasTag(FName(TEXT("Enemy"))) || CurActor->ActorHasTag(FName(TEXT("ScrapBox"))))) continue;
		if (!CurActor->IsValidLowLevel()) continue;

		if (Role == ROLE_Authority && CurActor != this)
		{
			EditHealth(-1 * BasicAttackDamage, CurActor);
		}
	}
	PunchSound->Play();
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
	UltimateSound->Play();
}

void AMayhemCharacter::SkillTwo()
{
	RageMoveSpeedBuff = (CharacterMovement->MaxWalkSpeed * .5f) + (Strength * .06f);
	RageAttkSpeedBuff = (BasicAttackCooldown * .2f) + (Strength * .06f);

	BasicAttackCooldown -= RageAttkSpeedBuff;
	CharacterMovement->MaxWalkSpeed += RageMoveSpeedBuff;

	RageTimer = RageDuration;
}

void AMayhemCharacter::SkillThree()
{
	//TODO: Replace with scaling based on Const??
	IronArmorHealthBuff = (float)MaxHealth * 0.25f;
	IronArmorRegenBuff = (float)HealthRegeneration * 0.25f;

	IronArmorTimer = IronArmorDuration;

	MaxHealth += IronArmorHealthBuff;
	Health += IronArmorHealthBuff;

	HealthRegeneration += IronArmorRegenBuff;
	IronArmorSound->Play();
}

void AMayhemCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMayhemCharacter, StunLength);
	
	DOREPLIFETIME(AMayhemCharacter, IronArmorTimer);
	DOREPLIFETIME(AMayhemCharacter, RageTimer);
}
