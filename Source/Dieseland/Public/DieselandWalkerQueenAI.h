// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DieselandEnemyAI.h"
#include "DieselandWalkerQueenAI.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ADieselandWalkerQueenAI : public ADieselandEnemyAI
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSummonWalkerBodyGuard();

	//here we update our  cooldown timers
	void UpdateCooldownTimers(float DeltaSeconds);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float AttackTimer;

	// Begin tick timer
	virtual void Tick(float DeltaTime) override;
	
};

