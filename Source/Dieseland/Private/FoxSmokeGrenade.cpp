// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "FoxSmokeGrenade.h"
#include "DieselandPlayerController.h"
#include "DieselandCharacter.h"
#include "DieselandEnemyBot.h"
#include "UnrealNetwork.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"


AFoxSmokeGrenade::AFoxSmokeGrenade(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	InitialLifeSpan = 2.0f;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/Shapes/Shape_Sphere.Shape_Sphere'"));
	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->SetStaticMesh(StaticMesh.Object);
	RootComponent = Mesh;
	Mesh->SetWorldScale3D(FVector(1.35f, 1.35f, 1.35f));


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

	SmokeCollision = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("SmokeCollision"));
	SmokeCollision->SetCapsuleHalfHeight(250.0f);
	SmokeCollision->SetCapsuleRadius(250.0f);
	SmokeCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	SmokeCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	SmokeCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SmokeCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	SmokeCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
	SmokeCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	SmokeCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	SmokeCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SmokeCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	SmokeCollision->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	SmokeCollision->SetCollisionEnabled((ECollisionEnabled::QueryOnly));
	SmokeCollision->AttachTo(Mesh);

	SmokeEffect = PCIP.CreateDefaultSubobject<UPostProcessComponent>(this, TEXT("Smoke Effect"));
	SmokeEffect->AttachParent = SmokeCollision;
	SmokeEffect->bUnbound = false;

	//static ConstructorHelpers::FObjectFinder<UTexture> SmokeReference(TEXT("Texture2D'/Game/Particles/Test/Effects/Textures/T_Smoke_Fill_NEW.T_Smoke_Fill_NEW'"));
	SmokeTexture = PCIP.CreateDefaultSubobject<UTexture2D>(this, TEXT("Texture2D'/Game/Particles/Test/Effects/Textures/T_Smoke_Fill_NEW.T_Smoke_Fill_NEW'"));

	//SmokeTexture = SmokeReference.Object;

	//SmokeEffect->SetWorldScale3D(FVector(0.05f, 0.05f, 0.05f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_FoxSmokeGrenade_WIP.Unreal_Particle_FoxSmokeGrenade_WIP'"));


	Particle = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleSystem"));
	Particle->Template = ParticleSystemAsset.Object;
	Particle->AttachTo(Mesh);
	Particle->bAutoActivate = false;
	Particle->SetHiddenInGame(false);

	bReplicates = true;
	bReplicateMovement = true;
	Particle->SetIsReplicated(true);
}

void AFoxSmokeGrenade::ServerActivateProjectile_Implementation()
{
	Particle->ActivateSystem();
}

bool AFoxSmokeGrenade::ServerActivateProjectile_Validate()
{
	return true;
}

void AFoxSmokeGrenade::ServerAdjustPostProcessing_Implementation(AActor* OtherActor)
{

	Cast<ADieselandCharacter>(OtherActor)->TopDownCameraComponent->PostProcessSettings.bOverride_SceneFringeIntensity = 1;
	Cast<ADieselandCharacter>(OtherActor)->TopDownCameraComponent->PostProcessSettings.SceneFringeIntensity = 1.1f;
	Cast<ADieselandCharacter>(OtherActor)->TopDownCameraComponent->PostProcessSettings.bOverride_GrainIntensity = 1;
	Cast<ADieselandCharacter>(OtherActor)->TopDownCameraComponent->PostProcessSettings.GrainIntensity = 1.25f;
	Cast<ADieselandCharacter>(OtherActor)->TopDownCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = 1;
	Cast<ADieselandCharacter>(OtherActor)->TopDownCameraComponent->PostProcessSettings.VignetteIntensity = 1.2f;
	Cast<ADieselandCharacter>(OtherActor)->TopDownCameraComponent->PostProcessSettings.bOverride_ColorGradingIntensity = 1.0f;
	Cast<ADieselandCharacter>(OtherActor)->TopDownCameraComponent->PostProcessSettings.ColorGradingIntensity = 0.8f;
	Cast<ADieselandCharacter>(OtherActor)->TopDownCameraComponent->PostProcessSettings.ColorGradingLUT = SmokeTexture;
	Cast<ADieselandCharacter>(OtherActor)->TopDownCameraComponent->PostProcessSettings.bOverride_ColorGradingLUT = 1.0f;
	Cast<ADieselandCharacter>(OtherActor)->StatusEffects.Add(FString("SmokeScreen"));
}

bool AFoxSmokeGrenade::ServerAdjustPostProcessing_Validate(AActor* OtherActor)
{
	return true;
}

void AFoxSmokeGrenade::ReceiveActorBeginOverlap(AActor* OtherActor)
{
	Super::ReceiveActorBeginOverlap(OtherActor);

	if (Role == ROLE_Authority)
	{
		if (OtherActor->ActorHasTag(TEXT("Player")) && !OtherActor->ActorHasTag("Projectile"))
		{
			//Cast<ADieselandCharacter>(Cast<ADieselandPlayerController>(GetOwner())->GetPawn())->EditHealth(-1 * ProjectileDamage, OtherActor);
			Cast<ADieselandCharacter>(OtherActor)->StatusEffects.Add(FString("Slowed"));
			Cast<ADieselandCharacter>(OtherActor)->SlowRemaining = 3.0f;
			Cast<ADieselandCharacter>(OtherActor)->CharacterMovement->MaxWalkSpeed = Cast<ADieselandCharacter>(OtherActor)->MoveSpeed * 0.30f;
			ServerAdjustPostProcessing(OtherActor);
				/*
				Cast<ADieselandCharacter>(OtherActor)->StatusEffects.Add(FString("Stunned"));
			Cast<ADieselandCharacter>(OtherActor)->StatusEffects.Add(FString("Charmed"));
			Cast<ADieselandCharacter>(OtherActor)->VectorDirection = Cast<ADieselandPlayerController>(GetOwner())->GetPawn()->GetActorLocation() - Cast<ADieselandCharacter>(OtherActor)->GetActorLocation();
			Cast<ADieselandCharacter>(OtherActor)->VectorDirection.Normalize();*/
			
		}
		if (OtherActor->ActorHasTag(TEXT("Enemy")))
		{
			Cast<ADieselandEnemyBot>(OtherActor)->StatusEffects.Add(FString("Slowed"));
			Cast<ADieselandEnemyBot>(OtherActor)->SlowRemaining = 3.0f;
			Cast<ADieselandEnemyBot>(OtherActor)->CharacterMovement->MaxWalkSpeed = 100.0f;
		}
	}
}

void AFoxSmokeGrenade::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(AFoxSmokeGrenade, Particle);
	//DOREPLIFETIME(AFoxSmokeGrenade, ServerAdjustPostProcessing);

}


