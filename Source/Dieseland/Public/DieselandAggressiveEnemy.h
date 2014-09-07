

#pragma once

#include "GameFramework/Character.h"
#include "DieselandAggressiveEnemy.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ADieselandAggressiveEnemy : public ACharacter
{
	GENERATED_UCLASS_BODY()

	protected:
		void Move(float val);

		//handles touch inputs 
		void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);



};
