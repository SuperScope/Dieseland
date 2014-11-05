// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "EngletonMachineGunSpark.h"


AEngletonMachineGunSpark::AEngletonMachineGunSpark(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	InitialLifeSpan = 0.2f;
	Mesh->SetWorldScale3D(FVector(1.5f, 1.5f, 1.5f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_EngletonMachineGun_WIP.Unreal_Particle_EngletonMachineGun_WIP'"));
	Particle = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleSystem"));
	Particle->Template = ParticleSystemAsset.Object;
}


