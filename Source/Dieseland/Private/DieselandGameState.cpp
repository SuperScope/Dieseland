

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
	KillGoal = 20;
	WinningTeam = 0;
	GameDuration = 10.0f;
	GameHasEnded = false;
	GameHasStarted = false;
	GameTimer = GameDuration;

	TeamColors.Add(FVector(0.000905f, 1.0f, 0.0f));
	TeamColors.Add(FVector(0.035f, 0.005232f, 0.004898f));
	TeamColors.Add(FVector(0.0f, 0.035871f, 1.0f));
	TeamColors.Add(FVector(1.0f, 1.0f, 1.0f));
	TeamColors.Add(FVector(0.0f, 0.828977f, 1.0f));
	TeamColors.Add(FVector(1.0f, 0.935999f, 0.0f));
	TeamColors.Add(FVector(0.747108f, 0.0f, 1.0f));
	TeamColors.Add(FVector(1.0f, 0.305141f, 0.0f));
	TeamColors.Add(FVector(1.0f, 0.131611f, 0.925403f));
}

void ADieselandGameState::ReceiveBeginPlay()
{
	if (Role == ROLE_Authority)
	{
		//GetWorldTimerManager().SetTimer(this, &ADieselandGameState::CalculateScore, .2f, true);

		//GetWorldTimerManager().SetTimer(this, &ADieselandGameState::GameTimerEnded, GameDuration, false);
	}
}

void ADieselandGameState::StartTimer()
{
	if (Role == ROLE_Authority)
	{
		GetWorldTimerManager().SetTimer(this, &ADieselandGameState::GameTimerEnded, GameDuration, false);
		GameHasStarted = true;
	}
}

void ADieselandGameState::Tick(float DeltaSeconds)
{
	if (Role == ROLE_Authority)
	{
		CalculateScore();
		if (!GameHasEnded && GameHasStarted)
		{
			GameTimer = GetWorldTimerManager().GetTimerRemaining(this, &ADieselandGameState::GameTimerEnded);
		}
	}
	Super::Tick(DeltaSeconds);
}

TArray<ADieselandPlayerController*> ADieselandGameState::GetPlayers()
{
	return Players;
}

void ADieselandGameState::CalculateScore()
{
	if (Role == ROLE_Authority)
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
				if (Players[x] != nullptr && Players[x]->PlayerState != nullptr)
				{
					int32 TempPlayerTeamNum = Cast<ADieselandPlayerState>(Players[x]->PlayerState)->TeamNumber;

					TempTeamScores[TempPlayerTeamNum] += Cast<ADieselandPlayerState>(Players[x]->PlayerState)->Kills;
					Cast<ADieselandPlayerState>(Players[x]->PlayerState)->SetNetIndex(x);
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
}

void ADieselandGameState::GameTimerEnded()
{
	if (Role == ROLE_Authority)
	{
		//Cast<ADieselandGameMode>(AuthorityGameMode)->EndGame();
		GameTimer = 0.0f;
		GameHasEnded = true;
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
	DOREPLIFETIME(ADieselandGameState, TeamColors);
	DOREPLIFETIME(ADieselandGameState, GameDuration);
	DOREPLIFETIME(ADieselandGameState, GameTimer);
	DOREPLIFETIME(ADieselandGameState, GameHasEnded);
	DOREPLIFETIME(ADieselandGameState, GameHasStarted);
	DOREPLIFETIME(ADieselandGameState, Players);
}
