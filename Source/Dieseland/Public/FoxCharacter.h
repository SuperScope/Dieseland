// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DieselandCharacter.h"
#include "FoxCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API AFoxCharacter : public ADieselandCharacter
{
	GENERATED_UCLASS_BODY()

	virtual void SkillOne() override;
	virtual void SkillTwo() override;
	virtual void SkillThree() override;
	virtual void MeleeAttack() override;
	virtual void RangedAttack() override;
	//audio assets

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> PenetrationRound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> UltimateVoice;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> RifleRound;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> CharmSound;
	
};
