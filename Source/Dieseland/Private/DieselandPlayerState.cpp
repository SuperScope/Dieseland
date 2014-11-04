// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "DieselandCharacter.h"
#include "DieselandPlayerState.h"


ADieselandPlayerState::ADieselandPlayerState(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	TeamNumber = 0;
}

void ADieselandPlayerState::UpdateTeamColors()
{
	AController* OwnerController = Cast<AController>(GetOwner());
	if (OwnerController != NULL)
	{
		ADieselandCharacter* DieselandCharacter = Cast<ADieselandCharacter>(OwnerController->GetCharacter());
		if (DieselandCharacter != NULL)
		{
			//Change colors in-game here
			
			if (DieselandCharacter->HealthBar != nullptr)
			{
				switch (TeamNumber)
				{
					case 0:
						Cast<UMaterialInstanceDynamic>(DieselandCharacter->HealthBar->Elements[0].Material)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(0.000905f, 1.0f, 0.0f));
						break;
					case 1:
						Cast<UMaterialInstanceDynamic>(DieselandCharacter->HealthBar->Elements[0].Material)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(0.035f, 0.005232f, 0.004898f));
						break;
					case 2:
						Cast<UMaterialInstanceDynamic>(DieselandCharacter->HealthBar->Elements[0].Material)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(0.0f, 0.035871f, 1.0f));
						break;
					case 3:
						Cast<UMaterialInstanceDynamic>(DieselandCharacter->HealthBar->Elements[0].Material)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(1.0f, 1.0f, 1.0f));
						break;
					case 4:
						Cast<UMaterialInstanceDynamic>(DieselandCharacter->HealthBar->Elements[0].Material)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(0.0f, 0.828977f, 1.0f));
						break;
					case 5:
						Cast<UMaterialInstanceDynamic>(DieselandCharacter->HealthBar->Elements[0].Material)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(1.0f, 0.935999f, 0.0f));
						break;
					case 6:
						Cast<UMaterialInstanceDynamic>(DieselandCharacter->HealthBar->Elements[0].Material)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(0.747108f, 0.0f, 1.0f));
						break;
					case 7:
						Cast<UMaterialInstanceDynamic>(DieselandCharacter->HealthBar->Elements[0].Material)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(1.0f, 0.305141f, 0.0f));
						break;
					case 8:
						Cast<UMaterialInstanceDynamic>(DieselandCharacter->HealthBar->Elements[0].Material)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(1.0f, 0.131611f, 0.925403f));
						break;
					//default:
						//Cast<UMaterialInstanceDynamic>(DieselandCharacter->HealthBar->Elements[0].Material)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(0.000905f, 1.0f, 0.0f));
				}
			}
		}
	}
}

void ADieselandPlayerState::SetTeamNum(int32 NewTeamNumber)
{
	TeamNumber = NewTeamNumber;

	UpdateTeamColors();
}

void ADieselandPlayerState::OnRep_TeamColor()
{
	UpdateTeamColors();
}

int32 ADieselandPlayerState::GetTeamNum() const
{
	return TeamNumber;
}

void ADieselandPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADieselandPlayerState, TeamNumber);
}
