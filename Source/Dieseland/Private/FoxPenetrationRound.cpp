// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "FoxPenetrationRound.h"


AFoxPenetrationRound::AFoxPenetrationRound(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	InitialLifeSpan = 1.1f;


	//the penetration round is meant to be large and move very quickly
	ProjectileMovement->InitialSpeed = 1900.0f;
	ProjCollision->SetCapsuleHalfHeight(150.0f);
	ProjCollision->SetCapsuleRadius(150.0f);

	//temp meshscale
	FVector MeshScale;
	MeshScale = FVector(1, 1, 1);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_FoxPenetratorShot.Unreal_Particle_FoxPenetratorShot'"));
	Particle = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleSystem"));
	Particle->Template = ParticleSystemAsset.Object;



	this->Mesh->SetWorldScale3D(MeshScale);
	///ReplicatedMovement.LinearVelocity.Z = 1000.0f;
	//ReplicatedMovement.
	Piercing = true;
}


