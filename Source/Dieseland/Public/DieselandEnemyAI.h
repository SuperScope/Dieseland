

#pragma once

#include "AIController.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
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

	virtual void Possess(class APawn *InPawn);

	void SetEnemy(class APawn * InPawn);

	UFUNCTION(BlueprintCallable, Category = Behavior)
	void SearchForEnemy();

	protected:
	uint8 EnemyKeyID;
	uint8 EnemyKeyLocationID;
};
