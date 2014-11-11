// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "DieselandCharacter.h"
#include "DieselandPlayerState.h"
#include "DieselandPlayerController.h"


ADieselandPlayerState::ADieselandPlayerState(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	TeamNumber = 1;
	Kills = 0;
}

void ADieselandPlayerState::ClientInitialize(class AController* InController)
{
	Super::ClientInitialize(InController);

}

void ADieselandPlayerState::UpdateTeamColors()
{
	//if (Role != ROLE_Authority)
	//{
		
	//}
}

void ADieselandPlayerState::SetTeamNum(int32 NewTeamNumber)
{
	TeamNumber = NewTeamNumber;
	
	if (Role != ROLE_Authority)
	{
		ServerSetTeamNum(NewTeamNumber);
	}
}

void ADieselandPlayerState::SetKillNum(int32 NewKillNumber)
{
	Kills = NewKillNumber;

	if (Role != ROLE_Authority)
	{
		ServerSetTeamNum(NewKillNumber);
	}
}

void ADieselandPlayerState::ServerSetTeamNum_Implementation(int32 NewTeamNumber)
{
	SetTeamNum(NewTeamNumber);
}
	
bool ADieselandPlayerState::ServerSetTeamNum_Validate(int32 NewTeamNumber)
{
	return true;
}

void ADieselandPlayerState::ServerSetKillNum_Implementation(int32 NewKillNumber)
{
	SetKillNum(NewKillNumber);
}

bool ADieselandPlayerState::ServerSetKillNum_Validate(int32 NewKillNumber)
{
	return true;
}

void ADieselandPlayerState::OnRep_TeamColor()
{
	//UpdateTeamColors();
}

int32 ADieselandPlayerState::GetTeamNum() const
{
	return TeamNumber;
}

void ADieselandPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADieselandPlayerState, TeamNumber);
	DOREPLIFETIME(ADieselandPlayerState, Kills);
}
