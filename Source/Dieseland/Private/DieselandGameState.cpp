

#include "Dieseland.h"
#include "DieselandGameState.h"
#include "DieselandCharacter.h"


ADieselandGameState::ADieselandGameState(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADieselandGameState::ReceiveBeginPlay()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = *Iterator;
		Players.Add(Cast<ADieselandCharacter>(PlayerController->GetPawn()));
	}
}

void ADieselandGameState::Tick(float DeltaSeconds)
{
	for (int32 x = 0; x < Players.Num; x++)
	{

	}

	Super::Tick(DeltaSeconds);
}
