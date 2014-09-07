

#pragma once

#include "GameFramework/Character.h"
#include "DieselandEnemy.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ADieselandEnemy : public ACharacter
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree* EnemyBehavior;
	
};
