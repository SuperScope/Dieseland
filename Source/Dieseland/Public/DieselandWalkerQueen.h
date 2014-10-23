// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DieselandEnemyBot.h"
#include "DieselandWalkerQueen.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ADieselandWalkerQueen : public ADieselandEnemyBot
{
	GENERATED_UCLASS_BODY()

	// The Base Cooldown for summoning walkers
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float WalkerSummonCooldown;

	// The timer for skill 1
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = Combat)
	float WalkerSummonTimer;

	UFUNCTION(BlueprintCallable, Category = Combat)
	void SummonWalkerBodyGuard();
};
