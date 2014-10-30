#pragma once

#include "GameFramework/Actor.h"
#include "BaseTrap.generated.h"

/**
*
*/
UCLASS()
class DIESELAND_API ABaseTrap : public AActor
{
	GENERATED_UCLASS_BODY()

	//Damage done to actors from traps
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Trap)
	int32 TrapDamage;

	//Speed loss to actors from oil traps
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Trap)
	int32 TrapSlow;

	//Gets normal Speed
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Trap)
		int32 RegularSpeed;

	//Damage done to actors from fire traps upon exitting them
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Trap)
		int32 LingerDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Trap)
		float LingerCoolDown;

	UFUNCTION(Reliable, NetMulticast, WithValidation)
		void ServerActivateTrap();

protected:

	virtual void ReceiveActorBeginOverlap(AActor* OtherActor) override;

	virtual void ReceiveActorEndOverlap(AActor* OtherActor) override;

};