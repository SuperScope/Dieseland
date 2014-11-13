

#include "Dieseland.h"
#include "UnrealNetwork.h"
#include "DieselandGameState.h"
#include "DieselandGameMode.h"
#include "DieselandPlayerState.h"
#include "DieselandPlayerController.h"
#include "DieselandCharacter.h"


ADieselandGameState::ADieselandGameState(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
	WinningScore = 0;
	KillGoal = 5;
	WinningTeam = 0;
}

void ADieselandGameState::ReceiveBeginPlay()
{
	GetWorldTimerManager().SetTimer(this, &ADieselandGameState::CalculateScore, 1.0f, true);
}

void ADieselandGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ADieselandGameState::CalculateScore()
{
	TArray<int32> TempTeamScores;

	for (int32 a = 0; a < 9; a++)
	{
		TempTeamScores.Add(0);
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = *Iterator;
		if (!Players.Contains(Cast<ADieselandPlayerController>(PlayerController)))
		{
			Players.Add(Cast<ADieselandPlayerController>(PlayerController));
		}
	}

	if (Players.Num() > 0)
	{
		for (int32 x = 0; x < Players.Num(); x++)
		{
			if (Players[x] != nullptr)
			{


				int32 TempPlayerTeamNum = Cast<ADieselandPlayerState>(Players[x]->PlayerState)->TeamNumber;

				TempTeamScores[TempPlayerTeamNum] += Cast<ADieselandPlayerState>(Players[x]->PlayerState)->Kills;

				/*if (Cast<ADieselandPlayerState>(Cast<ADieselandCharacter>(Players[x])->PlayerState)->Kills > WinningScore)
				{
				WinningScore = Players[x]->Kills;
				if (WinningScore >= KillGoal && Role == ROLE_Authority)
				{
				Cast<ADieselandGameMode>(AuthorityGameMode)->EndGame();
				}
				}*/
			}
			else
			{
				Players.RemoveAt(x, 1);
			}
		}
	}
	TeamScores = TempTeamScores;

	for (int32 i = 0; i < TeamScores.Num(); i++)
	{
		if (TeamScores[i] > WinningScore)
		{
			WinningScore = TeamScores[i];
			WinningTeam = i;

			if (WinningScore >= KillGoal && Role == ROLE_Authority)
			{
				Cast<ADieselandGameMode>(AuthorityGameMode)->EndGame();
			}
		}
		GEngine->AddOnScreenDebugMessage(i, 100.0f, FColor::Green, FString::FromInt(TeamScores[i]));
	}
}

void ADieselandGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ADieselandGameState, KillGoal);
	DOREPLIFETIME(ADieselandGameState, WinningScore);
	DOREPLIFETIME(ADieselandGameState, WinningTeam);
	DOREPLIFETIME(ADieselandGameState, TeamScores);
	DOREPLIFETIME(ADieselandGameState, Players);
}
