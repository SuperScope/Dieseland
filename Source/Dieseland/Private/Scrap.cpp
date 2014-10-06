

#include "Dieseland.h"
#include "Scrap.h"
#include "DieselandCharacter.h"


AScrap::AScrap(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/Shapes/Shape_Cube.Shape_Cube'"));

	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->SetStaticMesh(StaticMesh.Object);
	RootComponent = Mesh;
	Mesh->SetWorldScale3D(FVector(0.4f, 0.4f, 0.4f));
	Mesh->SetCollisionProfileName(FName(TEXT("BlockAll")));

	Mesh->SetSimulatePhysics(true);

	ScrapCollision = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("ScrapCollision"));
	ScrapCollision->SetCapsuleHalfHeight(150.0f);
	ScrapCollision->SetCapsuleRadius(150.0f);
	ScrapCollision->AttachTo(Mesh);
	ScrapCollision->SetCollisionProfileName(FName(TEXT("OverlapAll")));

	ScrapCollectionArea = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("ScrapCollectionArea"));
	ScrapCollectionArea->SetCapsuleHalfHeight(1000.0f);
	ScrapCollectionArea->SetCapsuleRadius(1000.0f);
	ScrapCollectionArea->AttachTo(Mesh);
	ScrapCollectionArea->SetCollisionProfileName(FName(TEXT("OverlapAll")));

	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AScrap::ReceiveBeginPlay()
{
	float TempZ = FMath::FRandRange(300.0f, 1000.0f);
	float TempY = FMath::FRandRange(-300.0f, 300.0f);
	float TempX = FMath::FRandRange(-300.0f, 300.0f);

	Mesh->AddImpulse(FVector(TempX, TempY, TempZ));
}

void AScrap::Tick(float DeltaSeconds)
{
	ScrapCollectionArea->GetOverlappingActors(ActorsInRange);

	if (TargetedActor == nullptr)
	{
		for (int32 x = 0; x < ActorsInRange.Num(); x++)
		{
			if (ActorsInRange[x]->ActorHasTag(FName(TEXT("Player"))))
			{
				TargetedActor = ActorsInRange[x];
				ScrapCollectionArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				break;
			}

		}
	}
	else if (TargetedActor != nullptr)
	{
		// Tried to make the scrap float to the player here

		/*float DeltaY = TargetedActor->GetActorLocation().Y - this->GetActorLocation().Y;
		float DeltaX = TargetedActor->GetActorLocation().X - this->GetActorLocation().X;
		float DeltaZ = TargetedActor->GetActorLocation().Z - this->GetActorLocation().Z;

		float YawAngleDegrees = FGenericPlatformMath::Atan2(DeltaY, DeltaX) * (180.0f / PI);
		float PitchAngleDegrees = FGenericPlatformMath::Atan2(DeltaY, DeltaX) * (180.0f / PI);
		
		FRotator CurrentRotation = this->GetActorRotation();
		FRotator NewRotation = FRotator(PitchAngleDegrees, YawAngleDegrees, CurrentRotation.Roll);

		this->SetActorRotation(FRotator(CurrentRotation.Pitch, FMath::FInterpTo(CurrentRotation.Yaw, YawAngleDegrees, DeltaSeconds, 0.5f), CurrentRotation.Roll));

		Mesh->AddImpulse(NewRotation.Vector() * -100.0f);
		*/
	}

	Super::Tick(DeltaSeconds);
}

void AScrap::ReceiveHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	if (Other->ActorHasTag(FName(TEXT("Player"))))
	{
		CollectScrap(Other);
	}
}

void AScrap::CollectScrap_Implementation(AActor* OtherActor)
{
	if (Role == ROLE_Authority)
	{
		Cast<ADieselandCharacter>(OtherActor)->Scrap += 1;
	}

	Destroy();
}

bool AScrap::CollectScrap_Validate(AActor* OtherActor)
{
	return true;
}
