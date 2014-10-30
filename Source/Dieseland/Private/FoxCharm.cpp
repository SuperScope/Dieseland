// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "DieselandPlayerController.h"
#include "DieselandCharacter.h"
#include "DieselandEnemyBot.h"
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
	bReplicates = true;
	bReplicateMovement = true;
	Mesh->SetIsReplicated(true);
}


void AFoxCharm::ReceiveActorBeginOverlap(AActor* OtherActor)
{
	AActor::ReceiveActorBeginOverlap(OtherActor);
	if (OtherActor == nullptr){
		return;
	}
	if (Role == ROLE_Authority && Cast<ADieselandPlayerController>(GetOwner())->GetPawn() != OtherActor)
	{
		if (OtherActor->ActorHasTag(TEXT("Player")))
		{
			Cast<ADieselandCharacter>(OtherActor)->StunRemaining = 3.0f;
			Cast<ADieselandCharacter>(OtherActor)->StatusEffects.Add(FString("Stunned"));
			Cast<ADieselandCharacter>(OtherActor)->StatusEffects.Add(FString("Charmed"));
			Cast<ADieselandCharacter>(OtherActor)->VectorDirection = Cast<ADieselandPlayerController>(GetOwner())->GetPawn()->GetActorLocation() - Cast<ADieselandCharacter>(OtherActor)->GetActorLocation();
			Cast<ADieselandCharacter>(OtherActor)->VectorDirection.Normalize();
		}
		else if (OtherActor->ActorHasTag(TEXT("Enemy")))
		{
			Cast<ADieselandEnemyBot>(OtherActor)->StatusEffects.Add(FString("Stunned"));
			Cast<ADieselandEnemyBot>(OtherActor)->StatusEffects.Add(FString("Charmed"));

		}
	}
}

