

#include "Dieseland.h"
#include "DeathTile.h"


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
    
    //Set Dummy Component as parent
    DeathTileMesh->AttachParent = DummyComponent;
    SphereComponent->AttachParent = DummyComponent; 
    
    //Set Mesh rotation and scale
    DeathTileMesh->SetWorldRotation(DTRotation);
    DeathTileMesh->SetWorldScale3D(DTScale);
    
    //Check for enemies on tile
    //TO DO: Function to detect collision and set EnemiesRemaining bool
    
    //Check to see if enemies remaining
    if(!EnemiesRemaining)
    {
        //Drop tile and switch it
    }
    
    

}

void ADeathTile::OnOverlap(AActor* Enemy, UPrimitiveComponent* Sphere)
{
    if(Enemy->ActorHasTag(FName(TEXT("Enemy"))))
    {
        EnemiesRemaining = true;
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("The player is colliding!"));
    }
    else
    {
        EnemiesRemaining = false;
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("The player is not colliding!"));
    }
    
}


