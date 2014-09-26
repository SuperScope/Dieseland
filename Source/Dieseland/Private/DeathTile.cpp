

#include "Dieseland.h"
#include "DeathTile.h"


ADeathTile::ADeathTile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	Tile = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Death Tile"));
}


