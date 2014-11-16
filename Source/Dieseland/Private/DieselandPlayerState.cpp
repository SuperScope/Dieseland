// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "DieselandCharacter.h"
#include "DieselandPlayerState.h"
#include "DieselandPlayerController.h"


ADieselandPlayerState::ADieselandPlayerState(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	SetIsReady(false);
	SetCharacterName(FString(TEXT("Engleton")));
	SetUsername(FString(TEXT("Player")));
	SetTeamNum(1);
	SetKillNum(0);
	SetDeaths(0);
	SetNetIndex(0);
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

	switch (TeamNumber)
	{
	case 0:
		TeamColor = FVector(0.000905f, 1.0f, 0.0f);
		break;
	case 1:
		TeamColor = FVector(0.035f, 0.005232f, 0.004898f);
		break;
	case 2:
		TeamColor = FVector(0.0f, 0.035871f, 1.0f);
		break;
	case 3:
		TeamColor = FVector(1.0f, 1.0f, 1.0f);
		break;
	case 4:
		TeamColor = FVector(0.0f, 0.828977f, 1.0f);
		break;
	case 5:
		TeamColor = FVector(1.0f, 0.935999f, 0.0f);
		break;
	case 6:
		TeamColor = FVector(0.747108f, 0.0f, 1.0f);
		break;
	case 7:
		TeamColor = FVector(1.0f, 0.305141f, 0.0f);
		break;
	case 8:
		TeamColor = FVector(1.0f, 0.131611f, 0.925403f);
		break;
	default:
		TeamColor = FVector(0.000905f, 1.0f, 0.0f);
	}
	
	if (Role != ROLE_Authority)
	{
		ServerSetTeamNum(NewTeamNumber);
	}
}

void ADieselandPlayerState::SetTeamColor(FVector NewTeamColor)
{
	TeamColor = NewTeamColor;

	if (Role != ROLE_Authority)
	{
		ServerSetTeamColor(NewTeamColor);
	}
}

void ADieselandPlayerState::SetIsReady(bool NewReadyState)
{
	IsReady = NewReadyState;

	if (Role != ROLE_Authority)
	{
		ServerSetIsReady(NewReadyState);
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

void ADieselandPlayerState::SetDeaths(int32 NewDeaths)
{
	Deaths = NewDeaths;

	if (Role != ROLE_Authority)
	{
		ServerSetDeaths(NewDeaths);
	}
}

void ADieselandPlayerState::SetUsername(const FString& NewName)
{
	Username = NewName;

	if (Role != ROLE_Authority)
	{
		ServerSetUsername(NewName);
	}
}

void ADieselandPlayerState::SetCharacterName(const FString& NewName)
{
	Username = NewName;

	if (Role != ROLE_Authority)
	{
		ServerSetUsername(NewName);
	}
}

void ADieselandPlayerState::SetNetIndex(int32 NewNetIndex)
{
	NetIndex = NewNetIndex;

	if (Role != ROLE_Authority)
	{
		ServerSetNetIndex(NewNetIndex);
	}
}

void ADieselandPlayerState::SetCharacterLevel(int32 NewLevel)
{
	CharacterLevel = NewLevel;

	if (Role != ROLE_Authority)
	{
		ServerSetCharacterLevel(NewLevel);
	}
}

void ADieselandPlayerState::ServerSetUsername_Implementation(const FString& NewName)
{
	SetUsername(NewName);
}

void ADieselandPlayerState::ServerSetCharacterName_Implementation(const FString& NewName)
{
	SetCharacterName(NewName);
}

bool ADieselandPlayerState::ServerSetUsername_Validate(const FString& NewName)
{
	return true;
}

bool ADieselandPlayerState::ServerSetCharacterName_Validate(const FString& NewName)
{
	return true;
}

void ADieselandPlayerState::ServerSetTeamNum_Implementation(int32 NewTeamNumber)
{
	SetTeamNum(NewTeamNumber);
}
	
bool ADieselandPlayerState::ServerSetTeamNum_Validate(int32 NewTeamNumber)
{
	return true;
}

void ADieselandPlayerState::ServerSetTeamColor_Implementation(FVector NewTeamColor)
{
	SetTeamColor(NewTeamColor);
}

bool ADieselandPlayerState::ServerSetTeamColor_Validate(FVector NewTeamColor)
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

void ADieselandPlayerState::ServerSetIsReady_Implementation(bool NewReadyState)
{
	SetIsReady(NewReadyState);
}

bool ADieselandPlayerState::ServerSetIsReady_Validate(bool NewReadyState)
{
	return true;
}

void ADieselandPlayerState::ServerSetDeaths_Implementation(int32 NewDeaths)
{
	SetDeaths(NewDeaths);
}

bool ADieselandPlayerState::ServerSetDeaths_Validate(int32 NewDeaths)
{
	return true;
}

void ADieselandPlayerState::ServerSetNetIndex_Implementation(int32 NewNetIndex)
{
	SetNetIndex(NewNetIndex);
}

bool ADieselandPlayerState::ServerSetNetIndex_Validate(int32 NewNetIndex)
{
	return true;
}

void ADieselandPlayerState::ServerSetCharacterLevel_Implementation(int32 NewLevel)
{
	SetCharacterLevel(NewLevel);
}

bool ADieselandPlayerState::ServerSetCharacterLevel_Validate(int32 NewLevel)
{
	return true;
}

void ADieselandPlayerState::OnRep_TeamColor()
{
	
}

int32 ADieselandPlayerState::GetTeamNum()
{
	return TeamNumber;
}

FVector ADieselandPlayerState::GetTeamColor()
{
	return TeamColor;
}

int32 ADieselandPlayerState::GetKills()
{
	return Kills;
}


int32 ADieselandPlayerState::GetDeaths()
{
	return Deaths;
}

bool ADieselandPlayerState::GetIsReady()
{
	return IsReady;
}

FString ADieselandPlayerState::GetUsername()
{
	return Username;
}

FString ADieselandPlayerState::GetCharacterName()
{
	return CharacterName;
}

int32 ADieselandPlayerState::GetNetIndex()
{
	return NetIndex;
}

int32 ADieselandPlayerState::GetCharacterLevel()
{
	return CharacterLevel;
}

void ADieselandPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADieselandPlayerState, TeamNumber);
	DOREPLIFETIME(ADieselandPlayerState, TeamColor);
	DOREPLIFETIME(ADieselandPlayerState, Kills);
	DOREPLIFETIME(ADieselandPlayerState, Deaths);
	DOREPLIFETIME(ADieselandPlayerState, Username);
	DOREPLIFETIME(ADieselandPlayerState, CharacterName);
	DOREPLIFETIME(ADieselandPlayerState, CharacterLevel);
	DOREPLIFETIME(ADieselandPlayerState, IsReady);
	DOREPLIFETIME(ADieselandPlayerState, NetIndex);
}
