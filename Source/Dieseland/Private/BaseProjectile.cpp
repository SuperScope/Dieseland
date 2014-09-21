

#include "Dieseland.h"
#include "BaseProjectile.h"
#include "UnrealNetwork.h"
#include "DieselandCharacter.h"
#include "DieselandPlayerController.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"


ABaseProjectile::ABaseProjectile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/Shapes/Shape_Sphere.Shape_Sphere'"));

	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->SetStaticMesh(StaticMesh.Object);
	RootComponent = Mesh;
	Mesh->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));
	Mesh->SetCollisionProfileName(FName(TEXT("OverlapAll")));
	Mesh->SetHiddenInGame(false);

	ProjCollision = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("ProjectileCollision"));
	ProjCollision->SetCapsuleHalfHeight(50.0f);
	ProjCollision->SetCapsuleRadius(50.0f);
	ProjCollision->SetCollisionProfileName(FName(TEXT("OverlapAll")));

	ProjectileMovement = PCIP.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileMovement"));
	ProjectileMovement->SetIsReplicated(true);
	ProjectileMovement->InitialSpeed = 800.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bInitialVelocityInLocalSpace = false;
	ProjectileMovement->UpdatedComponent = Mesh;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/Test/MovingBulletTest_WIP.MovingBulletTest_WIP'"));
	Particle = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleSystem"));
	Particle->Template = ParticleSystemAsset.Object;
	Particle->AttachTo(Mesh);
	Particle->bAutoActivate = false;
	Particle->SetHiddenInGame(false);

	ProjectileDamage = 10;

	InitialLifeSpan = 10.0f;

	bReplicates = true;
	bReplicateMovement = true;
	Particle->SetIsReplicated(true);
}

void ABaseProjectile::ServerActivateProjectile_Implementation()
{
	Particle->ActivateSystem();
}

bool ABaseProjectile::ServerActivateProjectile_Validate()
{
	return true;
}

void ABaseProjectile::ReceiveActorBeginOverlap(AActor* OtherActor)
{
	Super::ReceiveActorBeginOverlap(OtherActor);
	if (Role == ROLE_Authority)
	{
		if (Cast<ADieselandPlayerController>(GetOwner())->GetPawn() != OtherActor && (OtherActor->ActorHasTag(TEXT("Player")) || OtherActor->ActorHasTag(FName(TEXT("Enemy")))))
		{
		
			Cast<ADieselandCharacter>(Cast<ADieselandPlayerController>(GetOwner())->GetPawn())->EditHealth(-1 * ProjectileDamage, OtherActor);
			if (!Piercing)
			{
				this->Destroy();
			}

		}
	}
	
}

void ABaseProjectile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ABaseProjectile, ProjectileMovement);
	DOREPLIFETIME(ABaseProjectile, Particle);
}
