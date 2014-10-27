

#include "Dieseland.h"
#include "StrykerCharacter.h"
#include "DieselandCharacter.h"
#include "DieselandEnemyBot.h"
#include "UnrealNetwork.h"
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

	//adjustments for damage
	BasicAttackDamage = BaseDamage + (Strength * 1.5f) + (Dexterity * .5f) + (Intelligence * .5f);
	//adjusments for attackspeed
	BasicAttackCooldown = BaseCooldownSpeed / (1 + (Dexterity / 50));

	//adjustments for movement Speed
	MoveSpeed = BaseMoveSpeed + (Dexterity * 3);
	this->CharacterMovement->MaxWalkSpeed = MoveSpeed;

	AssassinationColliderRange = 250.0f;

	//base cooldowns
	BaseSkillOneCooldown = 22.0f;
	BaseSkillTwoCooldown = 9.0f;
	BaseSkillThreeCooldown = 9.0f;

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

	//particle effects
	static ConstructorHelpers::FObjectFinder<UParticleSystem> BlinkParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_StrykerBlinkCloak_WIP.Unreal_Particle_StrykerBlinkCloak_WIP'"));
	this->SkillOneParticle = BlinkParticleAsset.Object;


	bReplicates = true;
	bReplicateMovement = true;
	
	TauntCooldown = 4.0f;
	LaughCooldown = 2.0f;
	CommentCooldown = 2.0f;

	//setting all sounds
	SlashSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Slash Sound"));
	SlashSound->AttachParent = RootComponent;
	SlashSound->bAutoActivate = false;

	UltimateVoice = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Ultimate Voice Over"));
	UltimateVoice->AttachParent = RootComponent;
	UltimateVoice->bAutoActivate = false;

	BlinkSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Blink Sound"));
	BlinkSound->AttachParent = RootComponent;
	BlinkSound->bAutoActivate = false;

	PoisonSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Poison Sound"));
	PoisonSound->AttachParent = RootComponent;
	PoisonSound->bAutoActivate = false;

	AssassinationSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Assassination Sound"));
	AssassinationSound->AttachParent = RootComponent;
	AssassinationSound->bAutoActivate = false;


	//here I set melee to true so that Stryker only uses melee attacks
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
		AssasinationDuration2 += DeltaSeconds;
		SearchForAssassinationTarget();
		IsAttemptingAssassinate = false;
		
	}
	if (AssasinationHitCounter == 6)
	{
		AssasinationDuration = 0;
		AssasinationDuration2 = 0;
		IsAssassinating = false;
		AssasinationHitCounter = 0;
		AActor* AssassinationTarget = NULL;
		this->StatusEffects.Remove(FString("Stunned"));
	
		this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
		this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
		this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);
		this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
		this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Block);
	}
}

bool AStrykerCharacter::UpdateDurationTimers_Validate(float DeltaSeconds)
{
	return true;
}

void AStrykerCharacter::SearchForAssassinationTarget_Implementation()
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
	FVector AssassinateLocation;

	for (int32 b = 0; b < ActorsInMeleeRange.Num(); b++)
	{
		CurActor = ActorsInMeleeRange[b];
		if (!CurActor && (CurActor->ActorHasTag(FName(TEXT("Player"))) || CurActor->ActorHasTag(FName(TEXT("Enemy"))))) continue;
		if (!CurActor->IsValidLowLevel()) continue;

		if (Role == ROLE_Authority && CurActor != this){
			if (IsAssassinating == false)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message! %"), CurActor);
				this->CharacterMovement->Velocity += FVector(0, 0, 0);
				if ((CurActor->ActorHasTag(FName(TEXT("Player")))))
				{
					//this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
					//this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
					UE_LOG(LogTemp, Log, TEXT("Log text %s"), *CurActor->GetName());
					IsAssassinating = true;
					ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(CurActor);
					AssassinationTarget = CurActor;
				}
				else if ((CurActor->ActorHasTag(FName(TEXT("Enemy")))))
				{
					//this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
					//this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
					this->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
					UE_LOG(LogTemp, Log, TEXT("Log text %s"), *CurActor->GetName());
					IsAssassinating = true;
					ADieselandEnemyBot* DieselandPawn = Cast<ADieselandEnemyBot>(CurActor);
					AssassinationTarget = CurActor;
				}
			}
		}
	}
	//used for figuring out the distanec the player must move to strike with assassinate
	if (AssassinationTarget != NULL){
		//here we test to see if the target is dead, if so we stop attacking
		if (AssassinationTarget->ActorHasTag(FName(TEXT("Enemy")))){
			ADieselandEnemyBot* DieselandPawn = Cast<ADieselandEnemyBot>(AssassinationTarget);
			if (DieselandPawn->Health <= 0){
				AssasinationHitCounter = 6;
				return;
			}
		}
		else if (AssassinationTarget->ActorHasTag(FName(TEXT("Player")))){
			ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(AssassinationTarget);
			if (DieselandPawn->Health <= 0){
				AssasinationHitCounter = 6;
				return;
			}
		}
		//here we move around the player
		if (AssasinationDuration2 > 0.25f && AssasinationDuration > 0.3f)
		{
			FVector VectorPlayer = this->GetActorLocation();
			FVector VectorTarget = AssassinationTarget->GetActorLocation();
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
			AssasinationDuration2 = 0;
			int32 randomNumber = FMath::FRandRange(0, 7000);
			int32 randomNumber2 = FMath::FRandRange(0, 7000);
			this->CharacterMovement->Velocity += FVector(-MoveCharacterX * randomNumber, -MoveCharacterY * randomNumber2, 0);
		}
		//here I  rotate towards the player before attacking
		if (AssasinationDuration > 0.4f){
			FRotator NewRot = FRotationMatrix::MakeFromX(AssassinationTarget->GetActorLocation() - this->GetActorLocation()).Rotator();
			//FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), AssassinationTarget->GetActorLocation());
			this->SetActorRotation(NewRot);
		}
		//here we attack the player
		if (AssasinationDuration > 0.5f){
		//	FRotator NewRot = (this->GetActorLocation() - AssassinationTarget->GetActorLocation()).Rotation();
			//this->SetActorRotation(NewRot);
	
			FVector VectorPlayer = this->GetActorLocation();
			FVector VectorTarget = AssassinationTarget->GetActorLocation();
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

			EditHealth(-1 * (BasicAttackDamage + Dexterity), AssassinationTarget);
			AssasinationDuration = 0;
			AssasinationDuration2 = 0;
			AssasinationHitCounter++;
			//here we continually increase the distance in which stryker moves because stryker may have to move further each swing to hit the player
			if (AssasinationHitCounter == 1)
			{
				this->CharacterMovement->Velocity += FVector(-MoveCharacterX * 13000, -MoveCharacterY * 13000, 0);
			}
			else if (AssasinationHitCounter == 2)
			{
				this->CharacterMovement->Velocity += FVector(-MoveCharacterX * 14000, -MoveCharacterY * 14000, 0);
			}
			else if (AssasinationHitCounter == 3)
			{
				this->CharacterMovement->Velocity += FVector(-MoveCharacterX * 15000, -MoveCharacterY * 15000, 0);
			}
			else if (AssasinationHitCounter == 4)
			{
				this->CharacterMovement->Velocity += FVector(-MoveCharacterX * 16000, -MoveCharacterY * 16000, 0);
			}
			else if (AssasinationHitCounter == 5)
			{
				this->CharacterMovement->Velocity += FVector(-MoveCharacterX * 17000, -MoveCharacterY * 17000, 0);
			}
			else if (AssasinationHitCounter == 6)
			{
				this->CharacterMovement->Velocity += FVector(-MoveCharacterX * 18000, -MoveCharacterY * 18000, 0);
			}
		}
	}
}

bool AStrykerCharacter::SearchForAssassinationTarget_Validate()
{
	return true;
}
// Stryker Assasinate
void AStrykerCharacter::SkillOne()
{
	//here I ensure the player can't cast this ability when in air as it will cause a bug...
	if (this->CharacterMovement->Velocity.Z > 0 || this->CharacterMovement->Velocity.Z < 0){
		SkillOneCooldown = 0;
		return;
	}
	UWorld* const World = GetWorld();
	if (World){
		UltimateVoice->Play();
		FRotator CharacterRotation = Mesh->GetSocketRotation(FName(TEXT("AimSocket")));
		CharacterRotation = FRotator(CharacterRotation.Pitch, CharacterRotation.Yaw + 90.0f, CharacterRotation.Roll);
		this->SetActorRotation(CharacterRotation);
		FVector Direction = CharacterRotation.Vector();
		this->StatusEffects.Add(FString("Stunned"));
		this->CharacterMovement->Velocity += FVector(Direction.X * 12500, Direction.Y * 12500,0);
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
	//here I ensure the player can't cast this ability when in air as it will cause a bug...
	if (this->CharacterMovement->Velocity.Z > 0 || this->CharacterMovement->Velocity.Z < 0){
		SkillThreeCooldown = 0;
		return;
	}
	UWorld* const World = GetWorld();
	if (World){
		ServerActivateParticle(SkillOneParticle);
		FRotator CharacterRotation = Mesh->GetSocketRotation(FName(TEXT("AimSocket")));
		CharacterRotation = FRotator(CharacterRotation.Pitch, CharacterRotation.Yaw + 90.0f, CharacterRotation.Roll);
		this->SetActorRotation(CharacterRotation);
		FVector Direction = CharacterRotation.Vector();
		//this->StatusEffects.Add(FString("Stunned"));
		this->CharacterMovement->Velocity += FVector(Direction.X * 18000, Direction.Y * 18000, 0);
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


