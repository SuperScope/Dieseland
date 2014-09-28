

#pragma once

#include "GameFramework/Actor.h"
#include "DeathTile.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ADeathTile : public AActor
{
	GENERATED_UCLASS_BODY()
    
    /** Box component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Death Tile")
    TSubobjectPtr<UBoxComponent> Tile;

	
	
};
