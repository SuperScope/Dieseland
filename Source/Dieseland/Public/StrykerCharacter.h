

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


	//here we override all of the skills for Stryker
	virtual void SkillOne() override;
	virtual void SkillTwo() override;
	virtual void SkillThree() override;
	virtual void MeleeAttack() override;
	virtual void RangedAttack() override;


	
	
};

