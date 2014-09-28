

#pragma once

#include "AIController.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "UnrealNetwork.h"
#include "DieselandEnemyAI.generated.h"



/**
 * 
 */
UCLASS()
class DIESELAND_API ADieselandEnemyAI : public AAIController
{
	GENERATED_UCLASS_BODY()

	
	//all of the functions for AI search and move behavior
	UPROPERTY(transient)
	TSubobjectPtr<class UBlackboardComponent> BlackboardComp;
	UPROPERTY(transient)
	TSubobjectPtr<class UBehaviorTreeComponent> BehaviorComp;

	
	

	//function used to gain control of the enemy
	virtual void Possess(class APawn *InPawn);

	//function used to set the enemy target for the AI
	void SetEnemy(class APawn * InPawn);

	//function used to find an enemy target
	UFUNCTION(BlueprintCallable, Category = Behavior)
		void SearchForEnemy();



	// Called by the server to edit client's health
	UFUNCTION(reliable, Server, WithValidation)
		void ServerEditHealth(int32 Amt, AActor* Target);



	//here we update our  cooldown timers
	void UpdateCooldownTimers(float DeltaSeconds);
	
	//here we start our melee attack
	void MeleeAttack(float DeltaSeconds);

	//search for spawn location
	UFUNCTION(BlueprintCallable, Category = Behavior)
		void SearchForSpawnLocation();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerMeleeAttack();

	uint8 EnemyKeyID;
	uint8 EnemyKeyLocationID;
	uint8 SpawnLocationID;
	FVector SpawnLocation;

	// Begin tick timer
	virtual void Tick(float DeltaTime) override;
	protected:
		virtual void BeginPlay() override;

};
