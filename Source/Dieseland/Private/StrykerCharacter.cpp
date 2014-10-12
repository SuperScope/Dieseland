

#include "Dieseland.h"
#include "StrykerCharacter.h"
#include "DieselandCharacter.h"
#include "DieselandEnemyBot.h"
#include "DieselandPlayerController.h"
#include "StrykerPoisionProjectile.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"



AStrykerCharacter::AStrykerCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	//here I set his base values
	BaseMoveSpeed = 410;
	BaseHealth = 360;
	BaseDamage = 33;
	BaseCooldownSpeed = 1.3;
	//here I set his base stats
	Strength = 13;
	Intelligence = 13;
	Dexterity = 17;
	Constitution = 9;

	MaxHealth = BaseHealth + (Constitution * 20.0f) + (Strength * 3.0f);
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

	AssassinationColliderRange = 250.0f;

	//base cooldowns
	BaseSkillOneCooldown = 1.0f;
	BaseSkillTwoCooldown = 1.0f;
	BaseSkillThreeCooldown = 1.0f;

	//adjustments for cooldowns
	SkillOneCooldown = BaseSkillOneCooldown / (1 + Intelligence / 100);
	SkillTwoCooldown = BaseSkillTwoCooldown / (1 + Intelligence / 100);
	SkillThreeCooldown = BaseSkillThreeCooldown / (1 + Intelligence / 100);


	//all of the variables needed for creating a collider
	AssassinationCollider = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("AssassinationCollider"));
	AssassinationCollider->AttachParent = (Mesh);
	AssassinationCollider->AttachSocketName = FName(TEXT("AimSocket"));
	AssassinationCollider->SetCapsuleHalfHeight(AssassinationColliderRange / 2.0f);
	AssassinationCollider->SetCapsuleRadius(AssassinationColliderRange / 2.0f);
	AssassinationCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AActor* AssassinationTarget = NULL;


	bReplicates = true;
	bReplicateMovement = true;

	//here I set melee to false so that Stryker only uses ranged attacks
	IsMelee = true;
}

void AStrykerCharacter::Tick(float DeltaSeconds)
{
	UpdateDurationTimers(DeltaSeconds);

	Super::Tick(DeltaSeconds);
}

void AStrykerCharacter::UpdateDurationTimers_Implementation(float DeltaSeconds)
{
	if (IsAttemptingAssassinate)
	{
		AssasinationAttemptDuration += DeltaSeconds;
		SearchForAssassinationTarget();
		//Assasination Failed
		if (AssasinationAttemptDuration > 0.3f && IsAssassinating == false)
		{
			this->StatusEffects.Remove(FString("Stunned"));
			AssasinationAttemptDuration = 0;
			IsAttemptingAssassinate = false;
		}
	}
	if (IsAssassinating && AssasinationHitCounter < 6)
	{
		AssasinationDuration += DeltaSeconds;
		this->StatusEffects.Remove(FString("Stunned"));
		SearchForAssassinationTarget();
		IsAttemptingAssassinate = false;
		
	}
	if (AssasinationHitCounter == 6)
	{
		AssasinationDuration = 0;
		IsAssassinating = false;
		AssasinationHitCounter = 0;
		AActor* AssassinationTarget = NULL;
		this->StatusEffects.Remove(FString("Stunned"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen messag12321321e!"));
	}
}

bool AStrykerCharacter::UpdateDurationTimers_Validate(float DeltaSeconds)
{
	return true;
}

void AStrykerCharacter::SearchForAssassinationTarget()
{
	AssassinationCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AssassinationCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	AssassinationCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	AssassinationCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AssassinationCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	AssassinationCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
	AssassinationCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	AssassinationCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	AssassinationCollider->GetOverlappingActors(ActorsInMeleeRange);
	AssassinationCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AActor* CurActor = NULL;

	for (int32 b = 0; b < ActorsInMeleeRange.Num(); b++)
	{
		CurActor = ActorsInMeleeRange[b];
		if (!CurActor && (CurActor->ActorHasTag(FName(TEXT("Player"))) || CurActor->ActorHasTag(FName(TEXT("Enemy"))))) continue;
		if (!CurActor->IsValidLowLevel()) continue;

		if (Role == ROLE_Authority && CurActor != this){
			if (IsAssassinating == false)
			{
			
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message! %"), CurActor);
				FVector AssassinateLocation;
				this->CharacterMovement->Velocity += FVector(0, 0, 0);
				if ((CurActor->ActorHasTag(FName(TEXT("Player")))))
				{
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
					this->AssassinationCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
					this->AssassinationCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
					this->AssassinationCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
					this->AssassinationCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
					UE_LOG(LogTemp, Log, TEXT("Log text %s"), *CurActor->GetName());
					IsAssassinating = true;
					ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(CurActor);
					AssassinateLocation = DieselandPawn->GetActorLocation();
					AssassinationTarget = CurActor;
				}
				else if ((CurActor->ActorHasTag(FName(TEXT("Enemy")))))
				{
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
					UE_LOG(LogTemp, Log, TEXT("Log text %s"), *CurActor->GetName());
					IsAssassinating = true;
					ADieselandEnemyBot* DieselandPawn = Cast<ADieselandEnemyBot>(CurActor);
					AssassinateLocation = DieselandPawn->GetActorLocation();
					AssassinationTarget = CurActor;
				}
			}
		}
	}
	if (AssassinationTarget != NULL){
		if (AssasinationDuration > 0.5f){
			EditHealth(-1 * (BasicAttackDamage + Dexterity), AssassinationTarget);
			AssasinationDuration = 0;
			AssasinationHitCounter++;
		}
	}
}

// Stryker Assasinate
void AStrykerCharacter::SkillOne()
{
	/*	UWorld* const World = GetWorld();
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
	}*/

	UWorld* const World = GetWorld();
	if (World){
		FRotator CharacterRotation = Mesh->GetSocketRotation(FName(TEXT("AimSocket")));
		CharacterRotation = FRotator(CharacterRotation.Pitch, CharacterRotation.Yaw + 90.0f, CharacterRotation.Roll);
		this->SetActorRotation(CharacterRotation);
		FVector Direction = CharacterRotation.Vector();
		this->StatusEffects.Add(FString("Stunned"));
		this->CharacterMovement->Velocity += FVector(Direction.X * 15000, Direction.Y * 15000,0);
		IsAttemptingAssassinate = true;

	}
	/* some sample movement i'm usng to reference
	DieselandPawn->CharacterMovement->Velocity += FVector(-MoveCharacterX * 600 + (Intelligence * 1.5f), -MoveCharacterY * 600 + (Intelligence * 1.5f), 0); */
}

//Stryker Poison
void AStrykerCharacter::SkillTwo()
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
		AStrykerPoisionProjectile* const Projectile = World->SpawnActor<AStrykerPoisionProjectile>(AStrykerPoisionProjectile::StaticClass(), Mesh->GetSocketLocation(FName(TEXT("AimSocket"))), ProjectileRotation, SpawnParams);
		if (Projectile)
		{
			Projectile->ProjectileDamage = 0;
			Projectile->PoisionSlowAmount = int(75 - Dexterity / 2);
			Projectile->PoisionDamageReductionAmount = int(75 - Dexterity / 2);
			// Start the particle effect
			Projectile->ServerActivateProjectile();

			// Add the character's velocity to the projectile
			Projectile->ProjectileMovement->SetVelocityInLocalSpace((Projectile->ProjectileMovement->InitialSpeed  * ProjectileRotation.Vector()) + (GetVelocity().GetAbs() * Mesh->GetSocketRotation(FName(TEXT("AimSocket"))).GetNormalized().Vector()));
		}
	}
}

//Stryker Blink
void AStrykerCharacter::SkillThree()
{
	UWorld* const World = GetWorld();
	if (World){
		FRotator CharacterRotation = Mesh->GetSocketRotation(FName(TEXT("AimSocket")));
		CharacterRotation = FRotator(CharacterRotation.Pitch, CharacterRotation.Yaw + 90.0f, CharacterRotation.Roll);
		this->SetActorRotation(CharacterRotation);
		FVector Direction = CharacterRotation.Vector();
		//this->StatusEffects.Add(FString("Stunned"));
		this->CharacterMovement->Velocity += FVector(Direction.X * 15000, Direction.Y * 15000, 0);
	}
}

void AStrykerCharacter::RangedAttack()
{
	//Stryker does not use ranged
}

//stryker basic attack
void AStrykerCharacter::MeleeAttack()
{
	MeleeCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeleeCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	MeleeCollision->GetOverlappingActors(ActorsInMeleeRange);
	MeleeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AActor* CurActor = NULL;
	for (int32 b = 0; b < ActorsInMeleeRange.Num(); b++)
	{
		CurActor = ActorsInMeleeRange[b];
		if (!CurActor && (CurActor->ActorHasTag(FName(TEXT("Player"))) || CurActor->ActorHasTag(FName(TEXT("Enemy"))) || CurActor->ActorHasTag(FName(TEXT("ScrapBox"))))) continue;
		if (!CurActor->IsValidLowLevel()) continue;

		if (Role == ROLE_Authority && CurActor != this)
		{
			EditHealth(-1 * BasicAttackDamage, CurActor);
		}
	}
}

void AStrykerCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}


