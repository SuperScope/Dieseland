

#pragma once

#include "GameFramework/GameState.h"
#include "DieselandGameState.generated.h"

class ADieselandCharacter;

/**
 * 
 */
UCLASS()
class DIESELAND_API ADieselandGameState : public AGameState
{
	GENERATED_UCLASS_BODY()

	// Array of all of the players
	TArray<ADieselandCharacter*> Players;

	int32 WinningScore

	ADieselandCharacter* WinningPlayer;
	
	virtual void ReceiveBeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
