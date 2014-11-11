

#include "Dieseland.h"
#include "DeathTile.h"
#include "DieselandGameMode.h"
#include "UnrealNetwork.h"



ADeathTile::ADeathTile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
    // Create the Static Mesh Component(Octogon)
    DeathTileMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("DeathTileMesh"));
    
    //Create the Scene Component
    DummyComponent = PCIP.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneComponent"));
    
    //Set Root Component as the Dummy Component
    RootComponent = DummyComponent;

	SphereCollision = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("SphereCollision"));
	SphereCollision->SetCapsuleHalfHeight(1900.0f);
	SphereCollision->SetCapsuleRadius(1900.0f);
	SphereCollision->AttachTo(DeathTileMesh);

	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_Vehicle);
    
    //Find the Octogon mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshOctogon(TEXT("StaticMesh'/Game/PropsDLC/Mesh_Environment_DeathTile_WIP.Mesh_Environment_DeathTile_WIP'"));
	static ConstructorHelpers::FObjectFinder<UClass> SpotlightArrayBlueprint(TEXT("Class'/Game/Level/SpotlightArray_BP.SpotlightArray_BP_C'"));

    //If there is an Octogon mesh, set it to the tile mesh component
    if(StaticMeshOctogon.Object){
        DeathTileMesh->SetStaticMesh(StaticMeshOctogon.Object);
    }

	if (SpotlightArrayBlueprint.Object)
	{
		SpotlightArrayClass = SpotlightArrayBlueprint.Object;
	}

	PrimaryActorTick.bCanEverTick = true;
	
	SetRemoteRoleForBackwardsCompat(ENetRole::ROLE_SimulatedProxy);
	bReplicates = true;
	bReplicateMovement = true;
	
    //Set values for rotation and scale
    DTRotation.Add(0, 22.5, 0);
    DTScale.Set(1, 1, 1);
    
    //Set Dummy Component as parent
	DeathTileMesh->AttachParent = RootComponent;
	//SphereCollision->AttachParent = DummyComponent;
    
    //Set Mesh rotation and scale
    DeathTileMesh->SetWorldRotation(DTRotation);
    DeathTileMesh->SetWorldScale3D(DTScale);
    
    //Set booleans at start
	EnemyDetectionActive = false;

	EnemyCheckInterval = 1.0f;
	EnemyCheckTimer = EnemyCheckInterval;

	TimeLimit = 300.0f;
    Inactive = false;
}

void ADeathTile::SwitchDeathTile()
{
    if(World && Role == ROLE_Authority)
    {
       // World->DestroyActor(this);
       // int32 RandomIndex = FMath::RandRange(0, 15);
        //UDieselandStaticLibrary::SpawnBlueprint<AActor>(World, Cast <ADieselandGameMode> (World->GetAuthGameMode())->DeathTileArray[RandomIndex], TargetLocation, FRotator(0,0,0));
		Cast<ADieselandGameMode>(GetWorld()->GetAuthGameMode())->RespawnTile(this->GetActorLocation());
		//if (SpotlightArray != nullptr)
		//{ 
			//OnDestroyLights();
		//	SpotlightArray->Destroy();
		//}
		Destroy();
    }
}

void ADeathTile::EnemyDetection_Implementation()
{
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	SphereCollision->GetOverlappingActors(EnemiesOnTile);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AActor* CurActor = NULL;
	EnemyFound = false;
	for (int32 b = 0; b < EnemiesOnTile.Num(); b++)
	{
		CurActor = EnemiesOnTile[b];
		if (CurActor != nullptr && CurActor->ActorHasTag(FName(TEXT("Enemy"))))
		{
			EnemyFound = true;
			EnemiesRemaining = true;
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy Found!"));
		}
	}

	if (!EnemyFound && EnemyDetectionActive)
	{
		EnemyDetectionActive = false;
		OnShouldDrop();
	}
}

bool ADeathTile::EnemyDetection_Validate()
{
	return true;
}

void ADeathTile::Tick(float DeltaSeconds)
{
	if (EnemyDetectionActive){

		//Decrement the timer for enemy detection
		EnemyCheckTimer -= DeltaSeconds;

		//If the timer has expired, check to see if there are enemies and reset the timer
		if (EnemyCheckTimer <= 0.0f)
		{
			EnemyDetection();
			EnemyCheckTimer = EnemyCheckInterval;
		}
	}

	if (TimeLimit > 0.0f){
		TimeLimit -= DeltaSeconds;
	}
	else if(TimeLimit <= 0.0 && Inactive == false)
    {
        Inactive = true;
		OnShouldDrop();
		EnemyDetectionActive = false;
	}

	Super::Tick(DeltaSeconds);
}

void ADeathTile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADeathTile, SphereCollision);
	DOREPLIFETIME(ADeathTile, DeathTileMesh);
	DOREPLIFETIME(ADeathTile, SpotlightArray);
	DOREPLIFETIME(ADeathTile, DeathTileIndex);
	DOREPLIFETIME(ADeathTile, EnemyFound);
    DOREPLIFETIME(ADeathTile, Inactive);
	DOREPLIFETIME(ADeathTile, EnemyDetectionActive);
}
