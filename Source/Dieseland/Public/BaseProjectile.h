

#pragma once

#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ABaseProjectile : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
		bool IsAI;


	// Displayed MeshFp
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
		bool IsPoison;

	UPROPERTY(Replicated, Category = Visual, BlueprintReadWrite, EditAnywhere)
	int32 ProjectileDamage;

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void ServerActivateProjectile();

	// Public poision slow amount of this character
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 PoisionSlowAmount;

	// Public damage recution value of this character
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 PoisionDamageReductionAmount;

	virtual void ReceiveActorBeginOverlap(AActor* OtherActor) override;
	
};