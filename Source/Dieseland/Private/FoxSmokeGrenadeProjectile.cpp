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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SmokeGrenadeMesh(TEXT("StaticMesh'/Game/Particles/Test/Custom/model_smokegrenade.model_smokegrenade'"));
	//static ConstructorHelpers::FObjectFinder<UMaterial> GrenadeMatRef(TEXT("Material'/Game/UserInterfaceAssets/HUD/Materials/M_HUD_Health_Bar.M_HUD_Health_Bar'"));

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
	//GrenadeMeshMatStatic = GrenadeMatRef.Object;



	this->Mesh->SetWorldScale3D(MeshScale);

}
void AFoxSmokeGrenadeProjectile::ReceiveBeginPlay()
{
	GrenadeMesh->AddWorldRotation(FRotator(90.0f, 0.0f, 0.0f));
	//GrenadeMesh->AddLocalRotation(FRotator(0.0f, 0.0f, 90.0f));
	//GrenadeMeshMaterial = UMaterialInstanceDynamic::Create(GrenadeMeshMatStatic, this);
	//GrenadeMesh->Materials.Add(GrenadeMeshMaterial);
	GrenadeMesh->SetVisibility(true);
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
