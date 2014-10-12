

#pragma once

#include "GameFramework/SaveGame.h"
#include "DieselandSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API UDieselandSaveGame : public USaveGame
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString LastCharacter;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
};