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

	// Collider used to detect pulse range
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class USphereComponent> PulseCollision;

	// Called to subtract and/or add health to the target player
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void EditHealth(int32 Amt, AActor* Target);

	// Public health value of this character
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 Health;

	// Damage amount for the basic attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	int32 BasicAttackDamage;

	// Does this character use melee for it's basic attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	bool IsMelee;

	// The range of this character's melee attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float MeleeRange;

	// The range of this character's ranged attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float RangedRange;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float BlinkDist;

	// The Cooldown for the basic attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float BasicAttackCooldown;

	// The Cooldown for skill 1
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float SkillOneCooldown;

	// The Cooldown for skill 2
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float SkillTwoCooldown;

	// The Cooldown for skill 3
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float SkillThreeCooldown;

	// Replicated Rotation of the torso for aiming purposess
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_AimRotation, EditAnywhere, BlueprintReadWrite, Category = Network)
	FRotator AimRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	TArray<AActor*> ActorsInMeleeRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	TArray<AActor*> ActorsInPulseRange;


	//CORE ATTRIBUTES BEGINS HERE
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 Strength;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 Constitution;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 Dexterity;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 Intelligence;

	///TODO : PARAMETERS MUST MATCH FORMULAS
	UFUNCTION(BlueprintCallable, Category = Gameplay)
		void calculateSpeed(int32 coreAmt, int32 secondaryAmt, AActor* Target);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
		void calculateAttkSpeed(int32 coreAmt, int32 secondaryAmt, AActor* Target);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
		void calculateArmor(int32 coreAmt, int32 secondaryAmt, AActor* Target);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
		void calculateDamage(int32 coreAmt, int32 secondaryAmt, AActor* Target);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
		void calculateHealth(int32 coreAmt, int32 secondaryAmt, AActor* Target);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
		void calculateAbilityDamage(int32 coreAmt, int32 secondaryAmt, AActor* Target);

	//CORE ATTRIBUTES ENDS HERE

	// Combat functions
	UFUNCTION(BlueprintCallable, Category = Combat)
	void RangedAttack();

	UFUNCTION(BlueprintCallable, Category = Combat)
	void MeleeAttack();

	UFUNCTION(BlueprintCallable, Category = Combat)
	void SkillOne();

	UFUNCTION(BlueprintCallable, Category = Combat)
	void SkillTwo();

	UFUNCTION(BlueprintCallable, Category = Combat)
	void SkillThree();

	// Called when AimRotation is replicated
	UFUNCTION()
	void OnRep_AimRotation();

	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	float BasicAttackTimer;
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
	void ServerActivateProjectile();

protected:
	virtual void Tick(float DeltaSeconds) override;

};

