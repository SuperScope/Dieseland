

#pragma once

#include "GameFramework/Actor.h"
#include "ScrapBox.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API AScrapBox : public AActor
{
	GENERATED_UCLASS_BODY()

	// Displayed Mesh
	UPROPERTY(Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UDestructibleComponent> Mesh;

	UPROPERTY(Replicated, Category = Visual, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UParticleSystemComponent> Particle;

	UPROPERTY(Replicated, Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	int32 ScrapAmt;

	UFUNCTION(Reliable, NetMulticast, WithValidation, BlueprintCallable, Category = Gameplay)
	void DestroyCrate(AActor* Causer);

	virtual void ReceiveActorBeginOverlap(AActor* OtherActor) override;

};
