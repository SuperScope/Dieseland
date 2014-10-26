// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "FoxCharacter.h"
#include "FoxPenetrationRound.h"
#include "FoxCharm.h"
#include "FoxBasicAttack.h"
#include "DieselandCharacter.h"
#include "DieselandPlayerController.h"
#include "UnrealNetwork.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"



AFoxCharacter::AFoxCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//here I set her base values
	BaseMoveSpeed = 390;
	BaseHealth = 390;
	BaseDamage = 51;
	BaseCooldownSpeed = 2;
	//here I set her base stats
	Strength = 12;
	Intelligence = 14;
	Dexterity = 16;
	Constitution = 10;

	//adjustments for base stats
	MaxHealth = BaseHealth + (Constitution * 20.0f) + (Strength * 3.0f);
	Health = MaxHealth;
	HealthRegeneration = 1.0f + (Constitution / 10.0f) + (Strength / 20.0f);
	//show those adjustments
	PlayerLabel->SetText(FString::FromInt(Health));
	//adjustments for damage
	BasicAttackDamage = BaseDamage + (Strength * 1.5f) + (Dexterity * .5f) + (Intelligence * .5f);
	//adjusments for attackspeed
	BasicAttackCooldown = BaseCooldownSpeed / (1 + (Dexterity / 50));

	//adjustments for movement Speed
	MoveSpeed = BaseMoveSpeed + (Dexterity * 3);
	this->CharacterMovement->MaxWalkSpeed = MoveSpeed;

	//base cooldowns
	BaseSkillOneCooldown = 1.0f;
	BaseSkillTwoCooldown = 1.0f;
	BaseSkillThreeCooldown = 1.0f;

	//adjustments for cooldowns
	SkillOneCooldown = BaseSkillOneCooldown / (1 + Intelligence / 100);
	SkillTwoCooldown = BaseSkillTwoCooldown / (1 + Intelligence / 100);
	SkillThreeCooldown = BaseSkillThreeCooldown / (1 + Intelligence / 100);


	TauntCooldown = 1.0f;
	LaughCooldown = 1.0f;
	CommentCooldown = 1.0f;

	//setting all sounds
	PenetrationRound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Penetration Round Sound"));
	PenetrationRound->AttachParent = RootComponent;
	PenetrationRound->bAutoActivate = false;

	UltimateVoice = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Ultimate Voice Over"));
	UltimateVoice->AttachParent = RootComponent;
	UltimateVoice->bAutoActivate = false;

	RifleRound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Rifle Round Sound"));
	RifleRound->AttachParent = RootComponent;
	RifleRound->bAutoActivate = false;

	CharmSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Charm Sound"));
	CharmSound->AttachParent = RootComponent;
	CharmSound->bAutoActivate = false;


	bReplicates = true;
	bReplicateMovement = true;

	//here I set melee to false so that Fox only uses ranged attacks
	IsMelee = false;



}

// To do fox penetration round
void AFoxCharacter::SkillOne()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Cast<ADieselandPlayerController>(this->Controller);
		SpawnParams.Instigator = Instigator;

		FRotator ProjectileRotation = Mesh->GetSocketRotation(FName(TEXT("AimSocket")));

		ProjectileRotation = FRotator(ProjectileRotation.Pitch, ProjectileRotation.Yaw + 90.0f, ProjectileRotation.Roll);

		// spawn the projectile at the muzzle
		AFoxPenetrationRound* const Projectile = World->SpawnActor<AFoxPenetrationRound>(AFoxPenetrationRound::StaticClass(), Mesh->GetSocketLocation(FName(TEXT("AimSocket"))), ProjectileRotation, SpawnParams);
		if (Projectile)
		{
			Projectile->ProjectileDamage = 175 + (Dexterity * 6);
			// Start the particle effect
			Projectile->ServerActivateProjectile();

			// Add the character's velocity to the projectile
			Projectile->ProjectileMovement->SetVelocityInLocalSpace((Projectile->ProjectileMovement->InitialSpeed  * ProjectileRotation.Vector()) + (GetVelocity().GetAbs() * Mesh->GetSocketRotation(FName(TEXT("AimSocket"))).GetNormalized().Vector()));
		}
	}
}

//To do fox charm
void AFoxCharacter::SkillTwo()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Cast<ADieselandPlayerController>(this->Controller);
		SpawnParams.Instigator = Instigator;

		FRotator ProjectileRotation = Mesh->GetSocketRotation(FName(TEXT("AimSocket")));

		ProjectileRotation = FRotator(ProjectileRotation.Pitch, ProjectileRotation.Yaw + 90.0f, ProjectileRotation.Roll);

		// spawn the projectile at the muzzle
		AFoxCharm* const Projectile = World->SpawnActor<AFoxCharm>(AFoxCharm::StaticClass(), Mesh->GetSocketLocation(FName(TEXT("AimSocket"))), ProjectileRotation, SpawnParams);
		if (Projectile)
		{
			Projectile->ProjectileDamage = 0;
			// Start the particle effect
			Projectile->ServerActivateProjectile();

			// Add the character's velocity to the projectile
			Projectile->ProjectileMovement->SetVelocityInLocalSpace((Projectile->ProjectileMovement->InitialSpeed  * ProjectileRotation.Vector()) + (GetVelocity().GetAbs() * Mesh->GetSocketRotation(FName(TEXT("AimSocket"))).GetNormalized().Vector()));
		}
	}

}

//To do fox smoke bomb
void AFoxCharacter::SkillThree()
{
	
}

void AFoxCharacter::RangedAttack()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Cast<ADieselandPlayerController>(this->Controller);
		SpawnParams.Instigator = Instigator;

		FRotator ProjectileRotation = Mesh->GetSocketRotation(FName(TEXT("AimSocket")));

		ProjectileRotation = FRotator(ProjectileRotation.Pitch, ProjectileRotation.Yaw + 90.0f, ProjectileRotation.Roll);

		// spawn the projectile at the muzzle
		AFoxBasicAttack* const Projectile = World->SpawnActor<AFoxBasicAttack>(AFoxBasicAttack::StaticClass(), Mesh->GetSocketLocation(FName(TEXT("AimSocket"))), ProjectileRotation, SpawnParams);
		if (Projectile)
		{
			Projectile->ProjectileDamage = BasicAttackDamage;
			// Start the particle effect
			Projectile->ServerActivateProjectile();

			// Add the character's velocity to the projectile
			Projectile->ProjectileMovement->SetVelocityInLocalSpace((Projectile->ProjectileMovement->InitialSpeed  * ProjectileRotation.Vector()) + (GetVelocity().GetAbs() * Mesh->GetSocketRotation(FName(TEXT("AimSocket"))).GetNormalized().Vector()));
		}
	}
}

//Fox basic attack
void AFoxCharacter::MeleeAttack()
{
	//fox doesn't use melee
}
