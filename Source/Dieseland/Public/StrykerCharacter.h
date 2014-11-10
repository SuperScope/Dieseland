

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

	// Collider used to detect melee range
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
		TSubobjectPtr<class UCapsuleComponent> AssassinationCollider;

	// The range of this character's melee attack
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		float AssassinationColliderRange;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Combat)
		float AssasinationAttemptDuration;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Combat)
		bool IsAttemptingAssassinate;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		bool IsAssassinating;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		float AssasinationDuration;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		float AssasinationDuration2;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Combat)
		int32 AssasinationHitCounter;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Combat)
		AActor* AssassinationTarget;


	//combat functions
	UFUNCTION(Reliable, Server, WithValidation)
		void SearchForAssassinationTarget();

	//audio assets
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> SlashSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> UltimateVoice;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> BlinkSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> PoisonSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> AssassinationSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		TSubobjectPtr <UAudioComponent> SlashHitSound;




protected:
	virtual void Tick(float DeltaSeconds) override;
	
};

