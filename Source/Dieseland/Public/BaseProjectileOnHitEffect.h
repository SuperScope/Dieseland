// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseProjectileOnHitEffect.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ABaseProjectileOnHitEffect : public AActor
{
	GENERATED_UCLASS_BODY()


	UPROPERTY(Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(Replicated, Category = Visual, BlueprintReadOnly, VisibleAnywhere)
		TSubobjectPtr<class UParticleSystemComponent> Particle;

	UFUNCTION(Reliable, NetMulticast, WithValidation)
		void ServerActivateProjectile();

};
