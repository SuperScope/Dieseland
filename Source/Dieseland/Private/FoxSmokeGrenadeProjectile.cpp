// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "DieselandPlayerController.h"
#include "FoxSmokeGrenadeProjectile.h"
#include "FoxSmokeGrenade.h"


AFoxSmokeGrenadeProjectile::AFoxSmokeGrenadeProjectile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	InitialLifeSpan = 1.0f;
	//the penetration round is meant to be large and move very quickly
	ProjectileMovement->InitialSpeed = 1400.0f;
	ProjCollision->SetCapsuleHalfHeight(150.0f);
	ProjCollision->SetCapsuleRadius(150.0f);

	//temp meshscale
	FVector MeshScale;
	MeshScale = FVector(1.0f, 1.0f, 1.0f);

	this->Mesh->SetWorldScale3D(MeshScale);
}

void AFoxSmokeGrenadeProjectile::ReceiveActorBeginOverlap(AActor* OtherActor)
{
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		AActor::ReceiveActorBeginOverlap(OtherActor);
		if (OtherActor == nullptr){
			return;
		}
		if (Role == ROLE_Authority && Cast<ADieselandPlayerController>(GetOwner())->GetPawn() != OtherActor)
		{
			if (OtherActor->ActorHasTag(TEXT("Player")) || (OtherActor->ActorHasTag(TEXT("Enemy"))))
			{
				AFoxSmokeGrenade* const Grenade = World->SpawnActor<AFoxSmokeGrenade>(AFoxSmokeGrenade::StaticClass(), this->GetActorLocation(), this->GetActorRotation(), SpawnParams);
				Grenade->ServerActivateProjectile();
				this->Destroy();
			}
		}
	}
}
