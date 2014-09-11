

#pragma once

#include "GameFramework/Actor.h"
#include "LevelGeneration_BossZone.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ALevelGeneration_BossZone : public AActor
{
	GENERATED_UCLASS_BODY()

    UPROPERTY(VisibleAnywhere, Category = LevelGeneration)
    TSubobjectPtr<UBoxComponent> Ground;
	
};
