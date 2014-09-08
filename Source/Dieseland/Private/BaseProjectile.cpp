

#include "Dieseland.h"
#include "BaseProjectile.h"
#include "UnrealNetwork.h"
#include "DieselandCharacter.h"


ABaseProjectile::ABaseProjectile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/Shapes/Shape_Sphere.Shape_Sphere'"));

	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->SetStaticMesh(StaticMesh.Object);
	RootComponent = Mesh;
	Mesh->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));
	Mesh->SetCollisionProfileName(FName(TEXT("OverlapAll")));

	ProjCollision = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("ProjectileCollision"));
	ProjCollision->SetCapsuleHalfHeight(50.0f);
	ProjCollision->SetCapsuleRadius(50.0f);
	ProjCollision->SetCollisionProfileName(FName(TEXT("OverlapAll")));

	ProjectileMovement = PCIP.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileMovement"));
	ProjectileMovement->SetIsReplicated(true);
	ProjectileMovement->InitialSpeed = 800.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	InitialLifeSpan = 10.0f;

	bReplicates = true;
}

void ABaseProjectile::ReceiveActorBeginOverlap(AActor* OtherActor)
{
	Super::ReceiveActorBeginOverlap(OtherActor);
	if (GetOwner() != OtherActor)
	{
		if (Role == ROLE_Authority)
		{
			Cast<ADieselandCharacter>(GetOwner())->EditHealth(-25, OtherActor);
		}
		this->Destroy();
	}
}

void ABaseProjectile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ABaseProjectile, ProjectileMovement);
}
