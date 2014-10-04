

#pragma once

#include "DieselandCharacter.h"
#include "MayhemCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API AMayhemCharacter : public ADieselandCharacter
{
	GENERATED_UCLASS_BODY()
public:

	// The duration of Skull Crusher's stun
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float StunLength;

	virtual void MeleeAttack() override;

	virtual void SkillOne() override;

	virtual void SkillTwo() override;

	virtual void SkillThree() override;
	
};
