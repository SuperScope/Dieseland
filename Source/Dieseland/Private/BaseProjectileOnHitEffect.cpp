// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "BaseProjectileOnHitEffect.h"
#include "UnrealNetwork.h"


ABaseProjectileOnHitEffect::ABaseProjectileOnHitEffect(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//set the lifetime of the effect with this
	InitialLifeSpan = 0.3f;

	//set the size of the effect and set the mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/Shapes/Shape_Sphere.Shape_Sphere'"));
	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->SetStaticMesh(StaticMesh.Object);
	RootComponent = Mesh;
	//set the size with this
	Mesh->SetWorldScale3D(FVector(0.25f, 0.25f, 0.25f));
	

	//setting collision channel responses
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	Mesh->SetHiddenInGame(true);
	Mesh->SetIsReplicated(true);


	//Get out particle effect for on hit here, you'll need
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/Test/TempCrazylaser.TempCrazylaser'"));


	Particle = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleSystem"));
	Particle->Template = ParticleSystemAsset.Object;
	Particle->AttachTo(Mesh);
	Particle->bAutoActivate = false;
	Particle->SetHiddenInGame(false);
	Particle->SetIsReplicated(true);
	bReplicates = true;
}

void ABaseProjectileOnHitEffect::ServerActivateProjectile_Implementation()
{
	Particle->ActivateSystem();
}

bool ABaseProjectileOnHitEffect::ServerActivateProjectile_Validate()
{
	return true;
}

void ABaseProjectileOnHitEffect::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}



