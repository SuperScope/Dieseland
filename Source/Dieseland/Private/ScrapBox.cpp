

#include "Dieseland.h"
#include "DieselandStaticLibrary.h"
#include "ScrapBox.h"
#include "Scrap.h"
#include "UnrealNetwork.h"
#include "DieselandCharacter.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"



AScrapBox::AScrapBox(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	static ConstructorHelpers::FObjectFinder<UDestructibleMesh> DestructibleMesh(TEXT("DestructibleMesh'/Game/PropsDLC/Mesh_Environment_Crate_WIP_DM.Mesh_Environment_Crate_WIP_DM'"));
	SetRemoteRoleForBackwardsCompat(ENetRole::ROLE_SimulatedProxy);
	Mesh = PCIP.CreateDefaultSubobject<UDestructibleComponent>(this, TEXT("Mesh"));
	Mesh->SetDestructibleMesh(DestructibleMesh.Object);
	//Mesh->AttachTo(RootComponent);
	RootComponent = Mesh;
	Mesh->SetIsReplicated(true);
	//Mesh->SetCollisionProfileName(FName(TEXT("BlockAll")));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/Particles/P_Explosion.P_Explosion'"));
	Particle = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleSystem"));
	Particle->Template = ParticleSystemAsset.Object;
	Particle->AttachTo(Mesh);
	Particle->bAutoActivate = false;
	Particle->SetHiddenInGame(false);
	Particle->SetIsReplicated(true);

	static ConstructorHelpers::FObjectFinder<UClass> ScrapBlueprint(TEXT("Class'/Game/Blueprints/Scrap_BP.Scrap_BP_C'"));
	if (ScrapBlueprint.Object)
	{
		ScrapClass = (UClass*)ScrapBlueprint.Object;
	}

	ScrapAmt = 1;

	Tags.Add(FName(TEXT("ScrapBox")));
	bReplicates = true;
	bReplicateMovement = true;
}

void AScrapBox::MultiDestroyCrate_Implementation(AActor* Causer)
{
	//Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	//Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);

	Mesh->ApplyDamage(100.0, GetActorLocation(), FVector(0.0f, 0.0f, 0.0f), 1000.0f);
	//	this->InitialLifeSpan = 2.0f;
	//	this->SetLifeSpan(2.0f);
	if (this != nullptr){
		this->SetLifeSpan(0.5f);
	}
	
	if (Role == ROLE_Authority)
	{

		//Spawn Scrap pieces here
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Cast<ADieselandCharacter>(Causer);
			SpawnParams.Instigator = Instigator;
			for (int32 x = 0; x < ScrapAmt; x++)
			{
				RandomX = FMath::RandRange(-30, 30);
				RandomY = FMath::RandRange(-30, 30);
				AActor* Scrap = UDieselandStaticLibrary::SpawnBlueprint<AActor>(World, ScrapClass, FVector(GetActorLocation().X + RandomX, GetActorLocation().Y + RandomY, GetActorLocation().Z), FRotator(0.0f, 0.0f, 0.0f));
				if (Scrap != nullptr)
				{
					Cast<AScrap>(Scrap)->ScrapValue = FMath::RandRange(20, 30);
				}
				//Alternatively used to spawn c++ class
				//AScrap* const Scrap = World->SpawnActor<AScrap>(AScrap::StaticClass(), FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + (70.0f * x)), FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
			}
		}
	}
}

void AScrapBox::DestroyCrate_Implementation(AActor* Causer)
{
	MultiDestroyCrate(Causer);
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
	DOREPLIFETIME(AScrapBox, ScrapAmt);
	DOREPLIFETIME(AScrapBox, RandomX);
	DOREPLIFETIME(AScrapBox, RandomY);
}
