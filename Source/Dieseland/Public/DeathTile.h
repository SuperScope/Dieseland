

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
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Death Tile")
    TSubobjectPtr<UStaticMeshComponent> DeathTileMesh;
    
    /* Dummy Scene Component to be used as root component*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Death Tile")
    TSubobjectPtr<USceneComponent> DummyComponent;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Death Tile")
	TSubobjectPtr<UCapsuleComponent> SphereCollision;
    
    //Rotation for Tile
    UPROPERTY(BlueprintReadOnly, Category = "Death Tile")
    FRotator DTRotation;
    
    //Scale for Tile
    UPROPERTY(BlueprintReadOnly, Category = "Death Tile")
    FVector DTScale;
    
    //Boolean for enemies remaining on tile
    UPROPERTY(BlueprintReadOnly, Category = "Death Tile")
    bool EnemiesRemaining;
    
    //Boolean to determine if enemy was found
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Death Tile")
    bool EnemyFound;

	// Hard limit on tile active state
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Tile")
	float TimeLimit;
    
    //Array for the Death Tile Locations
    TArray<FVector> LocationArray;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Death Tile")
	bool EnemyDetectionActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Tile")
	TArray<AActor*> EnemiesOnTile;
    
	UFUNCTION(BlueprintImplementableEvent, Category = "Death Tile")
	void OnShouldDrop();

	UFUNCTION(BlueprintImplementableEvent, Category = "Death Tile")
	void OnSpawnLights();

	UFUNCTION(BlueprintImplementableEvent, Category = "Death Tile")
	void OnDestroyLights();

    //Find world
    UWorld* const World = GetWorld();

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Death Tile")
	AActor* SpotlightArray;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Death Tile")
	int32 DeathTileIndex;
    
	float EnemyCheckInterval;

	float EnemyCheckTimer;
	
	UClass* SpotlightArrayClass;

    virtual void Tick(float DeltaSeconds) override;
    
    //Function for switching Death Tiles
    UFUNCTION(BlueprintCallable, Category = "Death Tile")
    void SwitchDeathTile();
    
    //Function for detecting enemies
    UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Death Tile")
	void EnemyDetection();

};
