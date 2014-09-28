

#pragma once


#include "UnrealNetwork.h"
#include "DieselandEnemyBot.h"
#include "DieselandHighlanderKingBot.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ADieselandHighlanderKingBot : public ADieselandEnemyBot
{
	GENERATED_UCLASS_BODY()
	// Collider used to detect attack zone radius
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UCapsuleComponent> CannonZoneCollision;

};