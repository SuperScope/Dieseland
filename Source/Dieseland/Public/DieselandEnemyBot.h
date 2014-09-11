

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

	//Called to subtract and/or add health to the enemy
	UFUNCTION(BlueprintCallable, Category = Gameplay)
		void EditHealth(int32 Amt, AActor* Target);

	//public health value of this character
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 Health;

	// Damage amount for basic attacks
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		int32 BasicAttackDamage;

	// The range of this character's melee attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		float MeleeRange;

	// The Cooldown for the basic attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		float BasicAttackCooldown;

	// Mesh attached to the torso socket which is used to show attack direction - invisible by default
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
		TSubobjectPtr<class UStaticMeshComponent> AimMesh;

	//combat functions
	UFUNCTION(BlueprintCallable, Category = Combat)
		void MeleeAttack();

	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	float BasicAttackTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		TArray<AActor*> ActorsInMeleeRange;

	//check to see if the basic attack is active
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	bool BasicAttackActive;

protected:

	//timer for updating UI
	virtual void Tick(float DeltaSeconds) override;
};
