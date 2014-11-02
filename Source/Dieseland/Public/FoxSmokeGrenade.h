// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "FoxSmokeGrenade.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API AFoxSmokeGrenade : public AActor
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UStaticMeshComponent> Mesh;
	

	UPROPERTY(Replicated, Category = Visual, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UParticleSystemComponent> Particle;

	UPROPERTY(Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UCapsuleComponent> SmokeCollision;

	UPROPERTY(Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UPostProcessComponent> SmokeEffect;

	UPROPERTY(Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UTexture2D> SmokeTexture;

	


	UFUNCTION(Reliable, NetMulticast, WithValidation)
		void ServerActivateProjectile();

	UFUNCTION(Reliable, NetMulticast, WithValidation)
		void ServerAdjustPostProcessing(AActor* OtherActor);

	virtual void ReceiveActorBeginOverlap(AActor* OtherActor) override;
	
	
};
