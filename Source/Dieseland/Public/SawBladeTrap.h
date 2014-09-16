

#pragma once

#include "BaseTrap.h"
#include "SawBladeTrap.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ASawBladeTrap : public ABaseTrap
{
	GENERATED_UCLASS_BODY()

	//Sets the amount of damage the sawblades do
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SawBlade)
	float damageGiven;
	
	//Overrides function (uses implementation bc its a blueprint native event)
	//void OnActivateTrap_Implementation() OVERRIDE;
};
