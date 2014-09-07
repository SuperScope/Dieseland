

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
	
}

void ADieselandEnemyController::SetEnemy(class APawn *InPawn)
{

}


