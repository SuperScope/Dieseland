

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

	//here I initiliaze the box component and light component for the AI
	Box = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Box"));
	Light = PCIP.CreateDefaultSubobject<UPointLightComponent>(this, TEXT("Light"));
	//here I establish the settings for the light and box, such as size
	Box->bGenerateOverlapEvents = true;
	Box->SetRelativeScale3D(FVector(2, 2, 5));
	RootComponent = Box;

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

//for aggressive enemy types, if a player enters the trigger radius of this enemy type they then become aggressive towards the offending player
void ADieselandEnemyAI::OnTriggerEnter(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

//for both aggressive and non aggressive enemy types, if a currently attacking enemy has a player leave
// it's trigger radius it is non longer attacking and begeins healing.
void ADieselandEnemyAI::OnTriggerExit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

//here is a test message used to ensure that the trigger is working properly
void ADieselandEnemyAI :: Debug(FString Msg)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, Msg);
	}

}