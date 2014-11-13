

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

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = UserProfile)
	FString Username;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
};
