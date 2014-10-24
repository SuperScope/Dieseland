// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "DieselandGameMode.generated.h"

UCLASS(minimalapi)
class ADieselandGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()
    
    //Array for the Death Tiles
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    TArray<UClass*> DeathTileClassArray;

    //Array for the Death Tile Locations
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    TArray<FVector> LocationArray;

	//Array for the Death Tiles
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	TArray<AActor*> DeathTileArray;

	// Timer to keep track of the duration of the match
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	float GameTimer;

	UFUNCTION(Reliable, Server, WithValidation, Category = Gameplay)
	void StartGame();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void RespawnTile(FVector SpawnLocation);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	AActor* SpawnLightArray(UClass* ArrayClass, FVector SpawnLocation);

	UFUNCTION(Reliable, Server, WithValidation, Category = Gameplay)
	void EndGame();

	UClass* MayhemClass;
	UClass* EngletonClass;
	UClass* StrykerClass;

	uint8 PlayersSpawned;

	virtual void ReceiveBeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual APawn* SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot) override;
};
