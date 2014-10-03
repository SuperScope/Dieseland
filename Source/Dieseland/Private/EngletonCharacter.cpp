

#include "Dieseland.h"
#include "EngletonCharacter.h"
#include "DieselandPlayerController.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"


AEngletonCharacter::AEngletonCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//here I set his base values
	BaseMoveSpeed = 380;
	BaseHealth = 350;
	BaseDamage = 25;
	BaseCooldownSpeed = 1.5;
	//here I set his base stats
	Strength = 9;
	Intelligence = 17;
	Dexterity = 12;
	Constitution = 9;



	//here I adjust those base values based on his stats
	//adjustments for health
	Health = BaseHealth + (Constitution * 20.0f) + (Strength * 3.0f);
	//show those adjustments
	PlayerLabel->SetText(FString::FromInt(Health));
	//adjustments for damage
	BasicAttackDamage = BaseDamage + (Strength * 1.5f) + (Dexterity * .5f) + (Intelligence * .5f);
	//adjusments for attackspeed
	BasicAttackCooldown = BaseCooldownSpeed / (1 + (Dexterity / 50));
	//adjustments for movement Speed
	MoveSpeed = BaseMoveSpeed + (Dexterity * 3);
	this->CharacterMovement->MaxWalkSpeed = MoveSpeed;


	//here I set the range of Engleton's Bombardment
	BombardmentRange = 600;
	//Set our hitcount for bombardment
	BombardmentHitCounter = 0;

	//Setup collision area for Engleton's Bombardment
	// Set up collision area for melee attacks
	BombardmentCollision = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("BombardmentCollision"));
	BombardmentCollision->AttachParent = (Mesh);
	BombardmentCollision->AttachSocketName = FName(TEXT("AimSocket"));
	BombardmentCollision->AddLocalOffset(FVector(0.0f, 0.0f, (BombardmentRange / 2.0f) + 50.0f));
	BombardmentCollision->SetCapsuleHalfHeight(BombardmentRange / 2.0f);
	BombardmentCollision->SetCapsuleRadius(BombardmentRange / 2.0f);
	BombardmentCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEngletonCharacter::SkillOne()
{	

}

void AEngletonCharacter::SkillTwo()
{

}

//Engleton Bombardment
void AEngletonCharacter::SkillThree()
{
	//here I activate Bombardment if it's not already activated
	if (BombardmentTimer == 0)
	{
		BombardmentActivated = true;
	}
	//here is the particle effect for Bombardment
	ServerActivateParticle(SkillThreeParticle);

	BombardmentCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BombardmentCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	BombardmentCollision->GetOverlappingActors(ActorsInMeleeRange);
	BombardmentCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AActor* CurActor = NULL;
	for (int32 b = 0; b < ActorsInAOERange.Num(); b++)
	{
		CurActor = ActorsInAOERange[b];
		if (!CurActor && (CurActor->ActorHasTag(FName(TEXT("Player"))) || CurActor->ActorHasTag(FName(TEXT("Enemy"))))) continue;
		if (!CurActor->IsValidLowLevel()) continue;

		if (Role == ROLE_Authority && CurActor != this)
		{

			EditHealth(-1 * (75 + (Intelligence *3)), CurActor);
		}
	}
}

void AEngletonCharacter::RangedAttack()
{

}
void AEngletonCharacter::MeleeAttack()
{

}

//replicate values to the server
void AEngletonCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AEngletonCharacter, BombardmentRange);
}
void AEngletonCharacter::UpdateTimers(float DeltaSeconds)
{
	//here I check to see if Bombardment has been activated, if it has I peridocally deal damage over a set amount of time
	if (BombardmentActivated){
		BombardmentTimer += DeltaSeconds;

		//I use hit counter to determine when I need to stop dealing damage.
		if (BombardmentHitCounter == 6){
			BombardmentTimer = 0;
			BombardmentHitCounter = 0;
			BombardmentActivated = false;
		}

		if (BombardmentTimer > 0.5f && BombardmentHitCounter < 6)
		{
			SkillThree();
			BombardmentHitCounter++;
			BombardmentTimer = 0;
		}
	}
}

void AEngletonCharacter::Tick(float DeltaSeconds)
{
	
	UpdateTimers(DeltaSeconds);
	Super::Tick(DeltaSeconds);
}

