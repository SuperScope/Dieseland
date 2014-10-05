

#include "Dieseland.h"
#include "ScrapBox.h"
#include "UnrealNetwork.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"


AScrapBox::AScrapBox(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	static ConstructorHelpers::FObjectFinder<UDestructibleMesh> DestructibleMesh(TEXT("DestructibleMesh'/Game/Shapes/Shape_Cube_DM.Shape_Cube_DM'"));

	Mesh = PCIP.CreateDefaultSubobject<UDestructibleComponent>(this, TEXT("Mesh"));
	Mesh->SetDestructibleMesh(DestructibleMesh.Object);
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName(FName(TEXT("BlockAll")));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/P_Explosion.P_Explosion'"));
	Particle = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleSystem"));
	Particle->Template = ParticleSystemAsset.Object;
	Particle->AttachTo(Mesh);
	Particle->bAutoActivate = false;
	Particle->SetHiddenInGame(false);

	Tags.Add(FName(TEXT("ScrapBox")));
	
	bReplicates = true;
}

void AScrapBox::ReceiveActorBeginOverlap(AActor* OtherActor)
{

}

void AScrapBox::DestroyCrate_Implementation(AActor* Causer)
{
	Mesh->ApplyDamage(100.0, GetActorLocation(), FVector(0.0f, 0.0f, 0.0f), 1000.0f);

	//Spawn Scrap pieces here
}

bool AScrapBox::DestroyCrate_Validate(AActor* Causer)
{
	return true;
}

void AScrapBox::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(AScrapBox, Particle);
}
