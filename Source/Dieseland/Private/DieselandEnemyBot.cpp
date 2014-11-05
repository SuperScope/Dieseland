

#include "Dieseland.h"
#include "GameFramework/Actor.h"
#include "DieselandEnemyBot.h"
#include "DieselandCharacter.h"
#include "DieselandPlayerController.h"
#include "DieselandEnemyAI.h"
#include "BaseWalkerProjectile.h"
#include "UnrealNetwork.h"
#include "EngletonCrazyLaser.h"


ADieselandEnemyBot::ADieselandEnemyBot(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	AIControllerClass = ADieselandEnemyBot::StaticClass();

	// Set size for player capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	// Set the starting values
	MaxHealth = 175;
	Health = 175;
	BaseAttackDamage = 25;
	BasicAttackDamage = 25;
	HealthRegeneration = 2;

	// Create the text component
	PlayerLabel = PCIP.CreateDefaultSubobject<UTextRenderComponent>(this, TEXT("PlayerLabel"));
	PlayerLabel->AttachTo(RootComponent);
	PlayerLabel->AddRelativeLocation(FVector(-80.0f, 0.0f, 0.0f), false);
	PlayerLabel->AddLocalRotation(FRotator(90.0f, 0.0f, -180.0f));
	PlayerLabel->Text = FString::FromInt(Health);

	// Find the mesh to use for AimMesh component
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> SkeletalMesh(TEXT("StaticMesh'/Game/Shapes/Shape_Cube.Shape_Cube'"));
	SkeletalMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("SkeletalMesh"));
	SkeletalMesh->AttachParent = (Mesh);
//	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AimMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("AimMesh"));
	AimMesh->AttachParent = (SkeletalMesh);
	AimMesh->AttachSocketName = FName(TEXT("AimSocket"));
	//AimMesh->SetStaticMesh(StaticAimMesh.Object);
	AimMesh->SetHiddenInGame(true);

	// Tag this character as a player
	Tags.Add(FName("Enemy"));
	IsQueen = false;


	// Set default ranges
	MeleeRange = 144.0f;
	AttackZone = 1800.0f;
	ProjectileZone = 1200.0f;



	// Cooldown values
	BasicAttackCooldown = 0.2f;
	SpawnTimer = 0.0f;


	// Timer values
	BasicAttackTimer = 1.0f;
	this->CharacterMovement->RotationRate = FRotator(360, 0, 0);

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


	//all of the variables needed for creating a collider for the projectile zone
	ProjectileZoneCollision = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("ProjectileZone"));
	ProjectileZoneCollision->AttachParent = (Mesh);
	//AttackZoneCollision->AddLocalOffset(FVector(0.0f, 0.0f, (MeleeRange / 2.0f) + 50.0f));
	ProjectileZoneCollision->SetCapsuleHalfHeight(ProjectileZone / 2.0f);
	ProjectileZoneCollision->SetCapsuleRadius(ProjectileZone / 2.0f);
	ProjectileZoneCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Ensure replication
	bReplicates = true;
	bReplicateMovement = true;
	SkeletalMesh->SetIsReplicated(true);
	AimMesh->SetIsReplicated(true);
	SetActorTickEnabled(true);
	//Mesh->SetIsReplicated(true);
	PrimaryActorTick.bCanEverTick = true;

	//here we set the dieseland aggresion to false
	isAggressive = false;
	FirstRun = false;
	LingerCount = 0;

	//here is what we use to setup our particle system
	ParticleSystem = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleSystem"));
	ParticleSystem->AttachTo(RootComponent);
	ParticleSystem->bAutoActivate = false;
	ParticleSystem->SetHiddenInGame(false);
	ParticleSystem->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ParticleSystem->SetIsReplicated(true);

	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	this->CharacterMovement->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	this->CharacterMovement->MaxWalkSpeed = 300;

}


void ADieselandEnemyBot::Tick(float DeltaSeconds)
{
	// Every frame set the health display
	// TODO: Remove when UI is completed
	PlayerLabel->SetText(FString::FromInt(Health));

	Super::Tick(DeltaSeconds);

	//for when a character is poisoned
	if (IsPoisoned)
	{
		PoisonTimer += DeltaSeconds;
		if (PoisonTimer > 3)
		{
			IsPoisoned = false;
			PoisonTimer = 0;
			ResetStats();
		}

	}
	//set its move speed to 0

	if (StatusEffects.Contains(FString("Stunned")))
	{
		StunTimer += DeltaSeconds;
		StunLength += DeltaSeconds;
		if (StunTimer > 0.05f)
		{
			if (!StatusEffects.Contains(FString("Charmed"))){
				this->CharacterMovement->MaxWalkSpeed = 0;
			
			}
			else if (StatusEffects.Contains(FString("Charmed"))){
				this->CharacterMovement->MaxWalkSpeed = 100.0f;
				IsMelee = true;
			}
			StunTimer = 0;
			this->CharacterMovement->RotationRate = FRotator(0.0f, 1.0f, 0.0f);
		}
		if (StunLength >= 2.5f)
		{
			this->StatusEffects.Remove(FString("Stunned"));
			this->StatusEffects.Remove(FString("Charmed"));
			{
				this->CharacterMovement->MaxWalkSpeed = 300;
				this->CharacterMovement->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
				StunLength = 0.0f;
				if (IsWalker)
				{
					IsMelee = false;
				}
			}
		}
		if (SlowRemaining > 0.0f)
		{
			SlowRemaining -= DeltaSeconds;
			if (SlowRemaining <= 0.0f)
			{
				SlowRemaining = 0.0f;
				StatusEffects.Remove(FString("Slowed"));
				CharacterMovement->MaxWalkSpeed = 300.0f;
			}
		}
	}



	//here we setup health regeneration for bots
	if (Health < MaxHealth){
		HealthRegenTimer += DeltaSeconds;
		if (isAggressive){
			if (HealthRegenTimer > 1)
			{
				Health += HealthRegeneration;
				HealthRegenTimer = FMath::FRandRange(-0.2f, 0.2f);
			}
		}
		else if (isAggressive == false)
		{
			if (HealthRegenTimer > 0.1f)
			{
				Health += HealthRegeneration;
				HealthRegenTimer = FMath::FRandRange(-0.05f, 0.05f);
			}
		}
	}
	if (LingerTimer > 0.0f)
	{
		LingerTimer -= DeltaSeconds;
		if (LingerTimer < 0.0f)
		{
			LingerTimer = 0;
			LingerCount = 0;
		}
		if ((((LingerTimer < 5.f) && (LingerTimer > 4.f)) || ((LingerTimer < 3.f) && (LingerTimer > 2.f)) || ((LingerTimer < 1.f) && (LingerTimer > 0.f))) && (LingerCount == 0))
		{
			Health = Health - LingerDamage;
			LingerCount = 1;
		}
		else if ((((LingerTimer < 4.f) && (LingerTimer > 3.f)) || ((LingerTimer < 2.f) && (LingerTimer > 1.f))) && (LingerCount == 1))
		{
			Health = Health - LingerDamage;
			LingerCount = 0;
		}
	}
	if (FirstRun)
	{
		SpawnTimer += DeltaSeconds;
		if (SpawnTimer > 0.5f){
			ADieselandEnemyAI* BotController = Cast<ADieselandEnemyAI>(Controller);
		//	BotController->SearchForEnemy();
			FirstRun = false;
			SpawnTimer = 0.0f;
		}
	}
}

void ADieselandEnemyBot::ResetStats()
{
	BasicAttackDamage = BaseAttackDamage;
	HealthRegeneration = 2;
	this->CharacterMovement->MaxWalkSpeed = 300;
}

void ADieselandEnemyBot::EditHealth(int32 Amt, AActor* Target)
{
	if (this != nullptr){
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
}


//here is the basic melee attack for the AI
void ADieselandEnemyBot::MeleeAttack()
{
	//here I do an if check to test and see if the Bot is of melee type, if so then I proceed with the melee attack.
	if (IsMelee && (StatusEffects.Contains(FString("Stunned")) == false)){
		MeleeCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		MeleeCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
		MeleeCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
		MeleeCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		MeleeCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
		MeleeCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
		MeleeCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
		MeleeCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
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
}

//here is the basic ranged attack for the AI
void ADieselandEnemyBot::RangedAttack_Implementation()
{
	//here I do an if check to test and see if the Bot is not of melee type, if so then I proceed with the ranged attack.
	if (!IsMelee && (StatusEffects.Contains(FString("Stunned")) == false)){
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Cast<ADieselandEnemyAI>(this->Controller);
			SpawnParams.Instigator = Instigator;

			FRotator ProjectileRotation = this->GetActorRotation();

			ProjectileRotation = FRotator(ProjectileRotation.Pitch, ProjectileRotation.Yaw, ProjectileRotation.Roll);

			// spawn the projectile at the muzzle
			ABaseWalkerProjectile* const Projectile = World->SpawnActor<ABaseWalkerProjectile>(ABaseWalkerProjectile::StaticClass(), SkeletalMesh->GetSocketLocation(FName(TEXT("AimSocket"))), ProjectileRotation, SpawnParams);
			if (Projectile)
			{
				if (IsQueen)
				{
					FRotator ProjectileRotation2 = FRotator(ProjectileRotation.Pitch, ProjectileRotation.Yaw + 15, ProjectileRotation.Roll);
					FRotator ProjectileRotation3 = FRotator(ProjectileRotation.Pitch, ProjectileRotation.Yaw - 15, ProjectileRotation.Roll);
					ABaseWalkerProjectile* const Projectile2 = World->SpawnActor<ABaseWalkerProjectile>(ABaseWalkerProjectile::StaticClass(), SkeletalMesh->GetSocketLocation(FName(TEXT("AimSocket"))), ProjectileRotation2, SpawnParams);
					ABaseWalkerProjectile* const Projectile3 = World->SpawnActor<ABaseWalkerProjectile>(ABaseWalkerProjectile::StaticClass(), SkeletalMesh->GetSocketLocation(FName(TEXT("AimSocket"))), ProjectileRotation3, SpawnParams);
					Projectile->Mesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
					Projectile->ProjectileMovement->InitialSpeed = 2500;
					Projectile->Piercing = false;

					Projectile2->Mesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
					Projectile2->ProjectileMovement->InitialSpeed = 2500;
					Projectile2->Piercing = false;
					Projectile2->ProjectileDamage = BasicAttackDamage;
					Projectile2->ServerActivateProjectile();
					Projectile2->ProjectileMovement->SetVelocityInLocalSpace((Projectile->ProjectileMovement->InitialSpeed * ProjectileRotation2.Vector()) + (GetVelocity().GetAbs() * Mesh->GetSocketRotation(FName(TEXT("AimSocket"))).GetNormalized().Vector()));

					Projectile3->Mesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
					Projectile3->ProjectileMovement->InitialSpeed = 2500;
					Projectile3->Piercing = false;
					Projectile3->ProjectileDamage = BasicAttackDamage;
					Projectile3->ServerActivateProjectile();
					Projectile3->ProjectileMovement->SetVelocityInLocalSpace((Projectile->ProjectileMovement->InitialSpeed * ProjectileRotation3.Vector()) + (GetVelocity().GetAbs() * Mesh->GetSocketRotation(FName(TEXT("AimSocket"))).GetNormalized().Vector()));
				}
				else if (IsQueen == false)
				{
					Projectile->Mesh->SetWorldScale3D(FVector(0.3f,0.3f, 0.3f));
					Projectile->ProjectileMovement->InitialSpeed = 1600;
					Projectile->Piercing = false;
				}
				Projectile->ProjectileDamage = BasicAttackDamage;
				Projectile->ServerActivateProjectile();

				// Add the character's velocity to the projectile
				Projectile->ProjectileMovement->SetVelocityInLocalSpace((Projectile->ProjectileMovement->InitialSpeed * ProjectileRotation.Vector()) + (GetVelocity().GetAbs() * Mesh->GetSocketRotation(FName(TEXT("AimSocket"))).GetNormalized().Vector()));
			}
		}
	}
}
bool ADieselandEnemyBot::RangedAttack_Validate()
{
	return true;
}

void ADieselandEnemyBot::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ADieselandEnemyBot, Health);
	DOREPLIFETIME(ADieselandEnemyBot, AimMesh);
	if (!IsMelee){
		DOREPLIFETIME(ADieselandEnemyBot, SkeletalMesh);
	}
	//DOREPLIFETIME(ADieselandEnemyBot, Mesh);
	DOREPLIFETIME(ADieselandEnemyBot, BasicAttackTimer);
	DOREPLIFETIME(ADieselandEnemyBot, BasicAttackActive);
	DOREPLIFETIME(ADieselandEnemyBot, BasicAttackDamage);
	DOREPLIFETIME(ADieselandEnemyBot, ParticleSystem);
	DOREPLIFETIME(ADieselandEnemyBot, StatusEffects);
	DOREPLIFETIME(ADieselandEnemyBot, StunRemaining);
}

void ADieselandEnemyBot::OnZoneEnter()
{
	ADieselandEnemyAI* BotController = Cast<ADieselandEnemyAI>(Controller);

	AttackZoneCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackZoneCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	AttackZoneCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	AttackZoneCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AttackZoneCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	AttackZoneCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
	AttackZoneCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	AttackZoneCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
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
			isAggressive = true;
			if (IsMelee == false){
				this->CharacterMovement->MaxWalkSpeed = 300;
			}
		}
	}
}

void ADieselandEnemyBot::OnProjectileZoneEnter()
{
	ADieselandEnemyAI* BotController = Cast<ADieselandEnemyAI>(Controller);

	ProjectileZoneCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileZoneCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	ProjectileZoneCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	ProjectileZoneCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ProjectileZoneCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	ProjectileZoneCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
	ProjectileZoneCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	ProjectileZoneCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	ProjectileZoneCollision->GetOverlappingActors(ActorsInZoneRange);
	ProjectileZoneCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AActor* CurActor = NULL;
	for (int32 b = 0; b < ActorsInZoneRange.Num(); b++)
	{
		CurActor = ActorsInZoneRange[b];
		if (!CurActor && CurActor->ActorHasTag(FName(TEXT("Player")))) continue;
		if (!CurActor->IsValidLowLevel()) continue;

		if (Role == ROLE_Authority && CurActor->ActorHasTag(FName(TEXT("Player")))){
			if (!IsMelee)
			{
				this->CharacterMovement->MaxWalkSpeed = 0;
				if (!StatusEffects.Contains(FString("Stunned")))
				{
					RangedAttack();
				}
			}
		}
	}

}

void ADieselandEnemyBot::ServerActivateParticle_Implementation(UParticleSystem* Particle)
{
	ParticleSystem->SetTemplate(Particle);
	ParticleSystem->ActivateSystem();
}

bool ADieselandEnemyBot::ServerActivateParticle_Validate(UParticleSystem* Particle)
{
	return true;
}


