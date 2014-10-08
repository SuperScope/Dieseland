

#pragma once

#include "GameFramework/Actor.h"
#include "BaseWalkerProjectile.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ABaseWalkerProjectile : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
		bool IsAI;

	// Displayed Mesh
	UPROPERTY(Category = Combat, BlueprintReadOnly, VisibleAnywhere)
		TSubobjectPtr<class UStaticMeshComponent> Mesh;

	// Collider used to detect enemies for balancing
	UPROPERTY(Category = Combat, BlueprintReadOnly, VisibleAnywhere)
		TSubobjectPtr<class UCapsuleComponent> ProjCollision;

	// Movement component
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
		TSubobjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(Replicated, Category = Visual, BlueprintReadOnly, VisibleAnywhere)
		TSubobjectPtr<class UParticleSystemComponent> Particle;

	UPROPERTY(Replicated, Category = Visual, BlueprintReadWrite, EditAnywhere)
		bool Piercing;

	UPROPERTY(Replicated, Category = Visual, BlueprintReadWrite, EditAnywhere)
		int32 ProjectileDamage;

	UFUNCTION(Reliable, NetMulticast, WithValidation)
		void ServerActivateProjectile();

	virtual void ReceiveActorBeginOverlap(AActor* OtherActor) override;
	
};
