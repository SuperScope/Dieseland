// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "DieselandWalkerQueen.h"


ADieselandWalkerQueen::ADieselandWalkerQueen(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
	BaseAttackDamage = 40;
	HealthRegeneration = 6;
	BasicAttackDamage = BaseAttackDamage;
	MaxHealth = 1500;
	Health = MaxHealth;
}

void ADieselandWalkerQueen::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void ADieselandWalkerQueen::SummonWalkerBodyGuard()
{

}
