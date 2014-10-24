// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "FoxPenetrationRound.h"


AFoxPenetrationRound::AFoxPenetrationRound(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//the penetration round is meant to be large and move very quickly
	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjCollision->SetCapsuleHalfHeight(200.0f);
	ProjCollision->SetCapsuleRadius(200.0f);

	Piercing = true;
}


