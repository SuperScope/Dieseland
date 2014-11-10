

#include "Dieseland.h"
#include "BossTiles.h"
#include "DieselandGameMode.h"


ABossTiles::ABossTiles(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

    SetRemoteRoleForBackwardsCompat(ENetRole::ROLE_SimulatedProxy);
	bReplicates = true;
	bReplicateMovement = true;
    PrimaryActorTick.bCanEverTick = true;
    
    CanSpawn = true;
    BossTimer = 300.0f;
    Spawn = false;
    BossTag = "";
    CheckForBoss = false;
    IsBossAlive = false;
    BossDead = false;
    
    /*if(GetWorld() != nullptr)
    {
        if(Role == ROLE_Authority)
        {
            GameMode = Cast<ADieselandGameMode>(GetWorld()->GetAuthGameMode());
        }
    }*/
    

}

void ABossTiles::Tick(float DeltaSeconds)
{
     //BossTimer= GameMode->BossTimer;
    
    if(GameMode == nullptr)
    {
        if(GetWorld() != nullptr)
        {
            if(Role == ROLE_Authority)
            {
                GameMode = Cast<ADieselandGameMode>(GetWorld()->GetAuthGameMode());
            }
        }
    }
    
    if(GameMode->BossTimer <= 295.0f && CanSpawn == true)
    {
        
        CanSpawn = false;
        
        if(GameMode->RandomBossIndex == 0)
        {
            BossTag = "Zone1";
        }
        else if(GameMode->RandomBossIndex == 1)
        {
            BossTag = "Zone2";
        }
        else
        {
            BossTag = "Zone3";
        }
        GEngine->AddOnScreenDebugMessage(2, 10.0f, FColor::Red, FString("Spawn"));
        Spawn = true;
        CheckForBoss = true;
        BossStart = true;
    }
    
    if(BossDead == true)
    {
        BossDead = false;
        GameMode->BossTimer = 300.0f;
        CanSpawn = true;
        GameMode->CanSpawn = true;
        BossFinish = true;
    }
    
    Super::Tick(DeltaSeconds);
}


