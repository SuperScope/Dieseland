

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
    
    //Create the Sphere Component to be used for collision
    SphereComponent = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComponent"));
    
    //Find the Octogon mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshOctogon(TEXT("StaticMesh'/Game/Level/Maya_Octogon_export.Maya_Octogon_export'"));
    
    //If there is an Octogon mesh, set it to the tile mesh component
    if(StaticMeshOctogon.Object){
        DeathTileMesh->SetStaticMesh(StaticMeshOctogon.Object);
    }
    
    //Set values for rotation and scale
    DTRotation.Add(0, 22.5, 0);
    DTScale.Set(90, 90, 30);
    SphereScale.Set(734, 734, 63);
    
    //Set Dummy Component as parent
    DeathTileMesh->AttachParent = DummyComponent;
    SphereComponent->AttachParent = DummyComponent; 
    
    //Set Mesh rotation and scale
    DeathTileMesh->SetWorldRotation(DTRotation);
    DeathTileMesh->SetWorldScale3D(DTScale);
    
    //Set Sphere scale
    SphereComponent->SetWorldScale3D(SphereScale);
    
    //Set booleans at start
    IsTileDown = false;
    ReadyToRise = false;
    
    
    
    

}

void ADeathTile::ReceiveActorBeginOverlap(AActor* Enemy)
{
    Super::ReceiveActorBeginOverlap(Enemy);
    
    if(Enemy->ActorHasTag(FName(TEXT("Player"))))
    {
        EnemiesRemaining = true;
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an ON screen message!"));    }
    
}

void ADeathTile::ReceiveActorEndOverlap(AActor* Enemy)
{
    Super::ReceiveActorEndOverlap(Enemy);
    
    if(Enemy->ActorHasTag(FName(TEXT("Player"))))
    {
        EnemiesRemaining = false;
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an OFF screen message!"));    }
    
}

void ADeathTile::SwitchDeathTile()
{
    if(IsTileDown == true)
    {
        World->DestroyActor(this);
        int32 RandomIndex = FMath::RandRange(0, 15);
        UDieselandStaticLibrary::SpawnBlueprint<AActor>(World, Cast <ADieselandGameMode> (World->GetAuthGameMode())->DeathTileArray[RandomIndex], TargetLocation, FRotator(0,0,0));
        
        
    }
}



