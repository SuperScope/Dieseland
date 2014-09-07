// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "DieselandCharacter.generated.h"

UCLASS(Blueprintable)
class ADieselandCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()
public:

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class UCameraComponent> TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class USpringArmComponent> CameraBoom;

	// Temporary display of health value
	// TODO: Remove when UI is ready
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interface)
	TSubobjectPtr<class UTextRenderComponent> PlayerLabel;

	// Mesh attached to the torso socket which is used to show attack direction - invisible by default
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UStaticMeshComponent> AimMesh;

	// Collider used to detect melee range
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UCapsuleComponent> MeleeCollision;

	// Called to subtract and/or add health to the target player
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void EditHealth(int32 Amt, AActor* Target);

	// Public health value of this character
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 Health;

	// Does this character use melee for it's basic attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	bool IsMelee;

	// The range of this character's melee attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float MeleeRange;

	// The range of this character's ranged attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float RangedRange;

	// Replicated Rotation of the torso for aiming purposess
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_AimRotation, EditAnywhere, BlueprintReadWrite, Category = Network)
	FRotator AimRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	TArray<AActor*> ActorsInMeleeRange;

	// Combat functions
	UFUNCTION(BlueprintCallable, Category = Combat)
	void RangedAttack();

	UFUNCTION(BlueprintCallable, Category = Combat)
	void MeleeAttack();

	UFUNCTION(BlueprintCallable, Category = Combat)
	void SkillOne();

	UFUNCTION(BlueprintCallable, Category = Combat)
	void SkillTwo();

	UFUNCTION(BlueprintCallable, Category = Combat)
	void SkillThree();

	// Called when AimRotation is replicated
	UFUNCTION()
	void OnRep_AimRotation();

protected:
	virtual void Tick(float DeltaSeconds) override;

};

