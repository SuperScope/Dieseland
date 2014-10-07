

#pragma once

#include "GameFramework/Actor.h"
#include "DieselandStaticLibrary.h"
#include "DeathTile.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ADeathTile : public AActor
{
	GENERATED_UCLASS_BODY()
    
    /* Static Mesh */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Death Tile")
    TSubobjectPtr<UStaticMeshComponent> DeathTileMesh;
    
    /* Dummy Scene Component to be used as root component*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Death Tile")
    TSubobjectPtr<USceneComponent> DummyComponent;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Death Tile")
	TSubobjectPtr<USphereComponent> SphereCollision;
    
    //Rotation for Tile
    UPROPERTY(BlueprintReadOnly, Category = "Death Tile")
    FRotator DTRotation;
    
    //Scale for Tile
    UPROPERTY(BlueprintReadOnly, Category = "Death Tile")
    FVector DTScale;
    
    //Boolean for enemies remaining on tile
    UPROPERTY(BlueprintReadOnly, Category = "Death Tile")
    bool EnemiesRemaining;
    
    //Boolean for if the tile is down
    UPROPERTY(BlueprintReadWrite, Category = "Death Tile")
    bool IsTileDown;
    
    //Boolean for when the tile is ready to rise
    UPROPERTY(BlueprintReadWrite, Category = "Death Tile")
    bool ReadyToRise;
    
    //Boolean to determine if enemy was found
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Tile")
    bool EnemyFound;
    
    //Vectors for location
    UPROPERTY(BlueprintReadWrite, Category = "Death Tile")
    FVector CurrentLocation;
    
    UPROPERTY(BlueprintReadWrite, Category = "Death Tile")
    FVector TargetLocation;
    
    //Array for the Death Tile Locations
    TArray<FVector> LocationArray;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Tile")
	TArray<AActor*> EnemiesOnTile;
    
    //Find world
    UWorld* const World = GetWorld();
    
	float EnemyCheckInterval;
	
	float EnemyCheckTimer;
    
protected:
    
    
	//virtual void ReceiveActorBeginOverlap(AActor* OtherActor) override;
    
	//virtual void ReceiveActorEndOverlap(AActor* OtherActor) override;
    
    virtual void Tick(float DeltaSeconds) override;
    
    //Function for switching Death Tiles
    UFUNCTION(BlueprintCallable, Category = "Death Tile")
    void SwitchDeathTile();
    
    //Function for detecting enemies
    UFUNCTION(BlueprintCallable, Category = "Death Tile")
	void EnemyDetection();
    
    

};
