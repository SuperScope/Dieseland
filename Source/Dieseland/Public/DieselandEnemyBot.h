

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

	// Temporary display of health value
	// TODO: Remove when UI is ready
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interface)
	TSubobjectPtr<class UTextRenderComponent> PlayerLabel;

	
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Trap)
	float LingerTimer;

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

	// Damage amount for basic attacks
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		int32 BasicAttackDamage;

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


	UFUNCTION(BlueprintCallable, Category = Combat)
	void OnZoneExit();

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

protected:

	//timer for updating UI
	virtual void Tick(float DeltaSeconds) override;
};
