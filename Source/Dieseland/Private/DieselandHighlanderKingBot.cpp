


#include "Dieseland.h"
#include "UnrealNetwork.h"
#include "DieselandEnemyAI.h"
#include "DieselandHighlanderKingBot.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"




ADieselandHighlanderKingBot::ADieselandHighlanderKingBot(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	CannonRange = 800;
	CannonAttackDamage = 40;
	BaseAttackDamage = 60;
	HealthRegeneration = 6;
	BasicAttackDamage = BaseAttackDamage;
	MaxHealth = 1500;
	Health = MaxHealth;

	//all of the variables needed for creating a collider
	CannonZoneCollision = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("CannonZone"));
	CannonZoneCollision->AttachParent = (Mesh);
	CannonZoneCollision->AttachSocketName = FName(TEXT("AimSocket"));
	CannonZoneCollision->AddLocalOffset(FVector(0.0f, 0.0f, (CannonRange / 2.0f) + 50.0f));
	//CannonZoneCollision->f
	//CannonZoneCollision->SetCapsuleHalfHeight(CannonRange / 2.0f);
	//CannonZoneCollision->SetCapsuleRadius(CannonRange / 2.0f);
	CannonZoneCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Timer values
	CannonDamageUpdate = 0.2f;

	AttackPatternTimer = 20.0f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> BeamParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_HighlanderKingBeam_WIP.Unreal_Particle_HighlanderKingBeam_WIP'"));


	this->BeamParticle = BeamParticleAsset.Object;


	//here is what we use to setup our particle system
	ParticleSystem = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleSystem"));
	ParticleSystem->Template = BeamParticle;
	ParticleSystem->AttachTo(RootComponent);
	ParticleSystem->bAutoActivate = false;
	ParticleSystem->SetHiddenInGame(false);
	ParticleSystem->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ParticleSystem->SetIsReplicated(true);

}

//the function for it's cannon attack. This happens only once over a period set of time
void ADieselandHighlanderKingBot::CannonAttack()
{
	ADieselandEnemyAI* BotController = Cast<ADieselandEnemyAI>(Controller);

	CannonZoneCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CannonZoneCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	CannonZoneCollision->GetOverlappingActors(ActorsInZoneRange);
	CannonZoneCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AActor* CurActor = NULL;
	for (int32 b = 0; b < ActorsInZoneRange.Num(); b++)
	{
		CurActor = ActorsInZoneRange[b];
		if (!CurActor && CurActor->ActorHasTag(FName(TEXT("Player")))) continue;
		if (!CurActor->IsValidLowLevel()) continue;

		if (Role == ROLE_Authority && CurActor->ActorHasTag(FName(TEXT("Player")))){
			EditHealth(-1 * CannonAttackDamage, CurActor);
			//here I check to see if it's hte first time firing for the cannon, if so I activate the particle effect.
			if (CannonAttackTicks == 0){
				ServerActivateParticle(BeamParticle);
			}
			CannonAttackTicks++;
		}
	}
}

void ADieselandHighlanderKingBot::ServerActivateParticle_Implementation(UParticleSystem* Particle)
{
	ParticleSystem->SetTemplate(Particle);
	ParticleSystem->ActivateSystem();
}

bool ADieselandHighlanderKingBot::ServerActivateParticle_Validate(UParticleSystem* Particle)
{
	return true;
}


void ADieselandHighlanderKingBot::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	// Replicate to everyone
	DOREPLIFETIME(ADieselandHighlanderKingBot, ParticleSystem);
	DOREPLIFETIME(ADieselandHighlanderKingBot, CannonZoneCollision);
	DOREPLIFETIME(ADieselandHighlanderKingBot, CannonRange);
	DOREPLIFETIME(ADieselandHighlanderKingBot, CannonDamageUpdate);
	DOREPLIFETIME(ADieselandHighlanderKingBot, CannonAttackDamage);
	DOREPLIFETIME(ADieselandHighlanderKingBot, AttackPatternTimer);
	DOREPLIFETIME(ADieselandHighlanderKingBot, IsFiringCannons);
	DOREPLIFETIME(ADieselandHighlanderKingBot, CannonAttackTicks);

}