

#pragma once

#include "GameFramework/Character.h"
#include "UnrealNetwork.h"
#include "DieselandEnemyBot.generated.h"


/**
 * 
 */
UCLASS()
class DIESELAND_API ADieselandEnemyBot : public ACharacter
{
	GENERATED_UCLASS_BODY()

	// Numerical display of health value
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interface)
	TSubobjectPtr<class UTextRenderComponent> HealthLabel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interface)
	TSubobjectPtr<class UMaterialBillboardComponent> HealthBar;
	
	//this is for the pathfinding variables, specifically the Behavior Tree
	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree* BottBehavior;

	// Collider used to detect melee range
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UCapsuleComponent> MeleeCollision;

	// Collider used to detect attack zone radius
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UCapsuleComponent> AttackZoneCollision;

	// Collider used to detect attack zone radius
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
		TSubobjectPtr<class UCapsuleComponent> ProjectileZoneCollision;

	//Called to subtract and/or add health to the enemy
	UFUNCTION(BlueprintCallable, Category = Gameplay)
		void EditHealth(int32 Amt, AActor* Target);

	//public health value of this character
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 Health;

	// Value 0.0f - 1.0f used for health bar display
	float HealthPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		UMaterialInstanceDynamic* HealthBarMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Trap)
		float LingerTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Trap)
		float StunTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Trap)
	float StunLength;


	//Damage done to actors from fire traps upon exitting them
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Trap)
	int32 LingerDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Trap)
	int32 LingerCount;

	//set for the AIs aggresion
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool isAggressive;

	//set for the AIs attack type
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool IsMelee;

	//set for the AIs attack type
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool IsQueen;

	//set for the AIs attack type
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool IsKing;

	//set for the AIs attack type
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool IsWalker;

	//set for the AIs attack type
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool FirstRun;

	// Damage amount for basic attacks
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		int32 BaseAttackDamage;

	// Damage amount for basic attacks
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		int32 BasicAttackDamage;

	// Public health value of this character
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 MaxHealth;

	// Health Regeneration
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float HealthRegeneration;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float SpawnTimer;

	// Timer for health regen.
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float HealthRegenTimer;

	// The range of this character's melee attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		float MeleeRange;

	// The range of this character's attack zone radius
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		float AttackZone;

	// The range of this character's attack zone radius
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		float ProjectileZone;

	// The Cooldown for the basic attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		float BasicAttackCooldown;

	// Mesh attached to the torso socket which is used to show attack direction - invisible by default
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
		TSubobjectPtr<class UStaticMeshComponent> AimMesh;

	// Mesh attached to the player
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
		TSubobjectPtr<class UStaticMeshComponent> SkeletalMesh;

	//combat functions
	UFUNCTION(BlueprintCallable, Category = Combat)
		void MeleeAttack();

	//combat functions
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = Combat)
		void RangedAttack();

	UFUNCTION(BlueprintCallable, Category = Combat)
	void OnZoneEnter();

	UFUNCTION(BlueprintCallable, Category = Combat)
	void OnProjectileZoneEnter();

	//One forumla > many <3
	UFUNCTION(BlueprintCallable, Category = Gameplay)
		void ResetStats();

	// Is this character currently poisoned.
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool IsPoisoned;

	// How long has this character been poisoned.
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		float PoisonTimer;


	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	float BasicAttackTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		TArray<AActor*> ActorsInMeleeRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		TArray<AActor*> ActorsInZoneRange;

	//check to see if the basic attack is active
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	bool BasicAttackActive;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = Status)
		TArray<FString> StatusEffects;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = Status)
		float StunRemaining;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = Status)
		float SlowRemaining;


	//this is the function used to activate particles across the server
	UFUNCTION(Reliable, NetMulticast, WithValidation)
		void ServerActivateParticle(UParticleSystem* Particle);

	//this grabs the particle system
	UPROPERTY(Replicated, Category = Visual, BlueprintReadOnly, VisibleAnywhere)
		TSubobjectPtr<class UParticleSystemComponent> ParticleSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> WalkerCannon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> KingSwing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> SwordSwing;



protected:

	UClass* ScrapClass;

	UMaterial* HealthBarMatStatic;
	UMaterial* HealthBarBackMatStatic;

	virtual void ReceiveBeginPlay() override;

	//timer for updating UI
	virtual void Tick(float DeltaSeconds) override;
};
