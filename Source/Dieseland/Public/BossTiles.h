

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

    UPROPERTY(Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	bool Spawn;
    
    UPROPERTY(Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    bool CanSpawn;
    
    UPROPERTY(Category = Gameplay, EditAnywhere)
    int32 BossTimer;
    
    UPROPERTY(Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    bool BossStart;
    
    UPROPERTY(Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    bool BossFinish;
    
    UPROPERTY(Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    bool CheckForBoss;
    
    //Used when boss is dead
    UPROPERTY(Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    bool BossDead;
    
    //Used for checking is boss is alive or not
    UPROPERTY(Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    bool IsBossAlive;
    
    UPROPERTY(Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    FString BossTag;
    
    UPROPERTY(Category = Gameplay, BlueprintReadWrite, EditAnywhere)
    FString GateTag;
    
    ADieselandGameMode* GameMode;
    
    virtual void Tick(float DeltaSeconds) override;
	
};
