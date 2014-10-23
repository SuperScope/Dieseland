

#pragma once


#include "UnrealNetwork.h"
#include "DieselandEnemyBot.h"
#include "DieselandHighlanderKingBot.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ADieselandHighlanderKingBot : public ADieselandEnemyBot
{
	GENERATED_UCLASS_BODY()
	// Collider used to detect attack zone radius
	UPROPERTY(Replicated, Category = Combat, BlueprintReadOnly, VisibleAnywhere)
	TSubobjectPtr<class UBoxComponent> CannonZoneCollision;

	// The range of the cannon zone collider
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float CannonRange;

	// Update for attack damage on cannon fire
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float CannonDamageUpdate;


	//Attack Pattern variable used to determine when the highlander king is using its special abilities
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float AttackPatternTimer;

	//CannonAttackticks used t o measure how many times the cannons have fire
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	float CannonAttackTicks;


	//boolean used to determine whether or not the highlander is firing it's cannons
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	bool IsFiringCannons;

	UFUNCTION(BlueprintCallable, Category = Combat)
		void CannonAttack();

	//this is used t o create the beam particle
	UPROPERTY(Replicated, Category = Combat, BlueprintReadWrite, EditAnywhere)
	UParticleSystem* BeamParticle;

	
	// Damage amount for cannon, important to note this damage is updated once per .2 seconds
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	int32 CannonAttackDamage; 
};
