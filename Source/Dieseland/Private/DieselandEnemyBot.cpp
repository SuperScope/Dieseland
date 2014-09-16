

#include "Dieseland.h"
#include "GameFramework/Actor.h"
#include "DieselandEnemyBot.h"
#include "DieselandCharacter.h"
#include "DieselandPlayerController.h"
#include "DieselandEnemyAI.h"
#include "UnrealNetwork.h"


ADieselandEnemyBot::ADieselandEnemyBot(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	AIControllerClass = ADieselandEnemyBot::StaticClass();

	// Set size for player capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	// Set the starting health value
	Health = 100;

	// Create the text component
	PlayerLabel = PCIP.CreateDefaultSubobject<UTextRenderComponent>(this, TEXT("PlayerLabel"));
	PlayerLabel->AttachTo(RootComponent);
	PlayerLabel->AddRelativeLocation(FVector(-80.0f, 0.0f, 0.0f), false);
	PlayerLabel->AddLocalRotation(FRotator(90.0f, 0.0f, -180.0f));
	PlayerLabel->Text = FString::FromInt(Health);

	// Find the mesh to use for AimMesh component
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticAimMesh(TEXT("StaticMesh'/Game/Shapes/Shape_Cube.Shape_Cube'"));

	// Setup the AimMesh component
	AimMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("AimMesh"));
	AimMesh->AttachParent = (Mesh);
	AimMesh->AttachSocketName = FName(TEXT("AimSocket"));
	//AimMesh->SetStaticMesh(StaticAimMesh.Object);
	AimMesh->SetHiddenInGame(true);

	// Tag this character as a player
	Tags.Add(FName("Enemy"));


	BasicAttackDamage = 10;

	// Set default ranges
	MeleeRange = 144.0f;
	AttackZone = 1200.0f;

	// Cooldown values
	BasicAttackCooldown = 0.2f;


	// Timer values
	BasicAttackTimer = 1.0f;


	//all of the variables needed for creating a collider
	MeleeCollision = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("MeleeCollision"));
	MeleeCollision->AttachParent = (Mesh);
	MeleeCollision->AttachSocketName = FName(TEXT("AimSocket"));
	MeleeCollision->AddLocalOffset(FVector(0.0f, 0.0f, (MeleeRange / 2.0f) + 50.0f));
	MeleeCollision->SetCapsuleHalfHeight(MeleeRange / 2.0f);
	MeleeCollision->SetCapsuleRadius(MeleeRange / 2.0f);
	MeleeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//all of the variables needed for creating a collider for the attack zone
	AttackZoneCollision = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("AttackZone"));
	AttackZoneCollision->AttachParent = (Mesh);
	//AttackZoneCollision->AddLocalOffset(FVector(0.0f, 0.0f, (MeleeRange / 2.0f) + 50.0f));
	AttackZoneCollision->SetCapsuleHalfHeight(AttackZone / 2.0f);
	AttackZoneCollision->SetCapsuleRadius(AttackZone / 2.0f);
	AttackZoneCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Ensure replication
	bReplicates = true;
	AimMesh->SetIsReplicated(true);
	Mesh->SetIsReplicated(true);
	PrimaryActorTick.bCanEverTick = true;

	//here we set the dieseland aggresion to true
	isAggressive = false;
	
}

void ADieselandEnemyBot::Tick(float DeltaSeconds)
{
	// Every frame set the health display
	// TODO: Remove when UI is completed
	PlayerLabel->SetText(FString::FromInt(Health));

	Super::Tick(DeltaSeconds);
}

void ADieselandEnemyBot::EditHealth(int32 Amt, AActor* Target)
{
	if (Target->ActorHasTag(FName(TEXT("Player"))))
	{
		Cast<ADieselandCharacter>(Target)->Health += Amt;
		PlayerLabel->SetText(FString::FromInt(Health));

		//i Don't think we need this function for the AI
		if (Role < ROLE_Authority)
		{
			Cast<ADieselandPlayerController>(Controller)->ServerEditHealth(Amt, Target);
		}
	}
}


//here is the basic melee attack for the AI
void ADieselandEnemyBot::MeleeAttack()
{
	MeleeCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeleeCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	MeleeCollision->GetOverlappingActors(ActorsInMeleeRange);
	MeleeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AActor* CurActor = NULL;
	for (int32 b = 0; b < ActorsInMeleeRange.Num(); b++)
	{
		CurActor = ActorsInMeleeRange[b];
		if (!CurActor && CurActor->ActorHasTag(FName(TEXT("Player")))) continue;
		if (!CurActor->IsValidLowLevel()) continue;

		if (Role == ROLE_Authority){
			EditHealth(-1 * BasicAttackDamage, CurActor);
		}
	}
}


void ADieselandEnemyBot::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ADieselandEnemyBot, Health);
	DOREPLIFETIME(ADieselandEnemyBot, AimMesh);
	DOREPLIFETIME(ADieselandEnemyBot, BasicAttackTimer);
	DOREPLIFETIME(ADieselandEnemyBot, BasicAttackActive);
	DOREPLIFETIME(ADieselandEnemyBot, BasicAttackDamage);
}

void ADieselandEnemyBot::OnZoneEnter()
{
	ADieselandEnemyAI* BotController = Cast<ADieselandEnemyAI>(Controller);

	AttackZoneCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackZoneCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	AttackZoneCollision->GetOverlappingActors(ActorsInZoneRange);
	AttackZoneCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AActor* CurActor = NULL;
	isAggressive = false;
	for (int32 b = 0; b < ActorsInZoneRange.Num(); b++)
	{
		CurActor = ActorsInZoneRange[b];
		if (!CurActor && CurActor->ActorHasTag(FName(TEXT("Player")))) continue;
		if (!CurActor->IsValidLowLevel()) continue;

		if (Role == ROLE_Authority && CurActor->ActorHasTag(FName(TEXT("Player")))){
			this->CharacterMovement->MaxWalkSpeed = 400;
			isAggressive = true;
		}
	}
	if (!isAggressive)
	{
		this->CharacterMovement->MaxWalkSpeed = 0;
	}
}

void ADieselandEnemyBot::OnZoneExit()
{
	

}