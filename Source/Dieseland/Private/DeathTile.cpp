

#include "Dieseland.h"
#include "DeathTile.h"
#include "DieselandGameMode.h"


ADeathTile::ADeathTile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
    // Create the Static Mesh Component(Octogon)
    DeathTileMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("DeathTileMesh"));
    
    //Create the Scene Component
    DummyComponent = PCIP.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneComponent"));
    
    //Set Root Component as the Dummy Component
    RootComponent = DummyComponent;
    
    //Find the Octogon mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshOctogon(TEXT("StaticMesh'/Game/Level/Maya_Octogon_export.Maya_Octogon_export'"));
    
    //If there is an Octogon mesh, set it to the tile mesh component
    if(StaticMeshOctogon.Object){
        DeathTileMesh->SetStaticMesh(StaticMeshOctogon.Object);
    }
    
    //Set values for rotation and scale
    DTRotation.Add(0, 22.5, 0);
    DTScale.Set(90, 90, 30);
    
    //Set Dummy Component as parent
    DeathTileMesh->AttachParent = DummyComponent;
    
    //Set Mesh rotation and scale
    DeathTileMesh->SetWorldRotation(DTRotation);
    DeathTileMesh->SetWorldScale3D(DTScale);
    
    //Set booleans at start
    IsTileDown = false;
    ReadyToRise = false;

}

/*void ADeathTile::ReceiveActorBeginOverlap(AActor* Enemy)
{
    Super::ReceiveActorBeginOverlap(Enemy);
    
    if(Enemy->ActorHasTag(FName(TEXT("Enemy"))))
    {
        //EnemiesRemaining = true;
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an ON screen message!"));    }
    
}

void ADeathTile::ReceiveActorEndOverlap(AActor* Enemy)
{
    Super::ReceiveActorEndOverlap(Enemy);
    
    if(Enemy->ActorHasTag(FName(TEXT("Enemy"))))
    {
        //EnemiesRemaining = false;
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an OFF screen message!"));    }
    
}*/

void ADeathTile::SwitchDeathTile()
{
    if(IsTileDown == true)
    {
        World->DestroyActor(this);
        int32 RandomIndex = FMath::RandRange(0, 15);
        UDieselandStaticLibrary::SpawnBlueprint<AActor>(World, Cast <ADieselandGameMode> (World->GetAuthGameMode())->DeathTileArray[RandomIndex], TargetLocation, FRotator(0,0,0));
        
        
    }
}

/*void ADeathTile::EnemyDetection()
{
	DeathTileMesh->SetCollisionProfileName(TEXT("OverlapAll"));
	DeathTileMesh->GetOverlappingActors(EnemiesOnTile);
    
	AActor* CurActor = NULL;
    EnemyFound = false;
	for (int32 b = 0; b < EnemiesOnTile.Num(); b++)
	{
		CurActor = EnemiesOnTile[b];
		if (!CurActor && CurActor->ActorHasTag(FName(TEXT("Enemy"))))
        {
            EnemyFound = true;
            EnemiesRemaining = true;
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy Found!"));
        }
        
        if(b == EnemiesOnTile.Num()-1 && EnemyFound == false)
        {
            EnemiesRemaining = false;
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("There are no enemies!"));
        }
	}

}*/

void ADeathTile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
    
    //EnemyDetection();
    
}



