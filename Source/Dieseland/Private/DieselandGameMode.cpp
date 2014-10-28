// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Dieseland.h"
#include "DieselandGameMode.h"
#include "DieselandGameState.h"
#include "DieselandPlayerController.h"
#include "DieselandCharacter.h"
#include "MayhemCharacter.h"
#include "DeathTile.h"
#include "BossGates.h"
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

	static ConstructorHelpers::FObjectFinder<UClass> TileClass1(TEXT("Class'/Game/Level/DeathTiles/DeathTile_v1.DeathTile_v1_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> TileClass2(TEXT("Class'/Game/Level/DeathTiles/DeathTile_v2.DeathTile_v2_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> TileClass3(TEXT("Class'/Game/Level/DeathTiles/DeathTile_v3.DeathTile_v3_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> TileClass4(TEXT("Class'/Game/Level/DeathTiles/DeathTile_v4.DeathTile_v4_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> TileClass5(TEXT("Class'/Game/Level/DeathTiles/DeathTile_v5.DeathTile_v5_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> TileClass6(TEXT("Class'/Game/Level/DeathTiles/DeathTile_v6.DeathTile_v6_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> TileClass7(TEXT("Class'/Game/Level/DeathTiles/DeathTile_v7.DeathTile_v7_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> TileClass8(TEXT("Class'/Game/Level/DeathTiles/DeathTile_v8.DeathTile_v8_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> TileClass9(TEXT("Class'/Game/Level/DeathTiles/DeathTile_v9.DeathTile_v9_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> TileClass10(TEXT("Class'/Game/Level/DeathTiles/DeathTile_v10.DeathTile_v10_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> TileClass11(TEXT("Class'/Game/Level/DeathTiles/DeathTile_v11.DeathTile_v11_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> TileClass12(TEXT("Class'/Game/Level/DeathTiles/DeathTile_v12.DeathTile_v12_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> TileClass13(TEXT("Class'/Game/Level/DeathTiles/DeathTile_v13.DeathTile_v13_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> TileClass14(TEXT("Class'/Game/Level/DeathTiles/DeathTile_v14.DeathTile_v14_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> TileClass15(TEXT("Class'/Game/Level/DeathTiles/DeathTile_v15.DeathTile_v15_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> TileClass16(TEXT("Class'/Game/Level/DeathTiles/DeathTile_v16.DeathTile_v16_C'"));

	static ConstructorHelpers::FObjectFinder<UClass> MayhemBPClass(TEXT("Class'/Game/Blueprints/Players/Mayhem_BP.Mayhem_BP_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> EngletonBPClass(TEXT("Class'/Game/Blueprints/Engleton.Engleton_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> StrykerBPClass(TEXT("Class'/Game/Blueprints/Players/Stryker_BP.Stryker_BP_C'"));
    static ConstructorHelpers::FObjectFinder<UClass> HighlanderBPClass(TEXT("Class'/Game/Blueprints/Enemies/DieselandHighLanderKingBP.DieselandHighLanderKingBP_C'"));
    
    
    if(HighlanderBPClass.Object)
    {
        HighlanderKing = HighlanderBPClass.Object;
    }

	if (MayhemBPClass.Object)
	{
		MayhemClass = MayhemBPClass.Object;
		EngletonClass = EngletonBPClass.Object;
		StrykerClass = StrykerBPClass.Object;
	}
    
	if (TileClass1.Object)
	{
		DeathTileClassArray.Add(TileClass1.Object);
		DeathTileClassArray.Add(TileClass2.Object);
		DeathTileClassArray.Add(TileClass3.Object);
		DeathTileClassArray.Add(TileClass4.Object);
		DeathTileClassArray.Add(TileClass5.Object);
		DeathTileClassArray.Add(TileClass6.Object);
		DeathTileClassArray.Add(TileClass7.Object);
		DeathTileClassArray.Add(TileClass8.Object);
		DeathTileClassArray.Add(TileClass9.Object);
		DeathTileClassArray.Add(TileClass10.Object);
		DeathTileClassArray.Add(TileClass11.Object);
		DeathTileClassArray.Add(TileClass12.Object);
		DeathTileClassArray.Add(TileClass13.Object);
		DeathTileClassArray.Add(TileClass14.Object);
		DeathTileClassArray.Add(TileClass15.Object);
		DeathTileClassArray.Add(TileClass16.Object);
	}

    LocationArray.Add(FVector(1193.372559, -4332.289062, -1500.0));
    LocationArray.Add(FVector(1193.372559, -730.476562, -1500.0));
    LocationArray.Add(FVector(3740.241211, 1816.390625, -1500.0));
    LocationArray.Add(FVector(1193.372559, 4363.25, -1500.0));
    LocationArray.Add(FVector(-1353.48291, 1816.390625, -1500.0));
    LocationArray.Add(FVector(-3902.357666, -730.476562, -1500.0));
    LocationArray.Add(FVector(-3902.357666, -4332.289062, -1500.0));
    
    //Boss Zone 1
    BossSpawnArray.Add(FVector(-910.0, -9200.0, 350.0));
    //Boss Zone 2
    BossSpawnArray.Add(FVector(6340.0, -4710.0, 350.0));
    //Boss Zone 3
    BossSpawnArray.Add(FVector(-3980.0, 8130.0, 350.0));

    
	GameTimer = 9999.0f;
    BossTimer = 300.0f;
    canSpawn = true;
    Gate1 = false;
    Gate2 = false;
    Gate3 = false;

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void ADieselandGameMode::ReceiveBeginPlay()
{
	StartGame();
}

void ADieselandGameMode::Tick(float DeltaSeconds)
{
	GameTimer -= DeltaSeconds;
    BossTimer -= DeltaSeconds;

	if (GameTimer <= 0.0f)
	{
		EndGame();
	}
    
    if (BossTimer <= 290.0f && canSpawn == true)
    {
        canSpawn = false;
        SpawnBoss();
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
			DeathTileArray.Add(UDieselandStaticLibrary::SpawnBlueprint<AActor>(World, DeathTileClassArray[RandomIndex], LocationArray[i], FRotator(0, 0, 0)));
			Cast<ADeathTile>(DeathTileArray[i])->DeathTileIndex = i;
		}
	}
}

void ADieselandGameMode::SpawnBoss()
{
    //Find world
	UWorld* const World = GetWorld();
    if(World)
    {
        int32 RandomBossIndex = FMath::RandRange(0, 2);
        UDieselandStaticLibrary::SpawnBlueprint<AActor>(World, HighlanderKing, BossSpawnArray[RandomBossIndex], FRotator(0, 0, 0));
    
    //UGameplayStatics::GetAllActorsOfClass(this, ABossGates::StaticClass(), GateArray);
    if(RandomBossIndex == 0)
    {
        Gate1 = true;
    }
    else if(RandomBossIndex == 1)
    {
        Gate2 = true;
    }
    else if(RandomBossIndex == 2)
    {
        Gate3 = true;
    }
    }
}

bool ADieselandGameMode::StartGame_Validate()
{
	return true;
}

void ADieselandGameMode::RespawnTile(FVector SpawnLocation)
{
	//Find world
	UWorld* const World = GetWorld();

	if (World)
	{
		int32 RandomIndex = FMath::RandRange(0, 15);
		UDieselandStaticLibrary::SpawnBlueprint<AActor>(World, DeathTileClassArray[RandomIndex], SpawnLocation, FRotator(0, 0, 0));
	}
}

AActor* ADieselandGameMode::SpawnLightArray(UClass* ArrayClass, FVector SpawnLocation)
{
	//Find world
	UWorld* const World = GetWorld();

	if (World)
	{
		AActor* tempArray = UDieselandStaticLibrary::SpawnBlueprint<AActor>(World, ArrayClass, SpawnLocation, FRotator(0, 0, 0));
		return tempArray;
	}
	else
	{
		return nullptr;
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
		ResultPawn = GetWorld()->SpawnActor<APawn>(StrykerClass, StartLocation, StartRotation, SpawnInfo);
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
    DOREPLIFETIME(ADieselandGameMode, BossTimer);
	DOREPLIFETIME(ADieselandGameMode, DeathTileClassArray);
	DOREPLIFETIME(ADieselandGameMode, LocationArray);
}
