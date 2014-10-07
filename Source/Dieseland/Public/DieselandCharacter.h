// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "DieselandCharacter.generated.h"

UCLASS(Blueprintable)
class ADieselandCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()
public:

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class UCameraComponent> TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class USpringArmComponent> CameraBoom;

	// Temporary display of health value
	// TODO: Remove when UI is ready
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interface)
	TSubobjectPtr<class UTextRenderComponent> PlayerLabel;

	// Mesh attached to the torso socket which is used to show attack direction - invisible by default
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
		TSubobjectPtr<class UStaticMeshComponent> AimMesh;

	// Collider used to detect melee range
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UCapsuleComponent> MeleeCollision;

	// Collider used to detect AoE range
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class USphereComponent> AOECollision;

	// Called to subtract and/or add health to the target player
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void EditHealth(int32 Amt, AActor* Target);

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 Kills;

	// Public health value of this character
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 Health;

	// Public health value of this character
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 MaxHealth;

	// Health Regeneration
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float HealthRegeneration;

	// Public armor value of this character
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 Scrap;

	// The movement speed of a character
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float MoveSpeed;

	// The movement speed of a character
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float BaseMoveSpeed;

	// Damage amount for the basic attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	int32 BasicAttackDamage;


	// Setting base stats
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	int32 BaseDamage;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float BaseCooldownSpeed;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	int32 BaseHealth;
	// Current ammo for basic ranged attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	int32 BasicAttackAmmo;

	//set the character level
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	int32 CharacterLevel;

	// Max ammo size
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	int32 BasicAttackMag;

	// Does this character use melee for it's basic attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	bool IsMelee;

	// The range of this character's Melee Attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float MeleeRange;

	// The range of this character's ranged attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float RangedRange;

	// The Cooldown for the basic attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float BasicAttackCooldown;

	// The reload time for the basic ranged attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float BasicAttackReloadSpeed;

	// The Cooldown for skill 1
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float SkillOneCooldown;

	// The Cooldown for skill 2
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float SkillTwoCooldown;

	// The Cooldown for skill 3
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float SkillThreeCooldown;


	// The Base Cooldown for skill 1
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		float BaseSkillOneCooldown;
	// The Base Cooldown for skill 2
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		float BaseSkillTwoCooldown;
	// The Base Cooldown for skill 3
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		float BaseSkillThreeCooldown;


	//FireTrap Damage Timer
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Trap)
	float LingerTimer;

	//FireTrap Damage Coolown
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Trap)
	float LingerCoolDown;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Trap)
	int32 LingerDamage;

	// Replicated Rotation of the torso for aiming purposess
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_AimRotation, EditAnywhere, BlueprintReadWrite, Category = Network)
	FRotator AimRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	TArray<AActor*> ActorsInMeleeRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	TArray<AActor*> ActorsInAOERange;

	//CORE ATTRIBUTES BEGINS HERE

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 Strength;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 Constitution;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 Dexterity;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 Intelligence;


	//One forumla > many <3
	UFUNCTION(BlueprintCallable, Reliable, Server, WithValidation, Category = Gameplay)
	void CalculateStats();

	//CORE ATTRIBUTES ENDS HERE

	// Combat functions
	UFUNCTION(BlueprintCallable, Category = Combat)
	virtual void RangedAttack();

	UFUNCTION(BlueprintCallable, Category = Combat)
	virtual void MeleeAttack();

	UFUNCTION(BlueprintCallable, Category = Combat)
	virtual void SkillOne();

	UFUNCTION(BlueprintCallable, Category = Combat)
	virtual void SkillTwo();

	UFUNCTION(BlueprintCallable, Category = Combat)
	virtual void SkillThree();

	// Called when AimRotation is replicated
	UFUNCTION()
	void OnRep_AimRotation();

	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	float BasicAttackTimer;
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	float BasicAttackReloadTimer;
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	float SkillOneTimer;
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	float SkillTwoTimer;
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	float SkillThreeTimer;

	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	bool BasicAttackActive;

	UPROPERTY(Replicated, Category = Visual, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UParticleSystemComponent> ParticleSystem;

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void ServerActivateParticle(UParticleSystem* Particle);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerDamageEnemy(int32 Amt, AActor* Target);

	UPROPERTY(Replicated, Category = Combat, BlueprintReadWrite, EditAnywhere)
	UParticleSystem* SkillThreeParticle;

	UPROPERTY(Replicated, Category = Combat, BlueprintReadWrite, EditAnywhere)
	UParticleSystem* SkillTwoParticle;

	UPROPERTY(Replicated, Category = Combat, BlueprintReadWrite, EditAnywhere)
	UParticleSystem* SkillOneParticle;

	UPROPERTY(Replicated, Category = Combat, BlueprintReadWrite, EditAnywhere)
	UParticleSystem* BasicAttackParticle;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = Status)
	TArray<FString> StatusEffects;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = Status)
	float StunRemaining;

protected:
	virtual void Tick(float DeltaSeconds) override;
	UClass* ScrapClass;

};

