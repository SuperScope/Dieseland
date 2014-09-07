

#include "Dieseland.h"
#include "DieselandEnemyController.h"
#include "DieselandEnemy.h"




ADieselandEnemyController::ADieselandEnemyController(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	BlackboardComp = PCIP.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackBoardComp"));

	BehaviorComp = PCIP.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));

}

void ADieselandEnemyController::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);
	ADieselandEnemy* Enemy = Cast<ADieselandEnemy>(InPawn);

	if (Enemy && Enemy->EnemyBehavior)
	{
		BlackboardComp->InitializeBlackboard(Enemy->EnemyBehavior->BlackboardAsset);

		EnemyKeyID = BlackboardComp->GetKeyID("Enemy");
		EnemyLocationID = BlackboardComp->GetKeyID("Destination");

		BehaviorComp->StartTree(Enemy->EnemyBehavior);
	}
}

void ADieselandEnemyController::SearchForEnemy()
{
	APawn* MyEnemy = GetPawn();
	//here we do a quick check to ensure that MyEnemy is != null and if so end the function
	if (MyEnemy == NULL)
	{
		return;
	}
	const FVector MyLoc = MyEnemy->GetActorLocation();
	float BestDistsq = MAX_FLT;
	APawn* BestPawn = NULL;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{

		APawn* TestPawn = Cast<APawn>(*It);
		if (TestPawn)
		{
			const float DistSq = FVector::Dist(TestPawn->GetActorLocation, MyLoc);
			if (DistSq < BestDistsq)
			{
				BestDistsq = DistSq;
				BestPawn = TestPawn;
			}
		}
	}
	if (BestPawn)
	{
		SetEnemy(BestPawn);
	}
}

void ADieselandEnemyController::SetEnemy(class APawn *InPawn)
{
	BlackboardComp->SetValueAsObject(EnemyKeyID, InPawn);
	BlackboardComp->SetValueAsVector(EnemyLocationID, InPawn->GetActorLocation());

}


