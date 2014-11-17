// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "FoxCharacter.h"
#include "FoxPenetrationRound.h"
#include "FoxCharm.h"
#include "FoxSmokeGrenadeProjectile.h"
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

	//adjustments for damage
	BasicAttackDamage = BaseDamage + (Strength * 1.5f) + (Dexterity * .5f) + (Intelligence * .5f);
	//adjusments for attackspeed
	BasicAttackCooldown = BaseCooldownSpeed / (1 + (Dexterity / 50));

	//adjustments for movement Speed
	MoveSpeed = BaseMoveSpeed + (Dexterity * 3);
	this->CharacterMovement->MaxWalkSpeed = MoveSpeed;

	//base cooldowns
	BaseSkillOneCooldown = 1.0f; //23
	BaseSkillTwoCooldown = 1.0f; //9
	BaseSkillThreeCooldown = 1.0f; //13

	//adjustments for cooldowns
	SkillOneCooldown = BaseSkillOneCooldown / (1 + Intelligence / 100);
	SkillTwoCooldown = BaseSkillTwoCooldown / (1 + Intelligence / 100);
	SkillThreeCooldown = BaseSkillThreeCooldown / (1 + Intelligence / 100);


	TauntCooldown = 4.5f;
	LaughCooldown = 2.5f;
	CommentCooldown = 4.0f;

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

	SmokeBombSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Smoke Bomb Sound"));
	SmokeBombSound->AttachParent = RootComponent;
	SmokeBombSound->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> BasicAttackParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/MovingBulletTest_WIP.MovingBulletTest_WIP'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SkillOneParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_FoxPenetratorShot.Unreal_Particle_FoxPenetratorShot'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SkillTwoParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_FoxCharm_WIP3.Unreal_Particle_FoxCharm_WIP3'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SkillThreeParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_Bullet1.Unreal_Particle_Bullet1'"));


	this->BasicAttackParticle = BasicAttackParticleAsset.Object;
	this->SkillOneParticle = SkillOneParticleAsset.Object;
	this->SkillTwoParticle = SkillTwoParticleAsset.Object;
	this->SkillThreeParticle = SkillThreeParticleAsset.Object;

	bReplicates = true;
	bReplicateMovement = true;

	//here I set melee to false so that Fox only uses ranged attacks
	IsMelee = false;



}

// To do fox penetration round
void AFoxCharacter::SkillOne()
{
	ServerActivateParticle(SkillOneParticle);

	UWorld* const World = GetWorld();
	if (World)
	{
		UltimateVoice->Play();
		PenetrationRound->Play();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Cast<ADieselandPlayerController>(this->Controller);
		SpawnParams.Instigator = Instigator;

		FRotator ProjectileRotation = Mesh->GetSocketRotation(FName(TEXT("AimSocket")));

		ProjectileRotation = FRotator(ProjectileRotation.Pitch, ProjectileRotation.Yaw + 90.0f, ProjectileRotation.Roll);

		// spawn the projectile at the muzzle
		AFoxPenetrationRound* const Projectile = World->SpawnActor<AFoxPenetrationRound>(AFoxPenetrationRound::StaticClass(), Mesh->GetSocketLocation(FName(TEXT("AimSocket"))), ProjectileRotation, SpawnParams);
		if (Projectile)
		{
			Projectile->ProjectileDamage = 150 + (Dexterity * 5);
			// Start the particle effect
			//Projectile->ServerActivateProjectile();

			// Add the character's velocity to the projectile
			Projectile->ProjectileMovement->SetVelocityInLocalSpace((Projectile->ProjectileMovement->InitialSpeed  * ProjectileRotation.Vector()) + (GetVelocity().GetAbs() * Mesh->GetSocketRotation(FName(TEXT("AimSocket"))).GetNormalized().Vector()));
			OnSkillOne();
		}
	}
}

//To do fox charm
void AFoxCharacter::SkillTwo()
{
	ServerActivateParticle(SkillTwoParticle);

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
			//Projectile->ServerActivateProjectile();

			// Add the character's velocity to the projectile
			Projectile->ProjectileMovement->SetVelocityInLocalSpace((Projectile->ProjectileMovement->InitialSpeed  * ProjectileRotation.Vector()) + (GetVelocity().GetAbs() * Mesh->GetSocketRotation(FName(TEXT("AimSocket"))).GetNormalized().Vector()));
			OnSkillTwo();
		}
	}

}

//To do fox smoke bomb
void AFoxCharacter::SkillThree()
{
	ServerActivateParticle(SkillThreeParticle);

	UWorld* const World = GetWorld();
	if (World)
	{
		SmokeBombSound->Play();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Cast<ADieselandPlayerController>(this->Controller);
		SpawnParams.Instigator = Instigator;

		FRotator ProjectileRotation = Mesh->GetSocketRotation(FName(TEXT("AimSocket")));

		ProjectileRotation = FRotator(ProjectileRotation.Pitch, ProjectileRotation.Yaw + 90.0f, ProjectileRotation.Roll);

		// spawn the projectile at the muzzle
		AFoxSmokeGrenadeProjectile* const Projectile = World->SpawnActor<AFoxSmokeGrenadeProjectile>(AFoxSmokeGrenadeProjectile::StaticClass(), Mesh->GetSocketLocation(FName(TEXT("AimSocket"))), ProjectileRotation, SpawnParams);
		if (Projectile)
		{
			Projectile->ProjectileDamage = 0;
			// Start the particle effect
			//Projectile->ServerActivateProjectile();

			// Add the character's velocity to the projectile
			Projectile->ProjectileMovement->SetVelocityInLocalSpace((Projectile->ProjectileMovement->InitialSpeed  * ProjectileRotation.Vector()) + (GetVelocity().GetAbs() * Mesh->GetSocketRotation(FName(TEXT("AimSocket"))).GetNormalized().Vector()));
			OnSkillThree();
		}
	}
}

void AFoxCharacter::RangedAttack()
{
	ServerActivateParticle(BasicAttackParticle);

	UWorld* const World = GetWorld();
	if (World)
	{
		RifleRound->Play();
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
			//Projectile->ServerActivateProjectile();

			// Add the character's velocity to the projectile
			Projectile->ProjectileMovement->SetVelocityInLocalSpace((Projectile->ProjectileMovement->InitialSpeed  * ProjectileRotation.Vector()) + (GetVelocity().GetAbs() * Mesh->GetSocketRotation(FName(TEXT("AimSocket"))).GetNormalized().Vector()));
			OnBasicAttack();
		}
	}
}

//Fox basic attack
void AFoxCharacter::MeleeAttack()
{
	//fox doesn't use melee
}
void AFoxCharacter::SkillOneAim()
{
	Cast<UMaterialInstanceDynamic>(AimBar->Materials[0])->SetVectorParameterValue(FName(TEXT("Get Jinxed!")), FVector(0.0f, 1.0f, 0.0f));

	AimBar->SetHiddenInGame(false);
}

void AFoxCharacter::SkillTwoAim()
{
	Cast<UMaterialInstanceDynamic>(AimBar->Materials[0])->SetVectorParameterValue(FName(TEXT("Get Jinxed!")), FVector(0.0f, 1.0f, 0.0f));

	AimBar->SetHiddenInGame(false);
}

void AFoxCharacter::SkillThreeAim()
{
	Cast<UMaterialInstanceDynamic>(AimBar->Materials[0])->SetVectorParameterValue(FName(TEXT("Get Jinxed!")), FVector(0.0f, 1.0f, 0.0f));

	AimBar->SetHiddenInGame(false);
}


