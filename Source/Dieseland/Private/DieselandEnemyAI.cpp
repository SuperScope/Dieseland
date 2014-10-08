
#include "Dieseland.h"
#include "Engine.h"
#include "GameFramework/Character.h"
#include "DieselandEnemyAI.h"
#include "DieselandEnemyBot.h"
#include "DieselandCharacter.h"
#include "UnrealNetwork.h"
#include "AI/Navigation/NavigationSystem.h"



ADieselandEnemyAI::ADieselandEnemyAI(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//here I initiliaze the blackboard component and the behavior tree component for the enemy AI
	BlackboardComp = PCIP.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
	BehaviorComp = PCIP.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));


	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	GameTimer = 0;

	Tags.Add(FName("Enemy"));
}

//in this function the AI sets its enemy id and detection id, from there
// it initializes its behavior tree
void ADieselandEnemyAI::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);
	ADieselandEnemyBot* Bot = Cast<ADieselandEnemyBot>(InPawn);
	if (Bot && Bot->BottBehavior)
	{
		BlackboardComp->InitializeBlackboard(Bot->BottBehavior->BlackboardAsset);

		EnemyKeyID = BlackboardComp->GetKeyID("Enemy");
		EnemyKeyLocationID = BlackboardComp->GetKeyID("Destination");
		BehaviorComp->StartTree(Bot->BottBehavior);
		
	}
}
void ADieselandEnemyAI::SearchForSpawnLocation()
{

	if (Cast<ADieselandEnemyBot>(GetPawn()) != nullptr){
		ADieselandEnemyBot* BotPawn = Cast<ADieselandEnemyBot>(GetPawn());
		if (BotPawn->isAggressive == false){
			EnemyKeyLocationID = BlackboardComp->GetKeyID("Destination");
			BlackboardComp->SetValueAsVector(EnemyKeyLocationID, SpawnLocation);
		}
		else{
			SearchForEnemy();
		}
	}
}
//this function runs at the start so that I can get necessary enemy locations
void ADieselandEnemyAI::BeginPlay()
{
	//Super::BeginPlay();


}
//here the AI searches for an enemy player to attack
void ADieselandEnemyAI::SearchForEnemy()
{

	ADieselandEnemyBot* BotPawn = Cast<ADieselandEnemyBot>(GetPawn());
	APawn* MyBot = GetPawn();
	if (MyBot == NULL || BotPawn->isAggressive == false)
	{
		return;
	}

	const FVector MyLoc = MyBot->GetActorLocation();
	float BestDistSq = MAX_FLT;
	//dieselandcharacter is a temp till we redefine what dieseland character is
	ADieselandCharacter* BestPawn = NULL;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		ADieselandCharacter* TestPawn = Cast<ADieselandCharacter>(*It);
		if (TestPawn)
		{
			const float Distsq = FVector::Dist(TestPawn->GetActorLocation(), MyLoc);
			if (Distsq < BestDistSq)
			{
				BestDistSq = Distsq;
				BestPawn = TestPawn;
			}
		}
	}
	if (BestPawn)
	{
		SetEnemy(BestPawn);
	}
}

//here if we find an enemy from the SearchForEnemy() function we can then set an enemy
void ADieselandEnemyAI::SetEnemy(class APawn *InPawn)
{
	BlackboardComp->SetValueAsObject(EnemyKeyID, InPawn);
	BlackboardComp->SetValueAsVector(EnemyKeyLocationID, InPawn->GetActorLocation());
}


bool ADieselandEnemyAI::ServerEditHealth_Validate(int32 Amt, AActor* Target)
{
	return true;
}

void ADieselandEnemyAI::ServerEditHealth_Implementation(int32 Amt, AActor* Target)
{
	// Edit the health of the specific pawn
	if (GetPawn() != nullptr)
	{
		Cast<ADieselandEnemyBot>(GetPawn())->EditHealth(Amt, Target);
	}
}

bool ADieselandEnemyAI::ServerMeleeAttack_Validate()
{
	return true;
}

void ADieselandEnemyAI::ServerMeleeAttack_Implementation()
{
	ADieselandEnemyBot* DieselandPawn = Cast<ADieselandEnemyBot>(GetPawn());
	if (!DieselandPawn->StatusEffects.Contains(FString("Stunned")))
	{
		DieselandPawn->MeleeAttack();
	}
}

bool ADieselandEnemyAI::ServerRangedAttack_Validate()
{
	return true;
}

void ADieselandEnemyAI::ServerRangedAttack_Implementation()
{
	ADieselandEnemyBot* DieselandPawn = Cast<ADieselandEnemyBot>(GetPawn());
	if (!DieselandPawn->StatusEffects.Contains(FString("Stunned")))
	{
		DieselandPawn->RangedAttack();
	}
}


void ADieselandEnemyAI::UpdateCooldownTimers(float DeltaSeconds)
{
	if (Cast<ADieselandEnemyBot>(GetPawn()) != nullptr){
		ADieselandEnemyBot* DieselandPawn = Cast<ADieselandEnemyBot>(GetPawn());
		// Update all of the timers
		if (DieselandPawn->BasicAttackTimer > 0.0f)
		{
			DieselandPawn->BasicAttackTimer -= DeltaSeconds;
			if (DieselandPawn->BasicAttackTimer < 0.0f)
			{
				DieselandPawn->OnZoneEnter();
				DieselandPawn->OnProjectileZoneEnter();
				ServerMeleeAttack();
				SearchForSpawnLocation();
				DieselandPawn->BasicAttackTimer = 1.0f;
			}
		}
		// Basic Attack actions
		if (DieselandPawn->BasicAttackTimer <= 0.0f && DieselandPawn->BasicAttackActive)
		{
			ADieselandEnemyBot* DieselandPawn = Cast<ADieselandEnemyBot>(GetPawn());
				DieselandPawn->BasicAttackTimer = DieselandPawn->BasicAttackCooldown;
		}

		if (DieselandPawn->StunRemaining > 0.0f)
		{
			DieselandPawn->StunRemaining -= DeltaSeconds;
			if (DieselandPawn->StunRemaining <= 0.0f)
			{
				DieselandPawn->StunRemaining = 0.0f;
				DieselandPawn->StatusEffects.Remove(FString("Stunned"));

				//TODO: Replace with calculated speeds based on enemy type/level
				DieselandPawn->CharacterMovement->MaxWalkSpeed = 400.0f;
				DieselandPawn->CharacterMovement->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
			}
		}
	}
	GameTimer += DeltaSeconds;
	if (GameTimer > 1 && GameTimer < 2)
	{
		ADieselandEnemyBot* BotPawn = Cast<ADieselandEnemyBot>(GetPawn());
		SpawnLocationID = BlackboardComp->GetKeyID("SpawnPoint");
		SpawnLocation = BotPawn->GetActorLocation();
		BlackboardComp->SetValueAsVector(SpawnLocationID, BotPawn->GetActorLocation());
	}
}


void ADieselandEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateCooldownTimers(DeltaTime);
	
}

void ADieselandEnemyAI::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ADieselandEnemyAI, GameTimer);
}

