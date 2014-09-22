

#include "Dieseland.h"
#include "ScrapBox.h"


AScrapBox::AScrapBox(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/Shapes/Shape_Sphere.Shape_Sphere'"));

	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->SetStaticMesh(StaticMesh.Object);
	RootComponent = Mesh;
	Mesh->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));
	Mesh->SetCollisionProfileName(FName(TEXT("BlockAll")));

	
}

void AScrapBox::ReceiveActorBeginOverlap(AActor* OtherActor)
{

{
