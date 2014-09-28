

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
	TSubobjectPtr<class UBoxComponent> CannonZoneCollision;

	// The range of the cannon zone collider
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float CannonRange;

	// Damage amount for basic attacks
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float CannonDamagePerSecond;


	UFUNCTION(BlueprintCallable, Category = Combat)
		void CannonAttack();
};
