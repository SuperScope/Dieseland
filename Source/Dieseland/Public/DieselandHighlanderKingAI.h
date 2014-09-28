

#pragma once

#include "DieselandEnemyAI.h"
#include "DieselandHighlanderKingAI.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ADieselandHighlanderKingAI : public ADieselandEnemyAI
{
	GENERATED_UCLASS_BODY()


	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCannonAttack();



	// Begin tick timer
	//virtual void Tick(float DeltaTime) override;

	
};
