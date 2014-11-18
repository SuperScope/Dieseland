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
#include "DieselandPlayerState.h"

ADieselandGameMode::ADieselandGameMode(const class FPostConstructInitializeProperties& PCIP) : Super(PCIP)
{
	// use our custom PlayerController class
	PlayerControllerClass = ADieselandPlayerController::StaticClass();
	PlayerStateClass = ADieselandPlayerState::StaticClass();

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
	static ConstructorHelpers::FObjectFinder<UClass> FoxBPClass(TEXT("Class'/Game/Blueprints/Players/Fox_BP.Fox_BP_C'"));
    static ConstructorHelpers::FObjectFinder<USoundBase> CountdownSoundCue(TEXT("SoundBase'/Game/AudioDLC/Announcer/Sound_Announcer_Countdown_Cue.Sound_Announcer_Countdown_Cue'"));
    
    CountdownSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("CountdownSound"));
    CountdownSound->bAutoActivate = false;
    CountdownSoundBase =  CountdownSoundCue.Object;
    CountdownSound->SetSound(CountdownSoundBase);
    

	if (MayhemBPClass.Object)
	{
		MayhemClass = MayhemBPClass.Object;
		EngletonClass = EngletonBPClass.Object;
		StrykerClass = StrykerBPClass.Object;
		FoxClass = FoxBPClass.Object;
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

    BossTimer = 300.0f;

    CanSpawn = true;
    StartBossTimer = false;
    //CountdownSound->bAutoActivate = false;

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void ADieselandGameMode::ReceiveBeginPlay()
{
	//StartGame();
	TActorIterator< APlayerStart > ActorItr =
		TActorIterator< APlayerStart >(GetWorld());

	while (ActorItr)
	{
		SpawnArray.Add(*ActorItr);

		++ActorItr;
	}
}

void ADieselandGameMode::Tick(float DeltaSeconds)
{
    //If time to start timer, start timer
    if(StartBossTimer == true)
    {
        BossTimer -= DeltaSeconds;
    }
    
    //If its time to spawn boss and we can spawn a boss
    if (BossTimer <= 180.0f && CanSpawn == true)
    {
        CanSpawn = false;
        //Select a random boss and boss zone
        RandomSpawnIndex = FMath::RandRange(0, 2);
        RandomBossIndex = FMath::RandRange(0,1);
        
        //Spawning is done is BossTiles.cpp
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
    //Start boss timer when game starts
    StartBossTimer = true;
    CountdownSound->Play();
}


bool ADieselandGameMode::StartGame_Validate()
{
	return true;
}

APlayerStart* ADieselandGameMode::PickSpawn()
{
	int32 RandomSpawnNum = FMath::RandRange(0, (SpawnArray.Num() - 1));

	return SpawnArray[RandomSpawnNum];
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
	OnGameEnded();
	GEngine->AddOnScreenDebugMessage(21, 10.0f, FColor::Yellow, FString(TEXT("Winning Team: ")) + FString::FromInt(Cast<ADieselandGameState>(GameState)->WinningTeam));
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

	ResultPawn = GetWorld()->SpawnActor<APawn>(EngletonClass, StartLocation, StartRotation, SpawnInfo);

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
    DOREPLIFETIME(ADieselandGameMode, BossTimer);
    DOREPLIFETIME(ADieselandGameMode, CanSpawn);
    DOREPLIFETIME(ADieselandGameMode, RandomBossIndex);
    DOREPLIFETIME(ADieselandGameMode, RandomSpawnIndex);
	DOREPLIFETIME(ADieselandGameMode, DeathTileClassArray);
	DOREPLIFETIME(ADieselandGameMode, LocationArray);
}
