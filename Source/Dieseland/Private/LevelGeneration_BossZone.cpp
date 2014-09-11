

#include "Dieseland.h"
#include "LevelGeneration_BossZone.h"


ALevelGeneration_BossZone::ALevelGeneration_BossZone(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
    
   Ground = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Ground"));

}


