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

	//Array for the Spawn Locations
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	TArray<APlayerStart*> SpawnArray;

	//Array for the starting Spawn Locations
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	TArray<APlayerStart*> StartSpawnArray;
    
    //Array for the Boss Spawn Locations
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    TArray<FVector> BossSpawnArray;

	//Array for the Death Tiles
	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	TArray<AActor*> DeathTileArray;
    
    // Timer used to determined when boss should be spawned
    UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	float BossTimer;
    
    //Random number used to determine which boss to spawn
    UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	int32 RandomBossIndex;
    
    //Random numbed used to determine boss tile
    UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	int32 RandomSpawnIndex;
    
    //Used to make sure boss can spawn, eliminates multiple boss spawns at same time
    UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    bool CanSpawn;
    
    // Used to start timer when game actually starts
    UPROPERTY(Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    bool StartBossTimer;

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void StartGame();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	APlayerStart* PickSpawn();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void RespawnTile(FVector SpawnLocation);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	AActor* SpawnLightArray(UClass* ArrayClass, FVector SpawnLocation);

	UFUNCTION(Reliable, NetMulticast, WithValidation, Category = Gameplay)
	void EndGame();

	UFUNCTION(BlueprintImplementableEvent, Category = Gameplay)
	void OnGameEnded();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void FindSpawns();

	UClass* MayhemClass;
	UClass* EngletonClass;
	UClass* StrykerClass;
	UClass* FoxClass;
    

	uint8 PlayersSpawned;

	virtual void ReceiveBeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual APawn* SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot) override;
};
