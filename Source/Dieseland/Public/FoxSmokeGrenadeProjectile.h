// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseProjectile.h"
#include "GameFramework/Character.h"
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
	//here I check to see how long bobmarbment has been active
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		float GrenadeLifeTime;

	//here we update time
	void UpdateTimers(float DeltaSeconds);


protected:
	UMaterial* GrenadeMeshMatStatic;
	virtual void ReceiveBeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
};
