

#pragma once

#include "BaseProjectile.h"
#include "GameFramework/Actor.h"
#include "DieselandWalkerProjectile.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ADieselandWalkerProjectile : public ABaseProjectile
{
	GENERATED_UCLASS_BODY()


	virtual void ReceiveActorBeginOverlap(AActor* OtherActor) override;

	
};
