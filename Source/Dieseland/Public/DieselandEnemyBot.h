

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

	//Called to subtract and/or add health to the enemy
	UFUNCTION(BlueprintCallable, Category = Gameplay)
		void EditHealth(int32 Amt, AActor* Target);


	//public health value of this character
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 Health;

	//set for the AIs aggresion
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool isAggressive;

	// Damage amount for basic attacks
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		int32 BasicAttackDamage;

	// The range of this character's melee attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		float MeleeRange;

	// The range of this character's attack zone radius
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float AttackZone;

	// The Cooldown for the basic attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		float BasicAttackCooldown;

	// Mesh attached to the torso socket which is used to show attack direction - invisible by default
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
		TSubobjectPtr<class UStaticMeshComponent> AimMesh;

	//combat functions
	UFUNCTION(BlueprintCallable, Category = Combat)
		void MeleeAttack();

	UFUNCTION(BlueprintCallable, Category = Combat)
	void OnZoneEnter();

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

protected:

	//timer for updating UI
	virtual void Tick(float DeltaSeconds) override;
};
