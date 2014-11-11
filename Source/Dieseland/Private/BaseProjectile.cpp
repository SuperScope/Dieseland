

#include "Dieseland.h"
#include "BaseProjectile.h"
#include "UnrealNetwork.h"
#include "DieselandEnemyBot.h"
#include "DieselandCharacter.h"
#include "DieselandEnemyAI.h"
#include "DieselandCharacter.h"
#include "DieselandPlayerController.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "StrykerCharacter.h"


ABaseProjectile::ABaseProjectile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/Shapes/Shape_Sphere.Shape_Sphere'"));
	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->SetStaticMesh(StaticMesh.Object);
	RootComponent = Mesh;
	Mesh->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));
	//Mesh->SetCollisionProfileName(FName(TEXT("OverlapAll")));
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

	ProjCollision = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("ProjectileCollision"));
	ProjCollision->SetCapsuleHalfHeight(50.0f);
	ProjCollision->SetCapsuleRadius(50.0f);
	ProjCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	ProjCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	ProjCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ProjCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	ProjCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	ProjCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	ProjCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	ProjCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	ProjCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	ProjCollision->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	

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
	Tags.Add(FName("Projectile"));

	bReplicates = true;
	bReplicateMovement = true;
	Particle->SetIsReplicated(true);
	//ReplicatedMovement.LinearVelocity.Z ;
	
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

	//if (OtherActor == nullptr || OtherActor == NULL){
//		return;
//	}
	if (IsPoison == false){
		if (Role == ROLE_Authority && Cast<ADieselandPlayerController>(GetOwner())->GetPawn() != OtherActor)
		{
			if (OtherActor->ActorHasTag(TEXT("Player")) || OtherActor->ActorHasTag(TEXT("Enemy")) || OtherActor->ActorHasTag(TEXT("ScrapBox")))
			{
				Cast<ADieselandCharacter>(Cast<ADieselandPlayerController>(GetOwner())->GetPawn())->EditHealth(-1 * ProjectileDamage, OtherActor);
				if (!Piercing)				{
					this->Destroy();
				}
			}
			else if (!OtherActor->ActorHasTag(TEXT("Projectile")))
			{
				if (!Piercing){
					this->Destroy();
				}
			}
		}
	}
	//this is for stryker only, ispoisoin is set to true on the poison projectiles
	if (IsPoison)
	{
		if (Role == ROLE_Authority && Cast<ADieselandPlayerController>(GetOwner())->GetPawn() != OtherActor)
		{
			if (OtherActor->ActorHasTag(TEXT("Player")) || OtherActor->ActorHasTag(TEXT("Enemy")) || OtherActor->ActorHasTag(TEXT("ScrapBox")))
			{
				Cast<ADieselandCharacter>(Cast<ADieselandPlayerController>(GetOwner())->GetPawn())->EditSpeedDamage(PoisionSlowAmount, PoisionDamageReductionAmount, OtherActor);
				if (!Piercing)				{
					this->Destroy();
				}
			}
			else if (!OtherActor->ActorHasTag(TEXT("Projectile")))
			{
				if (!Piercing){
					this->Destroy();
				}
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
