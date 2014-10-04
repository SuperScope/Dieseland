

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

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float IronArmorTimer;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float IronArmorBuff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float IronArmorDuration;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float RageTimer;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float RageBuff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float RageDuration;

	UFUNCTION(Reliable, Server, WithValidation)
	void UpdateDurationTimers(float DeltaSeconds);

	virtual void MeleeAttack() override;

	virtual void SkillOne() override;

	virtual void SkillTwo() override;

	virtual void SkillThree() override;

protected:
	virtual void Tick(float DeltaSeconds) override;
};
