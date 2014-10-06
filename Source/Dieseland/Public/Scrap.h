

#pragma once

#include "GameFramework/Actor.h"
#include "Scrap.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API AScrap : public AActor
{
	GENERATED_UCLASS_BODY()

	// Displayed Mesh
	UPROPERTY(Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UCapsuleComponent> ScrapCollision;

	/*UPROPERTY(Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UCapsuleComponent> ScrapCollectionArea;*/

	UFUNCTION(BlueprintCallable, Reliable, WithValidation, NetMulticast, Category = Gameplay)
	void CollectScrap(AActor* OtherActor);

	TArray<AActor*> ActorsInRange;

	AActor* TargetedActor;

	virtual void ReceiveActorBeginOverlap(AActor* OtherActor);

	virtual void ReceiveHit(UPrimitiveComponent * MyComp, 
		AActor * Other, 
		UPrimitiveComponent * OtherComp, 
		bool bSelfMoved, 
		FVector HitLocation, 
		FVector HitNormal, 
		FVector NormalImpulse, 
		const FHitResult & Hit) override;

	virtual void ReceiveBeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
