

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
	//here I set melee to false so that Engleton only uses ranged attacks
	IsMelee = false;

	//here I set the range of Engleton's Bombardment
	BombardmentRange = 600;
	//Set our hitcount for bombardment
	BombardmentHitCounter = 0;
	//here I set the cooldown for player abilities
	SkillOneCooldown = 20.0f;
	SkillTwoCooldown = 12.0f;

	// Set up collision area for Bombardment attacks
	BombardmentCollision = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("BombardmentCollision"));
	BombardmentCollision->AttachParent = (Mesh);
	BombardmentCollision->AttachSocketName = FName(TEXT("AimSocket"));
	BombardmentCollision->SetCapsuleHalfHeight(BombardmentRange / 2.0f);
	BombardmentCollision->SetCapsuleRadius(BombardmentRange / 2.0f);
	BombardmentCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AOECollision->SetSphereRadius(10.0f);


	//here we get and set our particle effects
	static ConstructorHelpers::FObjectFinder<UParticleSystem> BombardmentParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_EngletonBombardment_WIP.Unreal_Particle_EngletonBombardment_WIP'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MachineGunFireParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_EngletonMachineGun_WIP.Unreal_Particle_EngletonMachineGun_WIP'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PulseParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_EngletonPulse_WIP.Unreal_Particle_EngletonPulse_WIP'"));
	this->BombardmentParticle = BombardmentParticleAsset.Object;
	this->MachineGunFireParticle = MachineGunFireParticleAsset.Object;
	this->PulseParticle = PulseParticleAsset.Object;



}

//Engleton Bombardment
void AEngletonCharacter::SkillOne()
{	
	//here I activate Bombardment if it's not already activated
	if (BombardmentTimer == 0)
	{
		BombardmentActivated = true;
	}

	//here we will play the particle effect as soon as the ability begins
	if (BombardmentHitCounter == 0){
		ServerActivateParticle(BombardmentParticle);
	}
	BombardmentCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BombardmentCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	BombardmentCollision->GetOverlappingActors(ActorsInBombardmentRange);
	BombardmentCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	AActor* CurActor = NULL;
	for (int32 b = 0; b < ActorsInBombardmentRange.Num(); b++)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));
		CurActor = ActorsInBombardmentRange[b];
		if (!CurActor && (CurActor->ActorHasTag(FName(TEXT("Player"))) || CurActor->ActorHasTag(FName(TEXT("Enemy"))))) continue;
		if (!CurActor->IsValidLowLevel()) continue;

		if (Role == ROLE_Authority && CurActor != this)
		{	
			//because this damage is applied every half and a second and not every second, the damage is halved. 
			//I apply the damage every half a second so that damage is more realisticly applied from the ability
			EditHealth(-1 * (50 + (Intelligence * 2)), CurActor);
		}
	}
}


void AEngletonCharacter::SkillTwo()
{

}

//here we a activate pulse
void AEngletonCharacter::SkillThree()
{
	PulseActivated = true;
	ServerActivateParticle(PulseParticle);
}

void AEngletonCharacter::RangedAttack()
{

	//particle base is set into play, just need to adjust it's spawn position to the same point as his hands
	ServerActivateParticle(MachineGunFireParticle);

}
void AEngletonCharacter::MeleeAttack()
{
	//Engleton does not use Melee
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

		//here we reaply damage every half a second
		if (BombardmentTimer > 0.5f && BombardmentHitCounter < 6)
		{
			SkillOne();
			BombardmentHitCounter++;
			BombardmentTimer = 0;
		}
	}//end of BombardmentActivated

	//here I check to see if  pulse has been activated, if so I increase Engleton's movement speed for a short period of time.
	if (PulseActivated)
	{
		if (PulseTimer == 0){
			this->CharacterMovement->MaxWalkSpeed = MoveSpeed * (1.3 + (Intelligence / 100));
		}
		PulseTimer += DeltaSeconds;
		if (PulseTimer >= 4){
			PulseActivated = false;
			PulseTimer = 0;
			this->CharacterMovement->MaxWalkSpeed = MoveSpeed;
		}
	}//end of EngletonPulse

}//end of UpdateTimers

void AEngletonCharacter::Tick(float DeltaSeconds)
{
	
	UpdateTimers(DeltaSeconds);
	Super::Tick(DeltaSeconds);
}

