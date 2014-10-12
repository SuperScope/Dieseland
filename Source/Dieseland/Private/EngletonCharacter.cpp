

#include "Dieseland.h"
#include "EngletonCharacter.h"
#include "DieselandEnemyBot.h"
#include "DieselandPlayerController.h"
#include "EngletonCrazyLaser.h"
#include "EngletonMachineGun.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"


AEngletonCharacter::AEngletonCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//here I set his base values
	BaseMoveSpeed = 375;
	BaseHealth = 350;
	BaseDamage = 15;
	BaseCooldownSpeed = 1.2;
	//here I set his base stats
	Strength = 10;
	Intelligence = 18;
	Dexterity = 13;
	Constitution = 11;



	//here I adjust those base values based on his stats
	//adjustments for health
	MaxHealth= BaseHealth + (Constitution * 20.0f) + (Strength * 3.0f);
	Health = MaxHealth;
	HealthRegeneration = 1.0f + (Constitution / 10.0f) + (Strength / 20.0f);
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
	//here I set pulse range
	PulseRange = 450;
	//here I set the range of Engleton's Bombardment
	BombardmentRange = 600;
	//Set our hitcount for bombardment
	BombardmentHitCounter = 0;
	//here I set the cooldown for player abilities
	BaseSkillOneCooldown = 25.0f;
	BaseSkillTwoCooldown = 12.0f;
	BaseSkillThreeCooldown = 12.0f;

	SkillOneCooldown = BaseSkillOneCooldown / (1 + Intelligence / 100);
	SkillTwoCooldown = BaseSkillTwoCooldown / (1 + Intelligence / 100);
	SkillThreeCooldown = BaseSkillThreeCooldown / (1 + Intelligence / 100);


	// Set up collision area for Bombardment attacks
	BombardmentCollision = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("BombardmentCollision"));
	BombardmentCollision->AttachParent = (Mesh);
	BombardmentCollision->AttachSocketName = FName(TEXT("AimSocket"));
	BombardmentCollision->SetCapsuleHalfHeight(BombardmentRange / 2.0f);
	BombardmentCollision->SetCapsuleRadius(BombardmentRange / 2.0f);
	BombardmentCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Set up collision area for the Pulse
	PulseCollision = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("PulseCollision"));
	PulseCollision->AttachParent = (Mesh);
	PulseCollision->AttachSocketName = FName(TEXT("AimSocket"));
	PulseCollision->SetCapsuleHalfHeight(BombardmentRange / 2.0f);
	PulseCollision->SetCapsuleRadius(BombardmentRange / 2.0f);
	PulseCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AOECollision->SetSphereRadius(10.0f);
	
	

	//here we get and set our particle effects
	static ConstructorHelpers::FObjectFinder<UParticleSystem> BombardmentParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_EngletonBombardment_WIP.Unreal_Particle_EngletonBombardment_WIP'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MachineGunFireParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_EngletonMachineGun_WIP.Unreal_Particle_EngletonMachineGun_WIP'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PulseParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_EngletonPulse_WIP.Unreal_Particle_EngletonPulse_WIP'"));
	this->BombardmentParticle = BombardmentParticleAsset.Object;
	this->MachineGunFireParticle = MachineGunFireParticleAsset.Object;
	this->PulseParticle = PulseParticleAsset.Object;
	
	AimMesh2 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("AimMesh2"));
	AimMesh2->AttachParent = (Mesh);
	AimMesh2->AttachSocketName = FName(TEXT("AimSocket2"));
	//AimMesh->SetStaticMesh(StaticAimMesh.Object);
	AimMesh2->SetHiddenInGame(true);
	
	//for sounds
	IdleSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Idle Sound"));
	IdleSound->AttachParent = RootComponent;
	IdleSound->bAutoActivate = false;

	PulseSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Pulse Sound"));
	PulseSound->AttachParent = RootComponent;
	PulseSound->bAutoActivate = false;

	UltimateSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Ultimate Sound"));
	UltimateSound->AttachParent = RootComponent;
	UltimateSound->bAutoActivate = false;

	MachineGunSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Machine Gun Sound"));
	MachineGunSound->AttachParent = RootComponent;
	MachineGunSound->bAutoActivate = false;

	CrazyLaserSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Crazy Laser Sound"));
	CrazyLaserSound->AttachParent = RootComponent;
	CrazyLaserSound->bAutoActivate = false;

	BombardmentSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Bombardment Sound"));
	BombardmentSound->AttachParent = RootComponent;
	BombardmentSound->bAutoActivate = false;

	MovementSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Movement Sound"));
	MovementSound->AttachParent = RootComponent;
	MovementSound->bAutoActivate = true;

}

//Engleton Bombardment
void AEngletonCharacter::SkillOne()
{	
	//here I activate Bombardment if it's not already activated
	if (BombardmentTimer == 0)
	{
		UltimateSound->Play();
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
			EditHealth(-1 * (30 + (Intelligence * 1.5f)), CurActor);
		}
	}
}

//crazy laser
void AEngletonCharacter::SkillTwo()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		CrazyLaserSound->Play();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Cast<ADieselandPlayerController>(this->Controller);
		SpawnParams.Instigator = Instigator;

		FRotator ProjectileRotation = Mesh->GetSocketRotation(FName(TEXT("AimSocket")));

		ProjectileRotation = FRotator(ProjectileRotation.Pitch, ProjectileRotation.Yaw, ProjectileRotation.Roll);

		// spawn the projectile at the muzzle
		AEngletonCrazyLaser* const Projectile = World->SpawnActor<AEngletonCrazyLaser>(AEngletonCrazyLaser::StaticClass(), Mesh->GetSocketLocation(FName(TEXT("AimSocket"))), ProjectileRotation, SpawnParams);
		if (Projectile)
		{
			Projectile->ProjectileDamage = 100 + (Intelligence * 3);
			// Start the particle effect
			Projectile->ServerActivateProjectile();

			// Add the character's velocity to the projectile
			Projectile->ProjectileMovement->SetVelocityInLocalSpace((Projectile->ProjectileMovement->InitialSpeed  * ProjectileRotation.Vector()) + (GetVelocity().GetAbs() * Mesh->GetSocketRotation(FName(TEXT("AimSocket"))).GetNormalized().Vector()));
		}
	}
}

//here we a activate pulse
void AEngletonCharacter::SkillThree()
{
	PulseCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PulseCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	PulseCollision->GetOverlappingActors(ActorsInPulseRange);
	PulseCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PulseSound->Play();
	PulseActivated = true;
	ServerActivateParticle(PulseParticle);

	AActor* CurActor = NULL;
	for (int32 b = 0; b < ActorsInPulseRange.Num(); b++)
	{
		
		CurActor = ActorsInPulseRange[b];
		if (!CurActor && (CurActor->ActorHasTag(FName(TEXT("Player"))) || CurActor->ActorHasTag(FName(TEXT("Enemy"))))) continue;
		if (!CurActor->IsValidLowLevel()) continue;
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ATTACK"));

		if (Role == ROLE_Authority && CurActor != this && (CurActor->ActorHasTag(FName(TEXT("Player")))))
		{	
			ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(CurActor);
			//because this damage is applied every half and a second and not every second, the damage is halved. 
			//I apply the damage every half a second so that damage is more realisticly applied from the ability
		//	EditHealth(-1 * (50 + (Intelligence * 2)), CurActor);
			FVector VectorPlayer = this->GetActorLocation();
			FVector VectorTarget = DieselandPawn->GetActorLocation();
			float MoveCharacterX = VectorPlayer.X - VectorTarget.X;
			float MoveCharacterY = VectorPlayer.Y - VectorTarget.Y;
			if (MoveCharacterX > 0){
				MoveCharacterX = 1;
			}
			if (MoveCharacterX <= 0){
				MoveCharacterX = -1;
			}
			if (MoveCharacterY > 0){
				MoveCharacterY = 1;
			}
			if (MoveCharacterY <= 0){
				MoveCharacterY = -1;
			}
			DieselandPawn->CharacterMovement->Velocity += FVector(-MoveCharacterX * 600, -MoveCharacterY * 600, 0);
			DieselandPawn->CharacterMovement->JumpZVelocity = 400 + (Intelligence * 3);
			DieselandPawn->CharacterMovement->DoJump(false);
		}
		//here we do it for enemies
		if (Role == ROLE_Authority && CurActor != this && (CurActor->ActorHasTag(FName(TEXT("Enemy")))))
		{
			ADieselandEnemyBot* DieselandPawn = Cast<ADieselandEnemyBot>(CurActor);
			//because this damage is applied every half and a second and not every second, the damage is halved. 
			//I apply the damage every half a second so that damage is more realisticly applied from the ability
			//	EditHealth(-1 * (50 + (Intelligence * 2)), CurActor);
			FVector VectorPlayer = this->GetActorLocation();
			FVector VectorTarget = DieselandPawn->GetActorLocation();
			float MoveCharacterX = VectorPlayer.X - VectorTarget.X;
			float MoveCharacterY = VectorPlayer.Y - VectorTarget.Y;
			if (MoveCharacterX > 0){
				MoveCharacterX = 1;
			}
			if (MoveCharacterX <= 0){
				MoveCharacterX = -1;
			}
			if (MoveCharacterY > 0){
				MoveCharacterY = 1;
			}
			if (MoveCharacterY <= 0){
				MoveCharacterY = -1;
			}
			DieselandPawn->CharacterMovement->Velocity += FVector(-MoveCharacterX * 500, -MoveCharacterY * 500, 0);
			DieselandPawn->CharacterMovement->JumpZVelocity = 350 + (Intelligence * 3);
			DieselandPawn->CharacterMovement->DoJump(false);
		}
		
	}

}

void AEngletonCharacter::RangedAttack()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Cast<ADieselandPlayerController>(this->Controller);
		SpawnParams.Instigator = Instigator;

		FRotator ProjectileRotation = Mesh->GetSocketRotation(FName(TEXT("AimSocket")));

		ProjectileRotation = FRotator(ProjectileRotation.Pitch, ProjectileRotation.Yaw, ProjectileRotation.Roll);

		// spawn the projectile at the muzzle
		AEngletonMachineGun* const Projectile = World->SpawnActor<AEngletonMachineGun>(AEngletonMachineGun::StaticClass(), Mesh->GetSocketLocation(FName(TEXT("AimSocket"))), ProjectileRotation, SpawnParams);
		AEngletonMachineGun* const Projectile2 = World->SpawnActor<AEngletonMachineGun>(AEngletonMachineGun::StaticClass(), Mesh->GetSocketLocation(FName(TEXT("AimSocket2"))), ProjectileRotation, SpawnParams);
		if (Projectile && Projectile2)
		{
			//particle base is set into play, just need to adjust it's spawn position to the same point as his hands
			ServerActivateParticle(MachineGunFireParticle);

			Projectile->ProjectileDamage = BasicAttackDamage/2;
			Projectile2->ProjectileDamage = BasicAttackDamage/2;
			// Start the particle effect
			Projectile->ServerActivateProjectile();
			Projectile2->ServerActivateProjectile();

			// Add the character's velocity to the projectile
			Projectile->ProjectileMovement->SetVelocityInLocalSpace((Projectile->ProjectileMovement->InitialSpeed * ProjectileRotation.Vector()) + (GetVelocity().GetAbs() * Mesh->GetSocketRotation(FName(TEXT("AimSocket"))).GetNormalized().Vector()));
			Projectile2->ProjectileMovement->SetVelocityInLocalSpace((Projectile->ProjectileMovement->InitialSpeed * ProjectileRotation.Vector()) + (GetVelocity().GetAbs() * Mesh->GetSocketRotation(FName(TEXT("AimSocket"))).GetNormalized().Vector()));
		}
	}
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
	//here I check to see if health is > maxhealth if so I set to health

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
