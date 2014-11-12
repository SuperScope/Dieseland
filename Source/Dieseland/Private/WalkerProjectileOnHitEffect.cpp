// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "WalkerProjectileOnHitEffect.h"


AWalkerProjectileOnHitEffect::AWalkerProjectileOnHitEffect(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//Get out particle effect for on hit here, you'll need
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_WalkerShotOnHit.Unreal_Particle_WalkerShotOnHit'"));

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Particle = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleSystem"));
	Particle->Template = ParticleSystemAsset.Object;

	Mesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
}


