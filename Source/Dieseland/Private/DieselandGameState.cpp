

#include "Dieseland.h"
#include "DieselandGameState.h"
#include "DieselandCharacter.h"


ADieselandGameState::ADieselandGameState(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
	WinningScore = 0;
}

void ADieselandGameState::ReceiveBeginPlay()
{
	
}

void ADieselandGameState::Tick(float DeltaSeconds)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = *Iterator;
		if (!Players.Contains(Cast<ADieselandCharacter>(PlayerController->GetPawn())))
		{
			Players.Add(Cast<ADieselandCharacter>(PlayerController->GetPawn()));
		}
	}

	if (Players.Num() > 0)
	{
		for (int32 x = 0; x < Players.Num(); x++)
		{
			if (Cast<ADieselandCharacter>(Players[x])->Kills > WinningScore)
			{
				WinningPlayer = Players[x];
			}
		}
	}
	Super::Tick(DeltaSeconds);
}
