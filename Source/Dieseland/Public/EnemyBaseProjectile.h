

#pragma once

#include "BaseProjectile.h"
#include "EnemyBaseProjectile.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API AEnemyBaseProjectile : public ABaseProjectile
{
	GENERATED_UCLASS_BODY()

	public: 
		virtual void ReceiveActorBeginOverlap(AActor* OtherActor) override;

	
	
};
