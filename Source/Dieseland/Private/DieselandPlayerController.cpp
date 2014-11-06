// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Dieseland.h"
#include "DieselandPlayerController.h"
#include "DieselandPlayerState.h"
#include "DieselandGameMode.h"
#include "AI/Navigation/NavigationSystem.h"
#include "DieselandCharacter.h"
#include "UnrealNetwork.h"
#include "BaseTrap.h"
#include "Scrap.h"
#include "DieselandStaticLibrary.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

ADieselandPlayerController::ADieselandPlayerController(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Used for showing mouse cursor
	//bShowMouseCursor = true;
	//DefaultMouseCursor = EMouseCursor::Crosshairs;

	static ConstructorHelpers::FObjectFinder<UClass> MayhemBPClass(TEXT("Class'/Game/Blueprints/Players/Mayhem_BP.Mayhem_BP_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> EngletonBPClass(TEXT("Class'/Game/Blueprints/Engleton.Engleton_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> StrykerBPClass(TEXT("Class'/Game/Blueprints/Players/Stryker_BP.Stryker_BP_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> FoxBPClass(TEXT("Class'/Game/Blueprints/Players/Fox_BP.Fox_BP_C'"));

	static ConstructorHelpers::FObjectFinder<UClass> ScrapBlueprint(TEXT("Class'/Game/Blueprints/Scrap_BP.Scrap_BP_C'"));
	if (ScrapBlueprint.Object)
	{
		ScrapClass = (UClass*)ScrapBlueprint.Object;
	}

	if (MayhemBPClass.Object)
	{
		MayhemClass = MayhemBPClass.Object;
		EngletonClass = EngletonBPClass.Object;
		StrykerClass = StrykerBPClass.Object;
		FoxClass = FoxBPClass.Object;
	}

	HealthRegenTimer = 0;
	bReplicates = true;
	LingerCount = 0;
	StatPlusCount = 0;
	PawnChosen = false;
	PauseGameInput = true;
}

void ADieselandPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	UpdateCooldownTimers(DeltaTime);
	
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr){

		// Temporary on screen cooldown display
		/*GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor::Green, FString("Basic Attack Ammo: ") + FString::SanitizeFloat(DieselandPawn->BasicAttackAmmo));
		GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Blue, FString("Basic Attack: ") + FString::SanitizeFloat(DieselandPawn->BasicAttackTimer));
		GEngine->AddOnScreenDebugMessage(2, 10.0f, FColor::Red, FString("Skill One: ") + FString::SanitizeFloat(DieselandPawn->SkillOneTimer));
		GEngine->AddOnScreenDebugMessage(3, 10.0f, FColor::Green, FString("Skill Two: ") + FString::SanitizeFloat(DieselandPawn->SkillTwoTimer));
		GEngine->AddOnScreenDebugMessage(4, 10.0f, FColor::Yellow, FString("Skill Three: ") + FString::SanitizeFloat(DieselandPawn->SkillThreeTimer));
		GEngine->AddOnScreenDebugMessage(5, 10.0f, FColor::Black, FString("Trap Countdown: ") + FString::SanitizeFloat(DieselandPawn->LingerTimer));
		GEngine->AddOnScreenDebugMessage(20, 10.0f, FColor::Red, FString("Kills: ") + FString::FromInt(DieselandPawn->Kills));

		if (DieselandPawn->BasicAttackReloadTimer > 0.0f){
			GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Yellow, FString("Basic Attack Reload: ") + FString::SanitizeFloat(DieselandPawn->BasicAttackReloadTimer));
		}*/

		if (DieselandPawn->Health <= 0)
		{
			//Prevents multiple respawns
			DieselandPawn->Health = 1;

			RespawnPawn();
			DieselandPawn->LingerTimer = 0;

		}
	}
}

void ADieselandPlayerController::UpdateCooldownTimers_Implementation(float DeltaSeconds)
{
	if (Cast<ADieselandCharacter>(GetPawn()) != nullptr){
		ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());

		

		//reset to max health
		if (DieselandPawn->Health > DieselandPawn->MaxHealth){
			DieselandPawn->Health = DieselandPawn->MaxHealth;
		}
		HealthRegenTimer += DeltaSeconds;
		//health regeneration
		if (HealthRegenTimer >= 1){
			if (DieselandPawn->Health < DieselandPawn->MaxHealth){
				DieselandPawn->Health += DieselandPawn->HealthRegeneration;
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));
			}
			HealthRegenTimer = 0;
		}

		// Update all of the timers
		if (DieselandPawn->BasicAttackTimer > 0.0f)
		{
			DieselandPawn->BasicAttackTimer -= DeltaSeconds;
			if (DieselandPawn->BasicAttackTimer < 0.0f)
			{
				DieselandPawn->BasicAttackTimer = 0.0f;
			}
		}
		if (DieselandPawn->BasicAttackReloadTimer > 0.0f)
		{
			DieselandPawn->BasicAttackReloadTimer -= DeltaSeconds;
			if (DieselandPawn->BasicAttackReloadTimer < 0.0f)
			{
				DieselandPawn->BasicAttackReloadTimer = 0.0f;
			}
			if (DieselandPawn->BasicAttackReloadTimer == 0.0f && DieselandPawn->BasicAttackAmmo == 0)
			{
				DieselandPawn->BasicAttackAmmo = DieselandPawn->BasicAttackMag;
			}
		}
		if (DieselandPawn->LingerTimer > 0.0f)
		{
			DieselandPawn->LingerTimer -= DeltaSeconds;
			if (DieselandPawn->LingerTimer < 0.0f)
			{
				DieselandPawn->LingerTimer = 0;
				LingerCount = 0;
			}
			if ((((DieselandPawn->LingerTimer < 5.f) && (DieselandPawn->LingerTimer > 4.f)) || ((DieselandPawn->LingerTimer < 3.f) && (DieselandPawn->LingerTimer > 2.f)) || ((DieselandPawn->LingerTimer < 1.f) && (DieselandPawn->LingerTimer > 0.f))) && (LingerCount == 0))
			{
				DieselandPawn->Health = DieselandPawn->Health - DieselandPawn->LingerDamage;
				LingerCount = 1;
			}
			else if ((((DieselandPawn->LingerTimer < 4.f) && (DieselandPawn->LingerTimer > 3.f)) || ((DieselandPawn->LingerTimer < 2.f) && (DieselandPawn->LingerTimer > 1.f))) && (LingerCount == 1))
			{
				DieselandPawn->Health = DieselandPawn->Health - DieselandPawn->LingerDamage;
				LingerCount = 0;
			}
		}
		if (DieselandPawn->SkillOneTimer > 0.0f)
		{
			DieselandPawn->SkillOneTimer -= DeltaSeconds;
			if (DieselandPawn->SkillOneTimer < 0.0f)
			{
				DieselandPawn->SkillOneTimer = 0.0f;
			}
		}

		if (DieselandPawn->SkillTwoTimer > 0.0f)
		{
			DieselandPawn->SkillTwoTimer -= DeltaSeconds;
			if (DieselandPawn->SkillTwoTimer < 0.0f)
			{
				DieselandPawn->SkillTwoTimer = 0.0f;
			}
		}

		if (DieselandPawn->SkillThreeTimer > 0.0f)
		{
			DieselandPawn->SkillThreeTimer -= DeltaSeconds;
			if (DieselandPawn->SkillThreeTimer < 0.0f)
			{
				DieselandPawn->SkillThreeTimer = 0.0f;
			}
		}

		if (DieselandPawn->LaughTimer > 0.0f)
		{
			DieselandPawn->LaughTimer -= DeltaSeconds;
			if (DieselandPawn->LaughTimer < 0.0f)
			{
				DieselandPawn->LaughTimer = 0.0f;
			}
		}

		if (DieselandPawn->CommentTimer > 0.0f)
		{
			DieselandPawn->CommentTimer -= DeltaSeconds;
			if (DieselandPawn->CommentTimer < 0.0f)
			{
				DieselandPawn->CommentTimer = 0.0f;
			}
		}


		if (DieselandPawn->TauntTimer > 0.0f)
		{
			DieselandPawn->TauntTimer -= DeltaSeconds;
			if (DieselandPawn->TauntTimer < 0.0f)
			{
				DieselandPawn->TauntTimer = 0.0f;
			}
		}


		// Basic Attack actions
		if (DieselandPawn->BasicAttackTimer <= 0.0f && DieselandPawn->BasicAttackReloadTimer <= 0.0f && DieselandPawn->BasicAttackActive)
		{
			if (DieselandPawn->IsMelee)
			{
				ServerMeleeAttack();
				DieselandPawn->BasicAttackTimer = DieselandPawn->BasicAttackCooldown;
			}
			else
			{
				ServerRangedAttack();
				DieselandPawn->BasicAttackTimer = DieselandPawn->BasicAttackCooldown;
				DieselandPawn->BasicAttackAmmo -= 1;
				if (DieselandPawn->BasicAttackAmmo <= 0){
					DieselandPawn->BasicAttackReloadTimer = DieselandPawn->BasicAttackReloadSpeed;
				}
			}
		}
		if (DieselandPawn->StunRemaining > 0.0f)
		{
			DieselandPawn->StunRemaining -= DeltaSeconds;
			if (DieselandPawn->StunRemaining <= 0.0f)
			{
				DieselandPawn->StunRemaining = 0.0f;
				DieselandPawn->StatusEffects.Remove(FString("Stunned"));
				//here we check and see if the play is also charmed, if so I then remove charm from the player
				if (DieselandPawn->StatusEffects.Contains(FString("Charmed")))
				{
					DieselandPawn->StatusEffects.Remove(FString("Charmed"));
				}
			}
			if (DieselandPawn->StatusEffects.Contains(FString("Charmed")))
			{
				DieselandPawn->CharacterMovement->Velocity += DieselandPawn->VectorDirection * 300;
			}
		}
		if (DieselandPawn->SlowRemaining > 0.0f)
		{
			if (DieselandPawn->SlowRemaining > 0.0f){
				DieselandPawn->SlowRemaining -= DeltaSeconds;
			}
			if (DieselandPawn->SlowRemaining <= 0.0f)
			{
				DieselandPawn->StatusEffects.Remove(FString("Slowed"));
				DieselandPawn->CharacterMovement->MaxWalkSpeed = DieselandPawn->MoveSpeed;
				DieselandPawn->SlowRemaining = 0.0f;
			}
		}
	}
}

bool ADieselandPlayerController::UpdateCooldownTimers_Validate(float DeltaSeconds)
{
	return true;
}

void ADieselandPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ADieselandPlayerController::OnMoveForward);
	InputComponent->BindAxis("MoveRight", this, &ADieselandPlayerController::OnMoveRight);

	InputComponent->BindAxis("LookNorth", this, &ADieselandPlayerController::OnFaceNorth);
	InputComponent->BindAxis("LookEast", this, &ADieselandPlayerController::OnFaceEast);

	InputComponent->BindAction("Attack", IE_Pressed, this, &ADieselandPlayerController::OnAttackPress);
	InputComponent->BindAction("Attack", IE_Released, this, &ADieselandPlayerController::OnAttackRelease);

	InputComponent->BindAction("Skill_1", IE_Pressed, this, &ADieselandPlayerController::ServerSkillOne);
	InputComponent->BindAction("Skill_2", IE_Pressed, this, &ADieselandPlayerController::ServerSkillTwo);
	InputComponent->BindAction("Skill_3", IE_Pressed, this, &ADieselandPlayerController::ServerSkillThree);

	InputComponent->BindAction("Reload", IE_Pressed, this, &ADieselandPlayerController::ServerReload);

	InputComponent->BindAction("UpgradeStrength", IE_Pressed, this, &ADieselandPlayerController::UpgradeStrength);
	InputComponent->BindAction("UpgradeIntelligence", IE_Pressed, this, &ADieselandPlayerController::UpgradeIntelligence);
	InputComponent->BindAction("UpgradeDexterity", IE_Pressed, this, &ADieselandPlayerController::UpgradeDexterity);
	InputComponent->BindAction("UpgradeConstitution", IE_Pressed, this, &ADieselandPlayerController::UpgradeConstitution);

	InputComponent->BindAction("Taunt", IE_Pressed, this, &ADieselandPlayerController::OnTaunt);
	InputComponent->BindAction("Laugh", IE_Pressed, this, &ADieselandPlayerController::OnLaugh);
	InputComponent->BindAction("Comment", IE_Pressed, this, &ADieselandPlayerController::OnComment);

	InputComponent->BindAction("Escape", IE_Pressed, this, &ADieselandPlayerController::OnEscape);

	InputComponent->BindAction("Score", IE_Released, this, &ADieselandPlayerController::OnShowScore);
}

void ADieselandPlayerController::ReceiveBeginPlay()
{
	if (Role == ROLE_Authority)
	{
//		Cast<ADieselandGameMode>(GetWorld()->GetAuthGameMode())->StartGame();
	}
}

bool ADieselandPlayerController::RespawnPawn_Validate()
{
	return true;
}

void ADieselandPlayerController::RespawnPawn_Implementation()
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());

	if (DieselandPawn != nullptr)
	{
		if (Role == ROLE_Authority)
		{
			FVector TempEnemyLoc = FVector(DieselandPawn->GetActorLocation().X, DieselandPawn->GetActorLocation().Y, DieselandPawn->GetActorLocation().Z);

			DieselandPawn->SetActorLocation(SpawnLocation);
			DieselandPawn->Health = DieselandPawn->MaxHealth;
			DieselandPawn->LingerTimer = 0;

			//Spawn Scrap pieces here
			UWorld* const World = GetWorld();
			if (World && DieselandPawn->LatestDamageCauser->ActorHasTag(FName(TEXT("Player"))))
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
}

void ADieselandPlayerController::ChangeCharacter(FString Character)
{
	ServerChangeCharacter(Character);
	GetWorldTimerManager().SetTimer(this, &ADieselandPlayerController::PossessNewPawn, 0.3f, false);
}

void ADieselandPlayerController::PossessNewPawn()
{
	//if (Role != ROLE_Authority)
	//{
		ServerPossessNewPawn();
	//}
}

void ADieselandPlayerController::ServerPossessNewPawn_Implementation()
{
	this->Possess(NewPawn);
}

bool ADieselandPlayerController::ServerPossessNewPawn_Validate()
{
	return true;
}
void ADieselandPlayerController::ServerChangeCharacter_Implementation(const FString& Character)
{
	APawn* TempPawn = GetPawn();

	//this->UnPossess();
	if (GetWorld() != nullptr && Role == ROLE_Authority){
		//Determine what player is desired and spawn that pawn
		if (Character == FString(TEXT("Mayhem")))
		{
			this->UnPossess();

			NewPawn = UDieselandStaticLibrary::SpawnBlueprint<APawn>(GetWorld(),
				MayhemClass,
				FVector(TempPawn->GetActorLocation().X + (70.0f), TempPawn->GetActorLocation().Y, TempPawn->GetActorLocation().Z),
				FRotator(0.0f, 0.0f, 0.0f));

			// Timer workaround to ensure replication
			//GetWorldTimerManager().SetTimer(this, &ADieselandPlayerController::PossessNewPawn, 0.3f, false);

		}
		else if (Character == FString(TEXT("Engleton")))
		{
			this->UnPossess();

			NewPawn = UDieselandStaticLibrary::SpawnBlueprint<APawn>(GetWorld(),
				EngletonClass,
				FVector(TempPawn->GetActorLocation().X + (70.0f), TempPawn->GetActorLocation().Y, TempPawn->GetActorLocation().Z),
				FRotator(0.0f, 0.0f, 0.0f));

			// Timer workaround to ensure replication
			//GetWorldTimerManager().SetTimer(this, &ADieselandPlayerController::PossessNewPawn, 0.3f, false);


		}
		else if (Character == FString(TEXT("Stryker")))
		{
			this->UnPossess();

			NewPawn = UDieselandStaticLibrary::SpawnBlueprint<APawn>(GetWorld(),
				StrykerClass,
				FVector(TempPawn->GetActorLocation().X + (70.0f), TempPawn->GetActorLocation().Y, TempPawn->GetActorLocation().Z),
				FRotator(0.0f, 0.0f, 0.0f));

			// Timer workaround to ensure replication
			//GetWorldTimerManager().SetTimer(this, &ADieselandPlayerController::PossessNewPawn, 0.3f, false);


		}
		else if (Character == FString(TEXT("Fox")))
		{
			this->UnPossess();

			NewPawn = UDieselandStaticLibrary::SpawnBlueprint<APawn>(GetWorld(),
				FoxClass,
				FVector(TempPawn->GetActorLocation().X + (70.0f), TempPawn->GetActorLocation().Y, TempPawn->GetActorLocation().Z),
				FRotator(0.0f, 0.0f, 0.0f));

			// Timer workaround to ensure replication
			//GetWorldTimerManager().SetTimer(this, &ADieselandPlayerController::PossessNewPawn, 0.3f, false);


		}

		PauseGameInput = false;
		// Destroy previous pawn
		TempPawn->Destroy();
	}
	GetWorldTimerManager().SetTimer(this, &ADieselandPlayerController::PossessNewPawn, 0.3f, false);
}

bool ADieselandPlayerController::ServerChangeCharacter_Validate(const FString& Character)
{
	return true;
}

bool ADieselandPlayerController::ServerReload_Validate()
{
	return true;
}

void ADieselandPlayerController::ServerReload_Implementation()
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());

	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned")))
	{
		if (DieselandPawn->BasicAttackAmmo < DieselandPawn->BasicAttackMag && DieselandPawn->BasicAttackReloadTimer <= 0.0f)
		{
			DieselandPawn->BasicAttackReloadTimer = DieselandPawn->BasicAttackReloadSpeed;
			DieselandPawn->BasicAttackAmmo = 0;
		}
	}
}

void ADieselandPlayerController::OnEscape()
{
	// TODO: Add pause menu rather than exiting to menu, and additional MP cleanup

	if (Role != ROLE_Authority)
	{
		APawn* TempPawn = GetPawn();
		this->UnPossess();
		TempPawn->Destroy();
	}
	GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();
	ConsoleCommand("open Dieseland_UserInterface");
}

void ADieselandPlayerController::OnShowScore()
{
	// TODO: Add score screen
}


void ADieselandPlayerController::ServerEditHealth_Implementation(int32 Amt, AActor* Target)
{
	// Edit the health of the specific pawn
	if (GetPawn() != nullptr)
	{
		Cast<ADieselandCharacter>(GetPawn())->EditHealth(Amt, Target);
	}
}
bool ADieselandPlayerController::ServerEditHealth_Validate(int32 Amt, AActor* Target)
{
	return true;
}



void ADieselandPlayerController::ServerEditSpeedDamage_Implementation(int32 Speed, int32 Damage, AActor* Target)
{
	// Edit the health of the specific pawn
	if (GetPawn() != nullptr)
	{
		Cast<ADieselandCharacter>(GetPawn())->EditSpeedDamage(Speed, Damage, Target);
	}
}

bool ADieselandPlayerController::ServerEditSpeedDamage_Validate(int32 Speed, int32 Damage, AActor* Target)
{
	return true;
}

void ADieselandPlayerController::OnMoveForward(float Val)
{
	if (GetPawn() != nullptr && !Cast<ADieselandCharacter>(GetPawn())->StatusEffects.Contains(FString("Stunned")) && !PauseGameInput){
		GetPawn()->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Val);
	}
}

void ADieselandPlayerController::OnMoveRight(float Val)
{
	if (GetPawn() != nullptr && !Cast<ADieselandCharacter>(GetPawn())->StatusEffects.Contains(FString("Stunned")) && !PauseGameInput){
		GetPawn()->AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Val);
	}
}

void ADieselandPlayerController::OnFaceNorth(float Val)
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	
	if (GetPawn() != nullptr && Val != 0.0f && DieselandPawn->StatusEffects.Contains(FString("Stunned")) && !PauseGameInput){

		// Convert the joystick axis to a rotator
		FVector TempAxisVector = FVector(Val, (GetInputAxisValue("LookEast") * 1.0f), 0.0f);
		FacingRotation = FRotationMatrix::MakeFromX(TempAxisVector).Rotator();
		
		// If the rotation value is different from previous, change the rotation
		if (DieselandPawn->AimRotation != FacingRotation){
			// Edit the pawn's spine rotation
			DieselandPawn->AimRotation = FacingRotation;
			// If this is the client, send the the rotator to the server
			if (Role < ROLE_Authority){
				ServerOnAim(FacingRotation);
			}
		}
	}
}
void ADieselandPlayerController::OnFaceEast(float Val)
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());

	if (GetPawn() != nullptr && Val != 0.0f && !DieselandPawn->StatusEffects.Contains(FString("Stunned")) && !PauseGameInput){
		
		// Convert the joystick axis to a rotator
		FVector TempAxisVector = FVector(GetInputAxisValue("LookNorth"), Val * 1.0f, 0.0f);
		FacingRotation = FRotationMatrix::MakeFromX(TempAxisVector).Rotator();

		// If the rotation value is different from previous, change the rotation
		if (DieselandPawn->AimRotation != FacingRotation){
			// Edit the pawn's spine rotation
			DieselandPawn->AimRotation = FacingRotation;
			// If this is the client, send the the rotator to the server
			if (Role < ROLE_Authority){
				ServerOnAim(FacingRotation);
			}
		}
	}
}

void ADieselandPlayerController::OnAttackPress_Implementation()
{
	if (GetPawn() != nullptr && !Cast<ADieselandCharacter>(GetPawn())->StatusEffects.Contains(FString("Stunned")) && !PauseGameInput){
		Cast<ADieselandCharacter>(GetPawn())->BasicAttackActive = true;
	}
}

bool ADieselandPlayerController::OnAttackPress_Validate()
{
	return true;
}

void ADieselandPlayerController::OnAttackRelease_Implementation()
{
	if (GetPawn() != nullptr){
		Cast<ADieselandCharacter>(GetPawn())->BasicAttackActive = false;
	}
}

bool ADieselandPlayerController::OnAttackRelease_Validate()
{
	return true;
}

void ADieselandPlayerController::ServerSkillOne_Implementation()
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned")) && !PauseGameInput){
		if (DieselandPawn->SkillOneTimer <= 0.0f)
		{
			DieselandPawn->SkillOne();
			DieselandPawn->SkillOneTimer = DieselandPawn->SkillOneCooldown;
		}
	}
}

bool ADieselandPlayerController::ServerSkillOne_Validate()
{
	return true;
}

void ADieselandPlayerController::ServerSkillTwo_Implementation()
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned")) && !PauseGameInput){
		if (DieselandPawn->SkillTwoTimer <= 0.0f)
		{
			DieselandPawn->SkillTwo();
			DieselandPawn->SkillTwoTimer = DieselandPawn->SkillTwoCooldown;
		}
	}
}

bool ADieselandPlayerController::ServerSkillTwo_Validate()
{
	return true;
}

void ADieselandPlayerController::ServerSkillThree_Implementation()
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned")) && !PauseGameInput){
		if (DieselandPawn->SkillThreeTimer <= 0.0f)
		{
			DieselandPawn->SkillThree();
			DieselandPawn->SkillThreeTimer = DieselandPawn->SkillThreeCooldown;
		}
	}
}

bool ADieselandPlayerController::ServerSkillThree_Validate()
{
	return true;
}

void ADieselandPlayerController::OnTaunt_Implementation()
{
	//here is the real level up function
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned")) && !PauseGameInput)
	{
		if (DieselandPawn->TauntTimer <= 0.0f && DieselandPawn->CommentTimer <= 0.0f && DieselandPawn->LaughTimer <= 0.0f)
		{
			DieselandPawn->Taunt();
			DieselandPawn->TauntTimer = DieselandPawn->TauntCooldown;
			//DieselandPawn->SkillThreeTimer = DieselandPawn->SkillThreeCooldown;
		}
	}
}



bool ADieselandPlayerController::OnTaunt_Validate()
{
	return true;
}

void ADieselandPlayerController::OnLaugh_Implementation()
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned")) && !PauseGameInput)
	{
		if (DieselandPawn->TauntTimer <= 0.0f && DieselandPawn->CommentTimer <= 0.0f && DieselandPawn->LaughTimer <= 0.0f)
		{
			DieselandPawn->Laugh();
			DieselandPawn->LaughTimer = DieselandPawn->LaughCooldown;
			//DieselandPawn->SkillThreeTimer = DieselandPawn->SkillThreeCooldown;
		}
	}
}

bool ADieselandPlayerController::OnLaugh_Validate()
{
	return true;
}

void ADieselandPlayerController::OnComment_Implementation()
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned")) && !PauseGameInput)
	{
		if (DieselandPawn->TauntTimer <= 0.0f && DieselandPawn->CommentTimer <= 0.0f && DieselandPawn->LaughTimer <= 0.0f)
		{
			DieselandPawn->Comment();
			DieselandPawn->CommentTimer = DieselandPawn->CommentCooldown;
			//DieselandPawn->SkillThreeTimer = DieselandPawn->SkillThreeCooldown;
		}
	}

}

bool ADieselandPlayerController::OnComment_Validate()
{
	return true;
}


void ADieselandPlayerController::UpgradeStrength_Implementation()
{
	//here is the real level up function
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned")) && DieselandPawn->CharacterLevel < 20 && DieselandPawn->Scrap >= 3 && !PauseGameInput){
		DieselandPawn->Scrap -= 3;
		DieselandPawn->Strength += 3;
		DieselandPawn->CalculateStats();
		GEngine->AddOnScreenDebugMessage(5, 5.0f, FColor::Cyan, FString("Upgraded Strength!"));
		StatPlusCount++;
	}
}
 


bool ADieselandPlayerController::UpgradeStrength_Validate()
{
	return true;
}

void ADieselandPlayerController::UpgradeIntelligence_Implementation()
{
	//here is the real level up function
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned")) && DieselandPawn->CharacterLevel < 20 && DieselandPawn->Scrap >= 3 && !PauseGameInput){
		DieselandPawn->Scrap -= 3;
		DieselandPawn->Intelligence += 3;
		DieselandPawn->CalculateStats();
		GEngine->AddOnScreenDebugMessage(5, 5.0f, FColor::Cyan, FString("Upgraded Intelligence!"));
		StatPlusCount++;
	}
}

bool ADieselandPlayerController::UpgradeIntelligence_Validate()
{
	return true;
}
void ADieselandPlayerController::UpgradeDexterity_Implementation()
{
	//here is the real level up function
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned")) && DieselandPawn->CharacterLevel < 20 && DieselandPawn->Scrap >= 3 && !PauseGameInput){
		DieselandPawn->Scrap -= 3;
		DieselandPawn->Dexterity += 3;
		DieselandPawn->CalculateStats();
		GEngine->AddOnScreenDebugMessage(5, 5.0f, FColor::Cyan, FString("Upgraded Dexterity!"));
		StatPlusCount++;
	}
}

bool ADieselandPlayerController::UpgradeDexterity_Validate()
{
	return true;
}
void ADieselandPlayerController::UpgradeConstitution_Implementation()
{
	//here we upgrade the players constitution
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned")) && DieselandPawn->CharacterLevel < 20 && DieselandPawn->Scrap >= 3 && !PauseGameInput){
		DieselandPawn->Scrap -= 3;
		DieselandPawn->Constitution += 3;
		DieselandPawn->CalculateStats();
		GEngine->AddOnScreenDebugMessage(5, 5.0f, FColor::Cyan, FString("Upgraded Constitution!"));
		StatPlusCount++;
	}
}

bool ADieselandPlayerController::UpgradeConstitution_Validate()
{
	return true;
}


void ADieselandPlayerController::SwapMelee_Implementation()
{
	//Uncomment only if you need to test melee/ranged swapping
	/*if (Cast<ADieselandCharacter>(GetPawn())->IsMelee)
	{
		Cast<ADieselandCharacter>(GetPawn())->IsMelee = false;
		GEngine->AddOnScreenDebugMessage(6, 10.0f, FColor::Cyan, FString("Now using ranged attack"));
	}
	else
	{
		Cast<ADieselandCharacter>(GetPawn())->IsMelee = true;
		GEngine->AddOnScreenDebugMessage(6, 10.0f, FColor::Cyan, FString("Now using melee attack"));
	}*/
	
}

bool ADieselandPlayerController::SwapMelee_Validate()
{
	return true;
}

void ADieselandPlayerController::ServerMeleeAttack_Implementation()
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned")) && !PauseGameInput){
		DieselandPawn->MeleeAttack();
	}
}

bool ADieselandPlayerController::ServerMeleeAttack_Validate()
{
	return true;
}


void ADieselandPlayerController::ServerRangedAttack_Implementation()
{
	ADieselandCharacter* DieselandPawn = Cast<ADieselandCharacter>(GetPawn());
	if (DieselandPawn != nullptr && !DieselandPawn->StatusEffects.Contains(FString("Stunned")) && !PauseGameInput){
		DieselandPawn->RangedAttack();
	}
}

bool ADieselandPlayerController::ServerRangedAttack_Validate()
{
	return true;
}

bool ADieselandPlayerController::ServerOnAim_Validate(FRotator Rotation)
{
	return true;
}

void ADieselandPlayerController::ServerOnAim_Implementation(FRotator Rotation)
{
	if (GetPawn() != nullptr && !Cast<ADieselandCharacter>(GetPawn())->StatusEffects.Contains(FString("Stunned")) && !PauseGameInput){
		Cast<ADieselandCharacter>(GetPawn())->AimRotation = Rotation;
	}
}

void ADieselandPlayerController::SetPauseGameInput_Implementation(bool paused)
{
	PauseGameInput = paused;
}

bool ADieselandPlayerController::SetPauseGameInput_Validate(bool paused)
{
	return true;
}

void ADieselandPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ADieselandPlayerController, PauseGameInput);
	DOREPLIFETIME(ADieselandPlayerController, PawnChosen);
	DOREPLIFETIME(ADieselandPlayerController, NewPawn);
}
