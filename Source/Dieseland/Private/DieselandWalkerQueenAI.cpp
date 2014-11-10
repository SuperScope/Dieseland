// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "UnrealNetwork.h"
#include "DieselandWalkerQueenAI.h"
#include "Engine.h"
#include "DieselandEnemyBot.h"
#include "DieselandWalkerQueen.h"


ADieselandWalkerQueenAI::ADieselandWalkerQueenAI(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	AttackTimer = 0;

}

bool ADieselandWalkerQueenAI::ServerSummonWalkerBodyGuard_Validate()
{
	return true;
}

void ADieselandWalkerQueenAI::ServerSummonWalkerBodyGuard_Implementation()
{
	ADieselandWalkerQueen* DieselandPawn = Cast<ADieselandWalkerQueen>(GetPawn());
	DieselandPawn->SummonWalkerBodyGuard();
}

void ADieselandWalkerQueenAI::UpdateCooldownTimers(float DeltaSeconds)
{
	if (Cast<ADieselandWalkerQueen>(GetPawn()) != nullptr)
	{
		ADieselandWalkerQueen* DieselandPawn = Cast<ADieselandWalkerQueen>(GetPawn());
		if (AttackTimer >= 15)
		{
			DieselandPawn->SummonWalkerBodyGuard();
			AttackTimer = 0;
		}

	

	}
	if (Cast<ADieselandEnemyBot>(GetPawn()) != nullptr){
		ADieselandEnemyBot* BotPawn = Cast<ADieselandEnemyBot>(GetPawn());
		if (BotPawn->isAggressive)
		{
			AttackTimer += DeltaSeconds;

		}
	}

}

void ADieselandWalkerQueenAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateCooldownTimers(DeltaTime);

}

