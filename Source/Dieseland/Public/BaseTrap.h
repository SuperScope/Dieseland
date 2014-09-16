

#pragma once

#include "GameFramework/Actor.h"
#include "BaseTrap.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ABaseTrap : public AActor
{
	GENERATED_UCLASS_BODY()

	//Collision Cube
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Trap)
	TSubobjectPtr<UBoxComponent> BaseCollisionComponent;

	//Static Mesh or trap
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Trap)
	TSubobjectPtr<UStaticMeshComponent> TrapMesh;
	
	//Function to call when player collision occurs
	UFUNCTION(BlueprintCallable, Category = Trap)
	void OnActivateTrap(AActor* OtherActor, int32 Damage, int32 Speed);

	UPROPERTY(VisibleAnywhere, Category = Trap)
	float DamageBase;

};
