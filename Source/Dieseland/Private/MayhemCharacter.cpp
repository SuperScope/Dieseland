

#include "Dieseland.h"
#include "MayhemCharacter.h"
#include "DieselandCharacter.h"
#include "ScrapBox.h"
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
	BaseMoveSpeed = 410;
	BaseHealth = 400;
	BaseDamage = 33;
	BaseCooldownSpeed = 1.7;
	//here I set his base stats
	Strength = 16;
	Intelligence = 9;
	Dexterity = 10;
	Constitution = 14;

	StunLength = 5.0f;

	IronArmorDuration = 4.0f;
	RageDuration = 3.0f;

	//here I adjust those base values based on his stats
	//adjustments for health
	MaxHealth = BaseHealth + (Constitution * 20.0f) + (Strength * 3.0f);
	Health = MaxHealth;
	HealthRegeneration = 1.0f + (Constitution / 10.0f) + (Strength / 20.0f);

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

	static ConstructorHelpers::FObjectFinder<UParticleSystem> RageParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_MayhemRage_WIP.Unreal_Particle_MayhemRage_WIP'"));
	SkillTwoParticle = RageParticleAsset.Object;

	IronArmorSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Iron Armor Sound"));
	IronArmorSound->AttachParent = RootComponent;
	IronArmorSound->bAutoActivate = false;

	PunchSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Punch Sound"));
	PunchSound->AttachParent = RootComponent;
	PunchSound->bAutoActivate = false;

	UltimateSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Ultimate Sound"));
	UltimateSound->AttachParent = RootComponent;
	UltimateSound->bAutoActivate = false;

	RageVoiceOver = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Rage Voice Over"));
	RageVoiceOver->AttachParent = RootComponent;
	RageVoiceOver->bAutoActivate = false;

	RageSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Rage Sound"));
	RageSound->AttachParent = RootComponent;
	RageSound->bAutoActivate = false;

	LaughCooldown = 2.0f;
	TauntCooldown = 2.0f;
	CommentCooldown = 4.0f;

}

void AMayhemCharacter::Tick(float DeltaSeconds)
{
	UpdateDurationTimers(DeltaSeconds);
	
	Super::Tick(DeltaSeconds);
}

void AMayhemCharacter::UpdateDurationTimers_Implementation(float DeltaSeconds)
{
	//update aim spheres
	AimSphere->SetWorldRotation(AimRotation.GetNormalized());
	AimSphere->AddLocalRotation(FRotator(0, 90, 0));

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
			if (CurActor->ActorHasTag(FName(TEXT("Player"))) && Cast<ADieselandCharacter>(CurActor)->GetTeamNumber() != this->GetTeamNumber())
			{
				Cast<ADieselandCharacter>(CurActor)->EditHealth(-1 * BasicAttackDamage, this);
			}
			else if (CurActor->ActorHasTag(FName(TEXT("Enemy"))))
			{
				Cast<ADieselandEnemyBot>(CurActor)->EditHealth(-1 * BasicAttackDamage, this);
			}
			else if (CurActor->ActorHasTag(FName(TEXT("ScrapBox"))))
			{
				Cast<AScrapBox>(CurActor)->DestroyCrate(this);
			}
		}
	}
	PunchSound->Play();
}

//Smash
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
			//EditHealth(-1 * BasicAttackDamage, CurActor);
			if (CurActor->ActorHasTag(FName(TEXT("Player"))) && Cast<ADieselandCharacter>(CurActor)->GetTeamNumber() != this->GetTeamNumber())
			{
				ADieselandCharacter* PlayerActor = Cast<ADieselandCharacter>(CurActor);

				PlayerActor->StatusEffects.Add(FString("Stunned"));
				PlayerActor->StunRemaining = StunLength;

				Cast<ADieselandCharacter>(CurActor)->EditHealth(-1 * BasicAttackDamage, this);
			}
			else if (CurActor->ActorHasTag(FName(TEXT("Enemy"))))
			{
				ADieselandEnemyBot* EnemyActor = Cast<ADieselandEnemyBot>(CurActor);

				EnemyActor->StatusEffects.Add(FString("Stunned"));
				EnemyActor->StunRemaining = StunLength;
				EnemyActor->CharacterMovement->MaxWalkSpeed = 0.0f;
				EnemyActor->CharacterMovement->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
			
				Cast<ADieselandCharacter>(CurActor)->EditHealth(-1 * BasicAttackDamage, this);
			}
			else if (CurActor->ActorHasTag(FName(TEXT("ScrapBox"))))
			{
				Cast<AScrapBox>(CurActor)->DestroyCrate(this);
			}
		}
	}
	UltimateSound->Play();
}

//rage
void AMayhemCharacter::SkillTwo()
{
	ServerActivateParticle(SkillTwoParticle);
	RageSound->Play();
	RageVoiceOver->Play();
	RageMoveSpeedBuff = (CharacterMovement->MaxWalkSpeed * .5f) + (Strength * .06f);
	RageAttkSpeedBuff = (BasicAttackCooldown * .1f) + (Strength * .03f);

	BasicAttackCooldown -= RageAttkSpeedBuff;
	CharacterMovement->MaxWalkSpeed += RageMoveSpeedBuff;

	RageTimer = RageDuration;
}

//Mayhem Iron Armor
void AMayhemCharacter::SkillThree()
{
	IronArmorHealthBuff = (float)MaxHealth * 0.25f + Strength * 0.75f;
	IronArmorRegenBuff = (float)HealthRegeneration * 0.25f + Strength + 0.03f;

	IronArmorTimer = IronArmorDuration;

	MaxHealth += IronArmorHealthBuff;
	Health += IronArmorHealthBuff;

	HealthRegeneration += IronArmorRegenBuff;
	IronArmorSound->Play();
}

void AMayhemCharacter::SkillOneAim()
{
	AimBarMaterial = UMaterialInstanceDynamic::Create(AimBarMatStatic, this);
	//AimBar->SetWorldLocation(FVector(0, 0, -50));
	AimSphere->SetRelativeLocation(FVector(0, 0, 60));
	AimSphere->SetWorldScale3D(FVector(4.5f, 4.5f, 0.1));
	AimSphere->CastShadow = false;
	AimSphere->Materials.Add(AimBarMaterial);
	Cast<UMaterialInstanceDynamic>(AimSphere->Materials[0])->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(0.75f, 0.01f, 0.01f));
	Cast<UMaterialInstanceDynamic>(AimSphere->Materials[0])->SetScalarParameterValue(FName(TEXT("Health percentage")), 1.0f);
	Cast<UMaterialInstanceDynamic>(AimSphere->Materials[0])->SetScalarParameterValue(FName(TEXT("Opacity")), 0.15f);

	AimSphere->SetHiddenInGame(false);

	AimBar->SetHiddenInGame(true);
}

void AMayhemCharacter::SkillTwoAim()
{


	AimBar->SetHiddenInGame(true);
}

void AMayhemCharacter::SkillThreeAim()
{

	AimBar->SetHiddenInGame(true);
}




void AMayhemCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMayhemCharacter, StunLength);
	
	DOREPLIFETIME(AMayhemCharacter, IronArmorTimer);
	DOREPLIFETIME(AMayhemCharacter, RageTimer);
}
