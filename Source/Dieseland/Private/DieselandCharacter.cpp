// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Dieseland.h"
#include "DieselandStaticLibrary.h"
#include "DieselandCharacter.h"
#include "DieselandPlayerController.h"
#include "DieselandEnemyBot.h"
#include "UnrealNetwork.h"
#include "BaseProjectile.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "ScrapBox.h"
#include "Scrap.h"
#include "DieselandGameState.h"

ADieselandCharacter::ADieselandCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Set size for player capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	CharacterMovement->bOrientRotationToMovement = true; // Rotate character to moving direction
	CharacterMovement->RotationRate = FRotator(0.f, 640.f, 0.f);
	CharacterMovement->bConstrainToPlane = true;
	CharacterMovement->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->RelativeRotation = FRotator(-55.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CharacterLevel = 1;

	// Set the starting health value
	MaxHealth = 700;
	Health = MaxHealth;
	BaseHealth = 100;

	// Scrap value
	Scrap = 0;
	Kills = 0;

	// Base Move Speed
	BaseMoveSpeed = 400;
	this->CharacterMovement->MaxWalkSpeed = BaseMoveSpeed;
	//baseDamage
	BaseDamage = 10;

	//baseCooldownSpeed
	BaseCooldownSpeed = 2.0f;
	// Create the text component
	// TODO: Remove when UI is implemented
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
	Tags.Add(FName("Player"));

	// Does the player use a melee attack
	IsMelee = false;

	// Damage values
	BasicAttackDamage = 10;

	// Ammo values
	BasicAttackMag = 20;
	BasicAttackAmmo = BasicAttackMag;

	// Set default ranges
	MeleeRange = 144.0f;
	RangedRange = 1200.0f;

	// Cooldown values
	BasicAttackCooldown = 1.0f;
	BasicAttackReloadSpeed = 3.0f;
	SkillOneCooldown = 2.0f;
	SkillTwoCooldown = 3.5f;
	SkillThreeCooldown = 4.0f;

	// Timer values
	BasicAttackTimer = 0.0f;
	BasicAttackReloadTimer = 0.0f;
	SkillOneTimer = 0.0f;
	SkillTwoTimer = 0.0f;
	SkillThreeTimer = 0.0f;
	LingerTimer = 0.0f;
	PoisonTimer = 0.0f;

	// Set up collision area for melee attacks
	MeleeCollision = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("MeleeCollision"));
	MeleeCollision->AttachParent = (Mesh);
	MeleeCollision->AttachSocketName = FName(TEXT("AimSocket"));
	MeleeCollision->AddLocalOffset(FVector(0.0f, 0.0f, (MeleeRange / 2.0f) + 50.0f));
	MeleeCollision->SetCapsuleHalfHeight(MeleeRange / 2.0f);
	MeleeCollision->SetCapsuleRadius(MeleeRange / 2.0f);
	MeleeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	AOECollision = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("AOECollision"));
	AOECollision->AttachParent = (Mesh);
	AOECollision->AttachSocketName = FName(TEXT("AimSocket"));
	AOECollision->SetSphereRadius(300.0f);
	AOECollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Retrieve particle assets
	static ConstructorHelpers::FObjectFinder<UParticleSystem> BasicAttackParticleAsset(TEXT("ParticleSystem'/Game/Particles/P_Explosion.P_Explosion'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SkillOneParticleAsset(TEXT("ParticleSystem'/Game/Particles/P_Explosion.P_Explosion'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SkillTwoParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_StrykerBlinkCloak_WIP.Unreal_Particle_StrykerBlinkCloak_WIP'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SkillThreeParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_EngletonPulse2_WIP.Unreal_Particle_EngletonPulse2_WIP'"));
	
	this->BasicAttackParticle = BasicAttackParticleAsset.Object;
	this->SkillOneParticle = SkillOneParticleAsset.Object;
	this->SkillTwoParticle = SkillTwoParticleAsset.Object;
	this->SkillThreeParticle = SkillThreeParticleAsset.Object;

	ParticleSystem = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleSystem"));
	ParticleSystem->Template = SkillThreeParticle;
	ParticleSystem->AttachTo(RootComponent);
	ParticleSystem->bAutoActivate = false;
	ParticleSystem->SetHiddenInGame(false);
	ParticleSystem->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//Find the scrap blueprint's class
	static ConstructorHelpers::FObjectFinder<UClass> ScrapBlueprint(TEXT("Class'/Game/Blueprints/Scrap_BP.Scrap_BP_C'"));
	if (ScrapBlueprint.Object)
	{
		ScrapClass = (UClass*)ScrapBlueprint.Object;
	}

	// Ensure replication
	bReplicates = true;
	bReplicateMovement = true;
	AimMesh->SetIsReplicated(true);
	Mesh->SetIsReplicated(true);
	ParticleSystem->SetIsReplicated(true);
	SetActorTickEnabled(true);
	this->SetOwner(Controller);
}

void ADieselandCharacter::Tick(float DeltaSeconds)
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
			CalculateStats();
		}

	}
}

// CORE ATTRIBUTE CALCULATION FUNCTION
void ADieselandCharacter::CalculateStats_Implementation()
{
	if (this->ActorHasTag(FName(TEXT("Player"))))
	{
		if (Cast<ADieselandPlayerController>(Controller)->StatPlusCount > 3){
			CharacterLevel++;
			Cast<ADieselandPlayerController>(Controller)->StatPlusCount = 0;
		}
		if (IsPoisoned == false){
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Swag"));
			//adjustments for health
			MaxHealth = BaseHealth + (Constitution * 20.0f) + (Strength * 3.0f);
			//adjustments for health regeneration
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

			//adjusments for ability cooldown speed
			SkillOneCooldown = BaseSkillOneCooldown / (1 + Intelligence / 100);
			SkillTwoCooldown = BaseSkillTwoCooldown / (1 + Intelligence / 100);
			SkillThreeCooldown = BaseSkillThreeCooldown / (1 + Intelligence / 100);
		}
	}
}

bool ADieselandCharacter::CalculateStats_Validate()
{
	return true;
}
void ADieselandCharacter::EditHealth(int32 Amt, AActor* Target)
{
	if (Target->ActorHasTag(FName(TEXT("Player"))))
	{
		Cast<ADieselandCharacter>(Target)->Health += Amt;

		PlayerLabel->SetText(FString::FromInt(Health));

		if (Role < ROLE_Authority)
		{
			Cast<ADieselandPlayerController>(Controller)->ServerEditHealth(Amt, Target);
		}

		if (Role == ROLE_Authority && Cast<ADieselandCharacter>(Target)->Health <= 0)
		{
			Kills += 1;
		}
	}
	else if (Target->ActorHasTag(FName(TEXT("Enemy"))))
	{
		ServerDamageEnemy(Amt, Target);
	}
	else if (Target->ActorHasTag(FName(TEXT("ScrapBox"))))
	{
		Cast<AScrapBox>(Target)->DestroyCrate(this);
	}
}

//function for adjusting speed and health, currently using this for strykers posions, I put the function here so it is extendable to other characters in case
//we want to use it again in the future
void ADieselandCharacter::EditSpeedDamage(int32 Speed, int32 Damage, AActor* Target)
{
	if (Target->ActorHasTag(FName(TEXT("Player"))))
	{
		Cast<ADieselandCharacter>(Target)->IsPoisoned = true;
		Cast<ADieselandCharacter>(Target)->CharacterMovement->MaxWalkSpeed = Cast<ADieselandCharacter>(Target)->CharacterMovement->MaxWalkSpeed * (Speed/100.0f);
		Cast<ADieselandCharacter>(Target)->BasicAttackDamage = Cast<ADieselandCharacter>(Target)->BasicAttackDamage * (Damage/100.0f);
		if (Role < ROLE_Authority)
		{
			Cast<ADieselandPlayerController>(Controller)->ServerEditSpeedDamage(Speed, Damage, Target);
		}
	}
	 if (Target->ActorHasTag(FName(TEXT("Enemy"))))
	{
		ServerChangeSpeedDamageEnemy(Speed, Damage, Target);
	}
	else if (Target->ActorHasTag(FName(TEXT("ScrapBox"))))
	{
		Cast<AScrapBox>(Target)->DestroyCrate(this);
	}
}

void ADieselandCharacter::ServerDamageEnemy_Implementation(int32 Amt, AActor* Target)
{
	Cast<ADieselandEnemyBot>(Target)->Health += Amt;
	if (Cast<ADieselandEnemyBot>(Target)->Health <= 0)
	{
		FVector TempEnemyLoc = FVector(Target->GetActorLocation().X, Target->GetActorLocation().Y, Target->GetActorLocation().Z);
		Target->Destroy();

		//Spawn Scrap pieces here
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			for (int32 x = 0; x < 5; x++)
			{
				UDieselandStaticLibrary::SpawnBlueprint<AActor>(World, ScrapClass, FVector(TempEnemyLoc.X, TempEnemyLoc.Y, TempEnemyLoc.Z + (70.0f * x)), FRotator(0.0f, 0.0f, 0.0f));

				//Alternatively used to spawn c++ class
				//AScrap* const Scrap = World->SpawnActor<AScrap>(AScrap::StaticClass(), FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + (70.0f * x)), FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
			}
		}
	}
} 

void ADieselandCharacter::ServerChangeSpeedDamageEnemy_Implementation(int32 Speed, int32 Damage, AActor* Target)
{
	Cast<ADieselandEnemyBot>(Target)->IsPoisoned = true;
	Cast<ADieselandEnemyBot>(Target)->CharacterMovement->MaxWalkSpeed = Cast<ADieselandEnemyBot>(Target)->CharacterMovement->MaxWalkSpeed * (Speed/100.0f);
	Cast<ADieselandEnemyBot>(Target)->BasicAttackDamage = Cast<ADieselandEnemyBot>(Target)->BasicAttackDamage * (Damage/100.0f);
}

bool ADieselandCharacter::ServerChangeSpeedDamageEnemy_Validate(int32 Speed, int32 Damage, AActor* Target)
{
	return true;
}


bool ADieselandCharacter::ServerDamageEnemy_Validate(int32 Amt, AActor* Target)
{
	return true;
}

void ADieselandCharacter::RangedAttack()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Cast<ADieselandPlayerController>(this->Controller);
		SpawnParams.Instigator = Instigator;

		FRotator ProjectileRotation = Mesh->GetSocketRotation(FName(TEXT("AimSocket")));

		ProjectileRotation = FRotator(ProjectileRotation.Pitch, ProjectileRotation.Yaw + 90.0f, ProjectileRotation.Roll);

		// spawn the projectile at the muzzle
		ABaseProjectile* const Projectile = World->SpawnActor<ABaseProjectile>(ABaseProjectile::StaticClass(), Mesh->GetSocketLocation(FName(TEXT("AimSocket"))), ProjectileRotation, SpawnParams);
		if (Projectile)
		{
			Projectile->ProjectileDamage = BasicAttackDamage;
			// Start the particle effect
			Projectile->ServerActivateProjectile();

			// Add the character's velocity to the projectile
			Projectile->ProjectileMovement->SetVelocityInLocalSpace((Projectile->ProjectileMovement->InitialSpeed  * ProjectileRotation.Vector()) + (GetVelocity().GetAbs() * Mesh->GetSocketRotation(FName(TEXT("AimSocket"))).GetNormalized().Vector()));
		}
	}
}

void ADieselandCharacter::MeleeAttack()
{
	MeleeCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeleeCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	MeleeCollision->GetOverlappingActors(ActorsInMeleeRange);
	MeleeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AActor* CurActor = NULL;
	for (int32 b = 0; b < ActorsInMeleeRange.Num(); b++)
	{
		CurActor = ActorsInMeleeRange[b];
		if (!CurActor && (CurActor->ActorHasTag(FName(TEXT("Player"))) || CurActor->ActorHasTag(FName(TEXT("Enemy"))))) continue;
		if (!CurActor->IsValidLowLevel()) continue;
		
		if (Role == ROLE_Authority && CurActor != this)
		{
			EditHealth(-1 * BasicAttackDamage, CurActor);
		}
	}
}

void ADieselandCharacter::SkillOne()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Cast<ADieselandPlayerController>(this->Controller);
		SpawnParams.Instigator = Instigator;

		FRotator ProjectileRotation = Mesh->GetSocketRotation(FName(TEXT("AimSocket")));

		ProjectileRotation = FRotator(ProjectileRotation.Pitch, ProjectileRotation.Yaw + 90.0f, ProjectileRotation.Roll);

		// spawn the projectile at the muzzle
		ABaseProjectile* const Projectile = World->SpawnActor<ABaseProjectile>(ABaseProjectile::StaticClass(), Mesh->GetSocketLocation(FName(TEXT("AimSocket"))), ProjectileRotation, SpawnParams);
		if (Projectile)
		{
			Projectile->ProjectileDamage = BasicAttackDamage;
			// Start the particle effect
			Projectile->ServerActivateProjectile();

			Projectile->SetLifeSpan(1.0f);
			Projectile->Piercing = true;

			// Add the character's velocity to the projectile
			Projectile->ProjectileMovement->SetVelocityInLocalSpace((Projectile->ProjectileMovement->InitialSpeed * ProjectileRotation.Vector()) + (GetVelocity().GetAbs() * Mesh->GetSocketRotation(FName(TEXT("AimSocket"))).GetNormalized().Vector()));
		}
	}
}

void ADieselandCharacter::SkillTwo()
{
	ServerActivateParticle(SkillTwoParticle);

	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	//Re-initialize hit info
	FHitResult RV_Hit(ForceInit);

	//call GetWorld() from within an actor extending class
	GetWorld()->LineTraceSingle(
		RV_Hit,        //result
		Mesh->GetSocketLocation(FName(TEXT("AimSocket"))),    //start
		(Mesh->GetSocketLocation(FName(TEXT("AimSocket"))) + (AimRotation.GetNormalized().Vector() * 500.0f)), //end
		ECC_Pawn, //collision channel
		RV_TraceParams
		);
	FVector TargetLocation;
	if (RV_Hit.IsValidBlockingHit())
	{
		TargetLocation = RV_Hit.Location;
	}
	else
	{
		TargetLocation = Mesh->GetSocketLocation(FName(TEXT("AimSocket"))) + (AimRotation.GetNormalized().Vector() * 500.0f);
	}
	// Make sure the player doesn't fall through the bottom of the map
	TargetLocation.Z = Mesh->GetSocketLocation(FName(TEXT("AimSocket"))).Z;
	SetActorLocation(TargetLocation);
}

void ADieselandCharacter::SkillThree()
{
	ServerActivateParticle(SkillThreeParticle);

	AOECollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AOECollision->SetCollisionProfileName(TEXT("OverlapAll"));
	AOECollision->GetOverlappingActors(ActorsInAOERange);
	AOECollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AActor* CurActor = NULL;
	for (int32 b = 0; b < ActorsInAOERange.Num(); b++)
	{
		CurActor = ActorsInAOERange[b];
		if (!CurActor && (CurActor->ActorHasTag(FName(TEXT("Player"))) || CurActor->ActorHasTag(FName(TEXT("Enemy"))))) continue;
		if (!CurActor->IsValidLowLevel()) continue;

		if (Role == ROLE_Authority && CurActor != this)
		{
			EditHealth(-1 * BasicAttackDamage, CurActor);
		}
	}
}

void ADieselandCharacter::OnRep_AimRotation()
{
	
}


void ADieselandCharacter::ServerActivateParticle_Implementation(UParticleSystem* Particle)
{
	ParticleSystem->SetTemplate(Particle);
	ParticleSystem->ActivateSystem();
}

bool ADieselandCharacter::ServerActivateParticle_Validate(UParticleSystem* Particle)
{
	return true;
}

void ADieselandCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ADieselandCharacter, Health);
	DOREPLIFETIME(ADieselandCharacter, MaxHealth);
	DOREPLIFETIME(ADieselandCharacter, HealthRegeneration);

	DOREPLIFETIME(ADieselandCharacter, Scrap);
	DOREPLIFETIME(ADieselandCharacter, Kills);

	DOREPLIFETIME(ADieselandCharacter, AimMesh);
	DOREPLIFETIME(ADieselandCharacter, AimRotation);

	DOREPLIFETIME(ADieselandCharacter, IsMelee);

	DOREPLIFETIME(ADieselandCharacter, MoveSpeed);

	DOREPLIFETIME(ADieselandCharacter, ParticleSystem);

	DOREPLIFETIME(ADieselandCharacter, LingerTimer);

	DOREPLIFETIME(ADieselandCharacter, BasicAttackTimer);
	DOREPLIFETIME(ADieselandCharacter, BasicAttackReloadTimer);
	DOREPLIFETIME(ADieselandCharacter, SkillOneTimer);
	DOREPLIFETIME(ADieselandCharacter, SkillTwoTimer);
	DOREPLIFETIME(ADieselandCharacter, SkillThreeTimer);

	DOREPLIFETIME(ADieselandCharacter, BasicAttackActive);
	DOREPLIFETIME(ADieselandCharacter, BasicAttackDamage);
	DOREPLIFETIME(ADieselandCharacter, BasicAttackReloadSpeed);
	DOREPLIFETIME(ADieselandCharacter, BasicAttackAmmo);

	DOREPLIFETIME(ADieselandCharacter, BaseDamage);
	DOREPLIFETIME(ADieselandCharacter, BaseCooldownSpeed);
	DOREPLIFETIME(ADieselandCharacter, BaseHealth);

	DOREPLIFETIME(ADieselandCharacter, StatusEffects);
	DOREPLIFETIME(ADieselandCharacter, StunRemaining);

	// Necessary
	DOREPLIFETIME(ADieselandCharacter, Strength);
	DOREPLIFETIME(ADieselandCharacter, Constitution);
	DOREPLIFETIME(ADieselandCharacter, Dexterity);
	DOREPLIFETIME(ADieselandCharacter, Intelligence);
}
