

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
	
	//here we update our  cooldown timers
	void UpdateCooldownTimers(float DeltaSeconds);


	// Begin tick timer
	virtual void Tick(float DeltaTime) override;

	
};
