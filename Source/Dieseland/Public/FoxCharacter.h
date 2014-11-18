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

	virtual void SkillOneAim() override;
	virtual void SkillTwoAim() override;
	virtual void SkillThreeAim() override;

	//here we update time
	void UpdateTimers(float DeltaSeconds);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool isAimingOne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool isAimingTwo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool isAimingThree;
		
	//audio assets

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> PenetrationRound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> UltimateVoice;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> RifleRound;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> CharmSound;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> SmokeBombSound;

protected:
	virtual void Tick(float DeltaSeconds) override;
	
};
