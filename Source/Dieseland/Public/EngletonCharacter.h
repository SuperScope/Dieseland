

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
		float PulseRange;

	// Collider used to detect Bombardment Range
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UCapsuleComponent> BombardmentCollision;

	// The range of this character's Pulse
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
		float BombardmentRange;

	// Collider used to detect pulse Range
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
		TSubobjectPtr<class UCapsuleComponent> PulseCollision;


	//here I check to see if Bombardment is active
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	bool BombardmentActivated;

	//here I check to see if pulse is activated
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	bool PulseActivated;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		TArray<AActor*> ActorsInBombardmentRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		TArray<AActor*> ActorsInPulseRange;

	//pulse actor used for owning players
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Gameplay)
		AActor* PulseActor;

	//here I check to see how long bobmarbment has been active
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float BombardmentTimer;

	//here I check to see how pulse has been activated
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float PulseTimer;

	//the number of times bombardment AOE damage is dealt
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	int16 BombardmentHitCounter;

	//here we get our bombardment particle effect
	UPROPERTY(Replicated, Category = Combat, BlueprintReadWrite, EditAnywhere)
		UParticleSystem* BombardmentParticle;

	//here we get our pulse particle effect
	UPROPERTY(Replicated, Category = Combat, BlueprintReadWrite, EditAnywhere)
		UParticleSystem* PulseParticle;

	//here we get our machine gun fire particle effect
	UPROPERTY(Replicated, Category = Combat, BlueprintReadWrite, EditAnywhere)
		UParticleSystem* MachineGunFireParticle;

	//here we update time
	void UpdateTimers(float DeltaSeconds);
	

protected:
	virtual void Tick(float DeltaSeconds) override;
	
};


