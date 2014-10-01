

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
    
    /* Static Mesh */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Death Tile")
    TSubobjectPtr<UStaticMeshComponent> DeathTileMesh;
    
    /* Dummy Scene Component to be used as root component*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Death Tile")
    TSubobjectPtr<USceneComponent> DummyComponent;
    
    /* Volume for collision detection*/
    UPROPERTY(VisibleAnywhere, Category = "Death Tile")
    TSubobjectPtr<USphereComponent> SphereComponent;
    
    //Function for collision detection
    UFUNCTION()
    void OnOverlap(class AActor* Enemy, class UPrimitiveComponent* Sphere);
    
    //Rotation for Tile
    FRotator DTRotation;
    
    //Scale for Tile
    FVector DTScale;
    
    //Boolean for enemies
    bool EnemiesRemaining;
    

};
