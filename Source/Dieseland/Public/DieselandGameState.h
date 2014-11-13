

#pragma once

#include "GameFramework/GameState.h"
#include "DieselandGameState.generated.h"

class ADieselandPlayerController;

/**
 * 
 */
UCLASS()
class DIESELAND_API ADieselandGameState : public AGameState
{
	GENERATED_UCLASS_BODY()

	// Array of all of the players
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	TArray<ADieselandPlayerController*> Players;

	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	int32 WinningScore;
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	int32 KillGoal;
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	int32 WinningTeam;
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	float GameDuration;

	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	TArray<int32> TeamScores;
	
	UFUNCTION(BlueprintCallable, Category = Score)
	void CalculateScore();

	UFUNCTION(BlueprintCallable, Category = Score)
	void GameTimerEnded();

	virtual void ReceiveBeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
