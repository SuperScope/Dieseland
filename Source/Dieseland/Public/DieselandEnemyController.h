

#pragma once

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "DieselandEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ADieselandEnemyController : public AAIController
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(transient)
	TSubobjectPtr<class UBlackboardComponent> BlackboardComp;

	UPROPERTY(transient)
	TSubobjectPtr<class UBehaviorTreeComponent> BehaviorComp;

	virtual void Possess(class APawn * InPawn);

	void SetEnemy(class APawn * InPawn);

	UFUNCTION(BlueprintCallable, Category = Behavior)
	void SearchForEnemy();
	

	protected:
		//both of these variables requires uint8
		uint8 EnemyKeyID;
		uint8 EnemyLocationID;
};
