// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "DieselandPlayerController.h"
#include "FoxSmokeGrenadeProjectile.h"
#include "FoxSmokeGrenade.h"
#include "DieselandCharacter.h"


AFoxSmokeGrenadeProjectile::AFoxSmokeGrenadeProjectile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	InitialLifeSpan = 1.0f;
	//the penetration round is meant to be large and move very quickly
	ProjectileMovement->InitialSpeed = 1400.0f;
	ProjCollision->SetCapsuleHalfHeight(150.0f);
	ProjCollision->SetCapsuleRadius(150.0f);

	//getting the smoke grenade mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SmokeGrenadeMesh(TEXT("StaticMesh'/Game/Shapes/Shape_Cube.Shape_Cube'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> GrenadeMatRef(TEXT("Material'/Game/UserInterfaceAssets/HUD/Materials/M_HUD_Health_Bar.M_HUD_Health_Bar'"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_Bullet1.Unreal_Particle_Bullet1'"));
	Particle = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleSystem"));
	Particle->Template = ParticleSystemAsset.Object;
	this->Particle->SetVisibility(false);


	//temp meshscale
	FVector MeshScale;
	MeshScale = FVector(1.0f, 1.0f, 1.0f);
	//for the aim bar and aim sphere stuff
	GrenadeMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent >(this, TEXT("GrenadeMesh"));
	GrenadeMesh->AttachParent = Mesh;
	GrenadeMesh->SetStaticMesh(SmokeGrenadeMesh.Object);
	GrenadeMeshMatStatic = GrenadeMatRef.Object;



	this->Mesh->SetWorldScale3D(MeshScale);

}
void ADieselandCharacter::ReceiveBeginPlay()
{
	Grenade
	AimBarMaterial = UMaterialInstanceDynamic::Create(AimBarMatStatic, this);

	//AimBar->SetWorldLocation(FVector(0, 0, -50));
	AimBar->SetWorldScale3D(FVector(4.0f, 1.0, 0.01));
	AimBar->CastShadow = false;
	AimBar->Materials.Add(AimBarMaterial);

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
			if ((OtherActor->ActorHasTag(TEXT("Player")) && Cast<ADieselandCharacter>(OtherActor)->GetTeamNumber() !=
				Cast<ADieselandCharacter>(Cast<ADieselandPlayerController>(GetOwner())->GetPawn())->GetTeamNumber()) || (OtherActor->ActorHasTag(TEXT("Enemy"))))
			{
				AFoxSmokeGrenade* const Grenade = World->SpawnActor<AFoxSmokeGrenade>(AFoxSmokeGrenade::StaticClass(), this->GetActorLocation(), this->GetActorRotation(), SpawnParams);
				Grenade->ServerActivateProjectile();
				this->Destroy();
			}
		}
	}
}
