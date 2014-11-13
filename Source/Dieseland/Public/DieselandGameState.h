

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
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	TArray<ADieselandCharacter*> Players;

	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	int32 WinningScore;
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	int32 KillGoal;
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	int32 WinningTeam;
	
	virtual void ReceiveBeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
