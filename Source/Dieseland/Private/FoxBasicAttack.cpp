// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "FoxBasicAttack.h"


AFoxBasicAttack::AFoxBasicAttack(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	InitialLifeSpan = 1.0f;


	//the penetration round is meant to be large and move very quickly
	ProjectileMovement->InitialSpeed = 1900.0f;
	ProjCollision->SetCapsuleHalfHeight(50.0f);
	ProjCollision->SetCapsuleRadius(50.0f);

	//temp meshscale
	FVector MeshScale;
	MeshScale = FVector(0.75f, 0.75f, 0.75f);

	this->Mesh->SetWorldScale3D(MeshScale);

	Piercing = false;
}


