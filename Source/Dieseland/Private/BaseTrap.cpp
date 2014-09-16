

#include "Dieseland.h"
#include "BaseTrap.h"


ABaseTrap::ABaseTrap(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	DamageBase = 50.f;

	BaseCollisionComponent = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BaseBoxComponent"));

	RootComponent = BaseCollisionComponent;

	TrapMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("TrapMesh"));

	TrapMesh->AttachTo(RootComponent);

	//TrapMesh->OnComponentBeginOverlap.__Internal_AddDynamic(this, &ABaseTrap::OnActivateTrap);
}

void ABaseTrap::OnActivateTrap(AActor* OtherActor, int32 Damage, int32 Speed)
{
	
}

