

#pragma once

#include "Object.h"
#include "DieselandStaticLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API UDieselandStaticLibrary : public UObject
{
	GENERATED_UCLASS_BODY()

	static FORCEINLINE bool IsValid(AActor* TheActor)
	{
		if (!TheActor) return false;
		if (!TheActor->IsValidLowLevel()) return false;
		return true;
	}

	template <typename BlueprintType>
	static FORCEINLINE BlueprintType* SpawnBlueprint(
		UWorld* TheWorld,
		UClass* TheClass,
		const FVector Location,
		const FRotator Rotation,
		const bool bNoCollisionFail = true,
		AActor* Owner = NULL,
		APawn* Instigator = NULL)
	{
		if (!TheWorld) return NULL;
		if (!TheClass) return NULL;

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.bNoCollisionFail = bNoCollisionFail;
		SpawnInfo.Owner = Owner;
		SpawnInfo.Instigator = Instigator;
		SpawnInfo.bDeferConstruction = false;

		return TheWorld->SpawnActor<BlueprintType>(TheClass, Location, Rotation, SpawnInfo);
	}
	
};
