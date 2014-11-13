

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

	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	int32 ScrapValue;

	UFUNCTION(BlueprintCallable, Reliable, WithValidation, NetMulticast, Category = Gameplay)
	void CollectScrap(AActor* OtherActor);

	//sound for scrap
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> ScrapSound;

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
