

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
    
    //Set initial values for variables
    CanSpawn = true;
    Spawn = false;
    BossTag = "";
    CheckForBoss = false;
    IsBossAlive = false;
    BossDead = false;

}

void ABossTiles::Tick(float DeltaSeconds)
{
    //If GameMode reference has not been set, set it
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
    
    //Make sure Server is the only one to set these variables
    if(Role == ROLE_Authority)
    {
        //If boss timer hits 0 and we can spawn a boss
        if(GameMode->BossTimer <= 295.0f && CanSpawn == true)
        {
            //Set ZoneTag based on RandomSpawnIndex
            if(GameMode->RandomSpawnIndex == 0)
            {
                ZoneTag = "Zone1";
            }
            else if(GameMode->RandomSpawnIndex == 1)
            {
                ZoneTag = "Zone2";
            }
            else
            {
                ZoneTag = "Zone3";
            }
            
            //Make sure only the tile spawning the boss turns this off
            if(this->ActorHasTag(ZoneTag))
            {
                CanSpawn = false;
            }
        
            //Set BossTag based on RandomBossIndex
            if(GameMode->RandomBossIndex == 0)
            {
                BossTag = "King";
            }
            else
            {
                BossTag = "Queen";
            }
        
            //Start the spawning process in blueprints
            Spawn = true;
            CheckForBoss = true;
        
        }
    
        //If boss has died, reset these values
        if(BossDead == true)
        {
            BossDead = false;
            GameMode->BossTimer = 300.0f;
            CanSpawn = true;
            GameMode->CanSpawn = true;
        
        }
    }
    
    Super::Tick(DeltaSeconds);
}

void ABossTiles::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
	DOREPLIFETIME(ABossTiles, BossTag);
    DOREPLIFETIME(ABossTiles, ZoneTag);
	DOREPLIFETIME(ABossTiles, CanSpawn);
	DOREPLIFETIME(ABossTiles, BossDead);
	DOREPLIFETIME(ABossTiles, IsBossAlive);
	DOREPLIFETIME(ABossTiles, CheckForBoss);
	DOREPLIFETIME(ABossTiles, BossStart);
    DOREPLIFETIME(ABossTiles, BossFinish);
    DOREPLIFETIME(ABossTiles, Spawn);
}


