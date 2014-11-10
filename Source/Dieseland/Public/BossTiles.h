

#pragma once

#include "GameFramework/Actor.h"
#include "UnrealNetwork.h"
#include "DieselandGameMode.h"
#include "BossTiles.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ABossTiles : public AActor
{
	GENERATED_UCLASS_BODY()

    //Used to trigger boss spawning in blueprints
    UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	bool Spawn;
    //Used to make sure we can spawn a boss, eliminates multiple bosses at same time
    UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    bool CanSpawn;
    //Starts Boss Gate dropping
    UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    bool BossStart;
    //Starts Boss Gate rising
    UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    bool BossFinish;
    //Used to trigger boss checking when boss spawns
    UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    bool CheckForBoss;
    //Used when boss is dead
    UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    bool BossDead;
    //Used for checking is boss is alive or not
    UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    bool IsBossAlive;
    //Tag used to determine which tile to spawn boss to
    UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    FName ZoneTag;
    //Tag used to determine which boss to spawn (king or queen)
    UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    FName BossTag;
    //GameMode reference
    ADieselandGameMode* GameMode;
    
    virtual void Tick(float DeltaSeconds) override;
	
};
