// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "FoxPenetrationRound.h"


AFoxPenetrationRound::AFoxPenetrationRound(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	InitialLifeSpan = 0.8f;


	//the penetration round is meant to be large and move very quickly
	ProjectileMovement->InitialSpeed = 2500.0f;
	ProjCollision->SetCapsuleHalfHeight(150.0f);
	ProjCollision->SetCapsuleRadius(150.0f);

	//temp meshscale
	FVector MeshScale;
	MeshScale = FVector(3, 3, 3);
	
	this->Mesh->SetWorldScale3D(MeshScale);
	///ReplicatedMovement.LinearVelocity.Z = 1000.0f;
	//ReplicatedMovement.
	Piercing = true;
}


