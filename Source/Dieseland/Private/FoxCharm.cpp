// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "FoxCharm.h"


AFoxCharm::AFoxCharm(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	InitialLifeSpan =0.5f;


	//the penetration round is meant to be large and move very quickly
	ProjectileMovement->InitialSpeed = 1600.0f;
	ProjCollision->SetCapsuleHalfHeight(150.0f);
	ProjCollision->SetCapsuleRadius(150.0f);

	//temp meshscale
	FVector MeshScale;
	MeshScale = FVector(4.0f, 4.0f, 4.0f);

	this->Mesh->SetWorldScale3D(MeshScale);

	Piercing = true;
}


