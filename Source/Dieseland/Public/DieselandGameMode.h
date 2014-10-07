// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "DieselandGameMode.generated.h"

UCLASS(minimalapi)
class ADieselandGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()
    
    //Array for the Death Tiles
    TArray<UClass*> DeathTileArray;

    //Array for the Death Tile Locations
    TArray<FVector> LocationArray;

	// Timer to keep track of the duration of the match
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	float GameTimer;

	UFUNCTION(Reliable, Server, WithValidation, Category = Gameplay)
	void EndGame();

	UClass* MayhemClass;
	UClass* EngletonClass;

	uint8 PlayersSpawned;

	virtual void Tick(float DeltaSeconds) override;
	virtual APawn* SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot) override;
};
