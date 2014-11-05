// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "DieselandWalkerQueen.h"
#include "DieselandEnemyBot.h"
#include "DieselandEnemyAI.h"
#include "Engine.h"
#include "UnrealNetwork.h"
#include "DieselandStaticLibrary.h"



ADieselandWalkerQueen::ADieselandWalkerQueen(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	AttackZone = 1400.0f;
	ProjectileZone = 700.0f;
	BaseAttackDamage = 40;
	HealthRegeneration = 6;
	BasicAttackDamage = BaseAttackDamage;
	MaxHealth = 1500;
	Health = MaxHealth;
	IsQueen = true;

	Tags.Add(FName("Queen"));

	static ConstructorHelpers::FObjectFinder<UClass> WalkerBPClass(TEXT("Class'/Game/Blueprints/Enemies/DieselandWalkerBP.DieselandWalkerBP_C'"));

	if (WalkerBPClass.Object)
	{
		Walker = WalkerBPClass.Object;
	}

}

void ADieselandWalkerQueen::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void ADieselandWalkerQueen::SummonWalkerBodyGuard()
{
	int RandomSpawnPoint = FMath::FRandRange(0, 2);
	float RandomSpawnDistance = 300;
	if (RandomSpawnPoint == 1)
	{
		RandomSpawnDistance = -300;
	}
	else if (RandomSpawnPoint == 2)
	{
		RandomSpawnDistance = 300;
	}
	FVector SpawnLocation = FVector(this->GetActorLocation().X + RandomSpawnDistance, this->GetActorLocation().Y, this->GetActorLocation().Z - 80);
	UWorld* const World = GetWorld();
	if (World)
	{
		//TODO SPAWN A HOMEDOG
		//UDieselandStaticLibrary::SpawnBlueprint<AActor>(World, Walker, SpawnLocation, FRotator(0, 0, 0));
		//ADieselandEnemyBot* tempBot = UDieselandStaticLibrary::SpawnBlueprint<ADieselandEnemyBot>(World, Walker, SpawnLocation, FRotator(0, 0, 0));
		//ADieselandEnemyBot* const TempBot = World->SpawnActor<ADieselandEnemyBot>(ADieselandEnemyBot::StaticClass(), SpawnLocation, FRotator(0, 0, 0));
	//	tempBot->BasicAttackTimer = 0.2f;
	//	tempBot->FirstRun = true;


	}
}
