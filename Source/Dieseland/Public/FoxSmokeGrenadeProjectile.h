// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseProjectile.h"
#include "FoxSmokeGrenadeProjectile.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API AFoxSmokeGrenadeProjectile : public ABaseProjectile
{
	GENERATED_UCLASS_BODY()

	virtual void ReceiveActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interface)
		TSubobjectPtr<class UStaticMeshComponent> GrenadeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		UMaterialInstanceDynamic* GrenadeMeshMaterial;


	UMaterial* GrenadeMeshMatStatic;
	
};
