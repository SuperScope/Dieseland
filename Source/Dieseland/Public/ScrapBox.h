

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
	TSubobjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(Replicated, Category = Visual, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UParticleSystemComponent> Particle;

	virtual void ReceiveActorBeginOverlap(AActor* OtherActor) override;

};
