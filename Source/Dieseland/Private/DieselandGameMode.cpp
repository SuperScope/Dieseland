// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Dieseland.h"
#include "DieselandGameMode.h"
#include "DieselandGameState.h"
#include "DieselandPlayerController.h"
#include "DieselandCharacter.h"
#include "MayhemCharacter.h"
#include "DeathTile.h"
#include "UnrealNetwork.h"
#include "DieselandStaticLibrary.h"

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

	GameStateClass = ADieselandGameState::StaticClass();
    
    static ConstructorHelpers::FObjectFinder<UBlueprint> TileClass1(TEXT("Blueprint'/Game/Level/DeathTiles/DeathTile_v1.DeathTile_v1'"));
    static ConstructorHelpers::FObjectFinder<UBlueprint> TileClass2(TEXT("Blueprint'/Game/Level/DeathTiles/DeathTile_v2.DeathTile_v2'"));
    static ConstructorHelpers::FObjectFinder<UBlueprint> TileClass3(TEXT("Blueprint'/Game/Level/DeathTiles/DeathTile_v3.DeathTile_v3'"));
    static ConstructorHelpers::FObjectFinder<UBlueprint> TileClass4(TEXT("Blueprint'/Game/Level/DeathTiles/DeathTile_v4.DeathTile_v4'"));
    static ConstructorHelpers::FObjectFinder<UBlueprint> TileClass5(TEXT("Blueprint'/Game/Level/DeathTiles/DeathTile_v5.DeathTile_v5'"));
    static ConstructorHelpers::FObjectFinder<UBlueprint> TileClass6(TEXT("Blueprint'/Game/Level/DeathTiles/DeathTile_v6.DeathTile_v6'"));
    static ConstructorHelpers::FObjectFinder<UBlueprint> TileClass7(TEXT("Blueprint'/Game/Level/DeathTiles/DeathTile_v7.DeathTile_v7'"));
    static ConstructorHelpers::FObjectFinder<UBlueprint> TileClass8(TEXT("Blueprint'/Game/Level/DeathTiles/DeathTile_v8.DeathTile_v8'"));
    static ConstructorHelpers::FObjectFinder<UBlueprint> TileClass9(TEXT("Blueprint'/Game/Level/DeathTiles/DeathTile_v9.DeathTile_v9'"));
    static ConstructorHelpers::FObjectFinder<UBlueprint> TileClass10(TEXT("Blueprint'/Game/Level/DeathTiles/DeathTile_v10.DeathTile_v10'"));
    static ConstructorHelpers::FObjectFinder<UBlueprint> TileClass11(TEXT("Blueprint'/Game/Level/DeathTiles/DeathTile_v11.DeathTile_v11'"));
    static ConstructorHelpers::FObjectFinder<UBlueprint> TileClass12(TEXT("Blueprint'/Game/Level/DeathTiles/DeathTile_v12.DeathTile_v12'"));
    static ConstructorHelpers::FObjectFinder<UBlueprint> TileClass13(TEXT("Blueprint'/Game/Level/DeathTiles/DeathTile_v13.DeathTile_v13'"));
    static ConstructorHelpers::FObjectFinder<UBlueprint> TileClass14(TEXT("Blueprint'/Game/Level/DeathTiles/DeathTile_v14.DeathTile_v14'"));
    static ConstructorHelpers::FObjectFinder<UBlueprint> TileClass15(TEXT("Blueprint'/Game/Level/DeathTiles/DeathTile_v15.DeathTile_v15'"));
    static ConstructorHelpers::FObjectFinder<UBlueprint> TileClass16(TEXT("Blueprint'/Game/Level/DeathTiles/DeathTile_v16.DeathTile_v16'"));
	
	static ConstructorHelpers::FObjectFinder<UBlueprint> MayhemBPClass(TEXT("Blueprint'/Game/Blueprints/Players/Mayhem_BP.Mayhem_BP'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> EngletonBPClass(TEXT("Blueprint'/Game/Blueprints/Engleton.Engleton'"));

	if (MayhemBPClass.Object != nullptr && EngletonBPClass.Object != nullptr)
	{
		MayhemClass = MayhemBPClass.Object->GeneratedClass;
		EngletonClass = EngletonBPClass.Object->GeneratedClass;
	}
    
	if (TileClass1.Object != nullptr && TileClass2.Object != nullptr)
	{
		DeathTileArray.Add(TileClass1.Object->GeneratedClass);
		DeathTileArray.Add(TileClass2.Object->GeneratedClass);
		DeathTileArray.Add(TileClass3.Object->GeneratedClass);
		DeathTileArray.Add(TileClass4.Object->GeneratedClass);
		DeathTileArray.Add(TileClass5.Object->GeneratedClass);
		DeathTileArray.Add(TileClass6.Object->GeneratedClass);
		DeathTileArray.Add(TileClass7.Object->GeneratedClass);
		DeathTileArray.Add(TileClass8.Object->GeneratedClass);
		DeathTileArray.Add(TileClass9.Object->GeneratedClass);
		DeathTileArray.Add(TileClass10.Object->GeneratedClass);
		DeathTileArray.Add(TileClass11.Object->GeneratedClass);
		DeathTileArray.Add(TileClass12.Object->GeneratedClass);
		DeathTileArray.Add(TileClass13.Object->GeneratedClass);
		DeathTileArray.Add(TileClass14.Object->GeneratedClass);
		DeathTileArray.Add(TileClass15.Object->GeneratedClass);
		DeathTileArray.Add(TileClass16.Object->GeneratedClass);
	}

    LocationArray.Add(FVector(1193.372559, -4332.289062, -1500.0));
    LocationArray.Add(FVector(1193.372559, -730.476562, -1500.0));
    LocationArray.Add(FVector(3740.241211, 1816.390625, -1500.0));
    LocationArray.Add(FVector(1193.372559, 4363.25, -1500.0));
    LocationArray.Add(FVector(-1353.48291, 1816.390625, -1500.0));
    LocationArray.Add(FVector(-3902.357666, -730.476562, -1500.0));
    LocationArray.Add(FVector(-3902.357666, -4332.289062, -1500.0));

    
	
    
	GameTimer = 9999.0f;

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void ADieselandGameMode::ReceiveBeginPlay()
{
	//StartGame();
}

void ADieselandGameMode::Tick(float DeltaSeconds)
{
	GameTimer -= DeltaSeconds;

	if (GameTimer <= 0.0f)
	{
		EndGame();
	}

	Super::Tick(DeltaSeconds);
}

void ADieselandGameMode::StartGame_Implementation()
{
	//Find world
	UWorld* const World = GetWorld();

	//If world exists spawn tiles and set location of tiles
	if (World){

		for (int32 i = 0; i <= 6; i++)
		{
			int32 RandomIndex = FMath::RandRange(0, 15);
			UDieselandStaticLibrary::SpawnBlueprint<AActor>(World, DeathTileArray[RandomIndex], LocationArray[i], FRotator(0, 0, 0));

		}
	}
}

void ADieselandGameMode::EndGame_Implementation()
{
	FString WinningName;
	if (Cast<ADieselandGameState>(GameState)->WinningPlayer != nullptr)
	{
		WinningName = Cast<ADieselandGameState>(GameState)->WinningPlayer->GetName();
	}

	GEngine->AddOnScreenDebugMessage(21, 10.0f, FColor::Yellow, WinningName);
}

bool ADieselandGameMode::EndGame_Validate()
{
	return true;
}

APawn* ADieselandGameMode::SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot)
{
	//NewPlayer->PlayerState->PlayerId
	// don't allow pawn to be spawned with any pitch or roll
	FRotator StartRotation(ForceInit);
	StartRotation.Yaw = StartSpot->GetActorRotation().Yaw;
	FVector StartLocation = StartSpot->GetActorLocation();

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Instigator;
	APawn* ResultPawn;

	if (PlayersSpawned % 2 == 0)
	{
		ResultPawn = GetWorld()->SpawnActor<APawn>(MayhemClass, StartLocation, StartRotation, SpawnInfo);
	}
	else
	{
		ResultPawn = GetWorld()->SpawnActor<APawn>(EngletonClass, StartLocation, StartRotation, SpawnInfo);
	}

	if (ResultPawn != NULL)
	{
		PlayersSpawned++;
	}
	return ResultPawn;

}

void ADieselandGameMode::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ADieselandGameMode, GameTimer);
	DOREPLIFETIME(ADieselandGameMode, DeathTileArray);
	DOREPLIFETIME(ADieselandGameMode, LocationArray);
}
