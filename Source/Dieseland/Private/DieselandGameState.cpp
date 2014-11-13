

#include "Dieseland.h"
#include "UnrealNetwork.h"
#include "DieselandGameState.h"
#include "DieselandGameMode.h"
#include "DieselandCharacter.h"


ADieselandGameState::ADieselandGameState(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
	WinningScore = 0;
	KillGoal = 25;
	WinningTeam = 0;
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
			if (Players[x] == nullptr)
			{
				Players.RemoveAt(x);
				continue;
			}
			if (Cast<ADieselandCharacter>(Players[x])->Kills > WinningScore)
			{
				WinningScore = Players[x]->Kills;
				if (WinningScore >= KillGoal && Role == ROLE_Authority)
				{
					Cast<ADieselandGameMode>(AuthorityGameMode)->EndGame();
				}
			}
		}
	}
	Super::Tick(DeltaSeconds);
}

void ADieselandGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ADieselandGameState, KillGoal);
	DOREPLIFETIME(ADieselandGameState, WinningScore);
	DOREPLIFETIME(ADieselandGameState, WinningTeam);
	DOREPLIFETIME(ADieselandGameState, Players);
}
