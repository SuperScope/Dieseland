// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Dieseland.h"
#include "DieselandGameMode.h"
#include "DieselandPlayerController.h"
#include "DieselandCharacter.h"
#include "DeathTile.h"

ADieselandGameMode::ADieselandGameMode(const class FPostConstructInitializeProperties& PCIP) : Super(PCIP)
{
	// use our custom PlayerController class
	PlayerControllerClass = ADieselandPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BasicCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
    
    //Find world
    UWorld* const World = GetWorld();
    
    //If world exists spawn tiles and set location of tiles
    if(World){
        ADeathTile* DeathTile1c = (ADeathTile*)World->SpawnActor(ADeathTile::StaticClass());
        ADeathTile* DeathTile2c = (ADeathTile*)World->SpawnActor(ADeathTile::StaticClass());
        ADeathTile* DeathTile3c = (ADeathTile*)World->SpawnActor(ADeathTile::StaticClass());
        ADeathTile* DeathTile4c = (ADeathTile*)World->SpawnActor(ADeathTile::StaticClass());
        ADeathTile* DeathTile5c = (ADeathTile*)World->SpawnActor(ADeathTile::StaticClass());
        ADeathTile* DeathTile6c = (ADeathTile*)World->SpawnActor(ADeathTile::StaticClass());
        ADeathTile* DeathTile7c = (ADeathTile*)World->SpawnActor(ADeathTile::StaticClass());
        
        DeathTile1c->SetActorLocation(FVector(1193.372559, -4332.289062, 0.0));
        DeathTile2c->SetActorLocation(FVector(1193.372559, -730.476562, 0.0));
        DeathTile3c->SetActorLocation(FVector(3740.241211, 1816.390625, 0.0));
        DeathTile4c->SetActorLocation(FVector(1193.372559, 4363.25, 0.0));
        DeathTile5c->SetActorLocation(FVector(-1353.48291, 1816.390625, 0.0));
        DeathTile6c->SetActorLocation(FVector(-3902.357666, -730.476562, 0.0));
        DeathTile7c->SetActorLocation(FVector(-3902.357666, -4332.289062, 0.0));
    }
    
   
}