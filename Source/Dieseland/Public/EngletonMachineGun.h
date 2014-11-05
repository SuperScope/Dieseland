

#pragma once

#include "BaseProjectile.h"
#include "EngletonMachineGun.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API AEngletonMachineGun : public ABaseProjectile
{
	GENERATED_UCLASS_BODY()


	virtual void ReceiveActorBeginOverlap(AActor* OtherActor) override;
	
};
