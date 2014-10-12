

#pragma once

#include "DieselandCharacter.h"
#include "StrykerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API AStrykerCharacter : public ADieselandCharacter
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(Reliable, Server, WithValidation)
	void UpdateDurationTimers(float DeltaSeconds);

	//here we override all of the skills for Stryker
	virtual void SkillOne() override;
	virtual void SkillTwo() override;
	virtual void SkillThree() override;
	virtual void MeleeAttack() override;
	virtual void RangedAttack() override;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Combat)
		float AssasinationAttemptDuration;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Combat)
		bool IsAttemptingAssassinate;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		bool IsAssasinating;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		float AssasinationDuration;



protected:
	virtual void Tick(float DeltaSeconds) override;
	
};

