

#include "Dieseland.h"
#include "DieselandEnemyAI.h"
#include "DieselandEnemyBot.h"
#include "DieselandCharacter.h"



ADieselandEnemyAI::ADieselandEnemyAI(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//here I initiliaze the blackboard component and the behavior tree component for the enemy AI
	BlackboardComp = PCIP.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
	BehaviorComp = PCIP.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));

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
//here the AI searches for an enemy player to attack
void ADieselandEnemyAI::SearchForEnemy()
{
	APawn* MyBot = GetPawn();
	if (MyBot == NULL)
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

