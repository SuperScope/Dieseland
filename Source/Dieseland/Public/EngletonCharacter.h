

#pragma once

#include "DieselandCharacter.h"
#include "EngletonCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API AEngletonCharacter : public ADieselandCharacter
{
	GENERATED_UCLASS_BODY()

	//here we override all of the skills for Engleton
	virtual void SkillOne() override;
	virtual void SkillTwo() override;
	virtual void SkillThree() override;
	virtual void MeleeAttack() override;
	virtual void RangedAttack() override;


	// The range of this character's Bombardment
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float BombardmentRange;

	// Collider used to detect pulse Range
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UCapsuleComponent> BombardmentCollision;

	//here I check to see if Bombardment is active
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	bool BombardmentActivated;


	//here I check to see how long bobmarbment has been active
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float BombardmentTimer;

	//the number of times bombardment AOE damage is dealt
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	int16 BombardmentHitCounter;

	//here we update time
	void UpdateTimers(float DeltaSeconds);
	

protected:
	virtual void Tick(float DeltaSeconds) override;
	
};


