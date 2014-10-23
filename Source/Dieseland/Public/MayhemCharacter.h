

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
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	float IronArmorHealthBuff;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	float IronArmorRegenBuff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float IronArmorDuration;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float RageTimer;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float RageAttkSpeedBuff;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float RageMoveSpeedBuff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float RageDuration;

	UFUNCTION(Reliable, Server, WithValidation)
	void UpdateDurationTimers(float DeltaSeconds);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
	TSubobjectPtr <UAudioComponent> UltimateSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
	TSubobjectPtr <UAudioComponent> IronArmorSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
	TSubobjectPtr <UAudioComponent> PunchSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
	TSubobjectPtr <UAudioComponent> RageVoiceOver;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
	TSubobjectPtr <UAudioComponent> RageSound;

	virtual void MeleeAttack() override;

	virtual void SkillOne() override;

	virtual void SkillTwo() override;

	virtual void SkillThree() override;

protected:
	virtual void Tick(float DeltaSeconds) override;
};
