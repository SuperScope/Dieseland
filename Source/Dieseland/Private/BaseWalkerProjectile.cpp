

#include "Dieseland.h"
#include "BaseWalkerProjectile.h"
#include "UnrealNetwork.h"
#include "DieselandEnemyBot.h"
#include "DieselandEnemyAI.h"
#include "DieselandCharacter.h"
#include "DieselandPlayerController.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"



ABaseWalkerProjectile::ABaseWalkerProjectile(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
	IsAI = true;
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
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	Mesh->SetHiddenInGame(true);

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
	ProjCollision->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);


	ProjectileMovement = PCIP.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileMovement"));
	ProjectileMovement->SetIsReplicated(true);
	ProjectileMovement->InitialSpeed = 1600.0f;
//	ProjectileMovement->Velocity
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bInitialVelocityInLocalSpace = false;
	ProjectileMovement->UpdatedComponent = Mesh;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/Test/MovingBulletTest_WIP.MovingBulletTest_WIP'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> LaunchParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/Test/TempCrazylaser.TempCrazylaser'"));
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
}

void ABaseWalkerProjectile::ServerActivateProjectile_Implementation()
{

	Particle->ActivateSystem();
}

bool ABaseWalkerProjectile::ServerActivateProjectile_Validate()
{
	return true;
}

void ABaseWalkerProjectile::ReceiveActorBeginOverlap(AActor* OtherActor)
{
	Super::ReceiveActorBeginOverlap(OtherActor);

	if (OtherActor == nullptr || OtherActor == NULL || Role == NULL){
		return;
	}
	if (IsAI == false)
	{
		if (Role == ROLE_Authority && Cast<ADieselandPlayerController>(GetOwner())->GetPawn() != OtherActor)
		{
			if (OtherActor->ActorHasTag(TEXT("Player")) || OtherActor->ActorHasTag(TEXT("Enemy")) || OtherActor->ActorHasTag(TEXT("ScrapBox")))
			{
				Cast<ADieselandCharacter>(Cast<ADieselandPlayerController>(GetOwner())->GetPawn())->EditHealth(-1 * ProjectileDamage, OtherActor);
				if (!Piercing)
				{
					this->Destroy();
				}

			}
			else if (!OtherActor->ActorHasTag(TEXT("Projectile")))
			{
				this->Destroy();
			}
		}
	}//end is AI

	else if (IsAI)
	{
		if (Role == ROLE_Authority && Cast<ADieselandEnemyAI>(GetOwner())->GetPawn() != OtherActor)
		{

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Thasdasdsas an on screen message!"));
			if (OtherActor->ActorHasTag(TEXT("Player")) || OtherActor->ActorHasTag(TEXT("Enemy")) || OtherActor->ActorHasTag(TEXT("ScrapBox")))
			{
				Cast<ADieselandEnemyBot>(Cast<ADieselandEnemyAI>(GetOwner())->GetPawn())->EditHealth(-1 * ProjectileDamage, OtherActor);
				if (!Piercing)
				{
					this->Destroy();
				}

			}
			else if (!OtherActor->ActorHasTag(TEXT("Projectile")))
			{
				this->Destroy();
			}
		}
	}//end is AI*/
}

void ABaseWalkerProjectile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ABaseWalkerProjectile, ProjectileMovement);
	DOREPLIFETIME(ABaseWalkerProjectile, Particle);

}
