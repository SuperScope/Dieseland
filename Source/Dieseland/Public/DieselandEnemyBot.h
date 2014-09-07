

#pragma once

#include "GameFramework/Character.h"
#include "DieselandEnemyBot.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ADieselandEnemyBot : public ACharacter
{
	GENERATED_UCLASS_BODY()

	
	//this is for the pathfinding variables, specifically the Behavior Tree
	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree* BottBehavior;
};
