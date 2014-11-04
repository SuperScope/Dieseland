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
    
    //Array for the Boss Spawn Locations
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    TArray<FVector> BossSpawnArray;

	//Array for the Death Tiles
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	TArray<AActor*> DeathTileArray;
    
    //Array for the Death Tiles
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	TArray<AActor*> GateArray;

	// Timer to keep track of the duration of the match
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	float GameTimer;
    
    UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	float BossTimer;
    
    UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	bool Gate1;
    
    UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	bool Gate2;
    
    UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	bool Gate3;

	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = Gameplay)
	void StartGame();
    
    UFUNCTION(BlueprintCallable, Category = Gameplay)
    void SpawnBoss();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void RespawnTile(FVector SpawnLocation);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	AActor* SpawnLightArray(UClass* ArrayClass, FVector SpawnLocation);

	UFUNCTION(Reliable, Server, WithValidation, Category = Gameplay)
	void EndGame();

	UClass* MayhemClass;
	UClass* EngletonClass;
	UClass* StrykerClass;
	UClass* FoxClass;
    UClass* HighlanderKing;
    
    bool canSpawn;

	uint8 PlayersSpawned;

	virtual void ReceiveBeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual APawn* SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot) override;
};
