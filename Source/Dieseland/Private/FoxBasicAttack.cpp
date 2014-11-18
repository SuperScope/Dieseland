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
	MeshScale = FVector(0.1f, 0.1f, 0.1f);


	this->Mesh->SetWorldScale3D(MeshScale);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/Test/MovingBulletTest_WIP2.MovingBulletTest_WIP2'"));
	Particle = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleSystem"));
	Particle->Template = ParticleSystemAsset.Object;

	Piercing = false;
}


