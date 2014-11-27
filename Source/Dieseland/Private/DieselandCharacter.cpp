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
#include "DieselandPlayerState.h"

ADieselandCharacter::ADieselandCharacter(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
	//getting cubes and stuff
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Game/Shapes/Shape_Cube.Shape_Cube'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("StaticMesh'/Game/Shapes/Shape_Sphere.Shape_Sphere'"));

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
	TopDownCameraComponent->SetIsReplicated(true);

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

	// Find the mesh to use for AimMesh component
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticAimMesh(TEXT("StaticMesh'/Game/Shapes/Shape_Cube.Shape_Cube'"));

	// Setup the AimMesh component
	AimMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("AimMesh"));
	AimMesh->AttachParent = (Mesh);
	AimMesh->AttachSocketName = FName(TEXT("AimSocket"));
	//AimMesh->SetStaticMesh(StaticAimMesh.Object);
	AimMesh->SetHiddenInGame(true);
	AimMesh->SetIsReplicated(true);

	CharacterLightSource = PCIP.CreateDefaultSubobject<UPointLightComponent>(this, TEXT("LightSource"));
	CharacterLightSource->AttachParent = RootComponent;
	CharacterLightSource->AddRelativeLocation(FVector(0.0f, 10.0f, 460.0f));
	CharacterLightSource->SetIntensity(10000.0f);
	CharacterLightSource->SetLightColor(FColor::FromHex(FString("C2A171FF")));
	CharacterLightSource->SetCastShadows(false);

	static ConstructorHelpers::FObjectFinder<UMaterial> HealthBarTextRef(TEXT("Material'/Game/MaterialsDLC/M_HealthText.M_HealthText'"));

	// Create the text component
	HealthLabel = PCIP.CreateDefaultSubobject<UTextRenderComponent>(this, TEXT("HealthLabel"));
	HealthLabel->AttachTo(RootComponent);
	HealthLabel->AddRelativeLocation(FVector(0.0f, 0.0f, 200.0f), false);
	HealthLabel->Text = FString::FromInt(Health);
	HealthLabel->SetMaterial(0, HealthBarTextRef.Object);
	HealthLabel->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
	HealthLabel->HorizontalAlignment = EHorizTextAligment::EHTA_Center;

	static ConstructorHelpers::FObjectFinder<UMaterial> HealthBarMatRef(TEXT("Material'/Game/UserInterfaceAssets/HUD/Materials/M_HUD_Health_Bar.M_HUD_Health_Bar'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> HealthBarBackMatRef(TEXT("Material'/Game/MaterialsDLC/Material_BasicDarkGrey.Material_BasicDarkGrey'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> AimBarMatRef(TEXT("Material'/Game/UserInterfaceAssets/HUD/Materials/M_HUD_Health_Bar.M_HUD_Health_Bar'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MiniMapMatRef(TEXT("Material'/Game/Materials/MiniMapIcon.MiniMapIcon'"));

	HealthBar = PCIP.CreateDefaultSubobject<UMaterialBillboardComponent>(this, TEXT("HealthBar"));
	HealthBar->AttachParent = RootComponent;
	HealthBar->AddRelativeLocation(FVector(0.0f, 0.0f, 175.0f));
	HealthBarMatStatic = HealthBarMatRef.Object;
	HealthBarBackMatStatic = HealthBarBackMatRef.Object;
	//HealthBarMaterial = UMaterialInstanceDynamic::Create(HealthBarMatRef.Object, this);
	HealthBar->AddElement(HealthBarMatRef.Object, nullptr, false, 10.0f, 75.0f, nullptr);
	HealthBar->AddElement(HealthBarBackMatRef.Object, nullptr, false, 10.0f, 75.0f, nullptr);


	//for the aim bar and aim sphere stuff
	AimBar = PCIP.CreateDefaultSubobject<UStaticMeshComponent >(this, TEXT("AimBar"));
	AimBar->AttachParent = Mesh;
	AimBar->AttachSocketName = FName(TEXT("AimSocket"));
	AimBar->SetStaticMesh(CubeMesh.Object);
	AimBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AimBarMatStatic = AimBarMatRef.Object;
	//for the aim sphere

	//adjustments for foxs aiming mechanism


	AimSphere = PCIP.CreateDefaultSubobject<UStaticMeshComponent >(this, TEXT("AimSphere"));
	AimSphere->AttachParent = Mesh;
	AimSphere->SetStaticMesh(SphereMesh.Object);
	AimSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AimSphere->SetHiddenInGame(true);


	//used for the minimap
	MiniMapIcon = PCIP.CreateDefaultSubobject<UStaticMeshComponent >(this, TEXT("MiniMap Icon"));
	MiniMapIcon->AttachParent = TopDownCameraComponent;
	MiniMapIcon->SetStaticMesh(CubeMesh.Object);
	MiniMapIcon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	IconMatStatic = MiniMapMatRef.Object;
	MiniMapIcon->SetIsReplicated(false);

	

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

	//set default laugh, comment and taunt timers
	LaughCooldown = 5.0f;
	CommentCooldown = 5.0f;
	TauntCooldown = 5.0f;

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
	LaughTimer = 0.0f;
	CommentTimer = 0.0f;
	TauntTimer = 0.0f;

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
	static ConstructorHelpers::FObjectFinder<UParticleSystem> BasicAttackParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_Bullet1.Unreal_Particle_Bullet1'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SkillOneParticleAsset(TEXT("ParticleSystem'/Game/Particles/P_Explosion.P_Explosion'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SkillTwoParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_StrykerBlinkCloak_WIP.Unreal_Particle_StrykerBlinkCloak_WIP'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SkillThreeParticleAsset(TEXT("ParticleSystem'/Game/Particles/Test/Unreal_Particle_EngletonPulse2_WIP.Unreal_Particle_EngletonPulse2_WIP'"));


	this->BasicAttackParticle = BasicAttackParticleAsset.Object;
	this->SkillOneParticle = SkillOneParticleAsset.Object;
	this->SkillTwoParticle = SkillTwoParticleAsset.Object;
	this->SkillThreeParticle = SkillThreeParticleAsset.Object;

	ParticleSystem = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleSystem"));
	ParticleSystem->Template = SkillThreeParticle;
	ParticleSystem->AttachTo(Mesh);
	ParticleSystem->AttachSocketName = FName(TEXT("AimSocket"));
	ParticleSystem->bAutoActivate = false;
	ParticleSystem->SetHiddenInGame(false);
	ParticleSystem->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AOEParticleSystem = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("AOEParticleSystem"));
	AOEParticleSystem->Template = SkillThreeParticle;
	AOEParticleSystem->AttachTo(RootComponent);
	AOEParticleSystem->bAutoActivate = false;
	AOEParticleSystem->SetHiddenInGame(false);
	AOEParticleSystem->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TauntSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Taunt Sound"));
	TauntSound->AttachParent = RootComponent;
	TauntSound->bAutoActivate = false;

	CommentSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Laugh Sound"));
	CommentSound->AttachParent = RootComponent;
	CommentSound->bAutoActivate = false;

	LaughSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Comment Sound"));
	LaughSound->AttachParent = RootComponent;
	LaughSound->bAutoActivate = false;

	ReloadSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Reload Sound"));
	ReloadSound->AttachParent = RootComponent;
	ReloadSound->bAutoActivate = false;
    
    static ConstructorHelpers::FObjectFinder<USoundBase> CharSelectBase(TEXT("SoundBase'/Game/AudioDLC/Interface/Sound_Menu_BackgroundSongwithAmbience_WIP.Sound_Menu_BackgroundSongwithAmbience_WIP'"));
    CharSelectMusic = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("CharSelectMusic"));
	CharSelectMusic->AttachParent = RootComponent;
	CharSelectMusic->bAutoActivate = false;
    CharSelectMusic->SetSound(CharSelectBase.Object);
    StopMusic= false;
    

	//Find the scrap blueprint's class
	static ConstructorHelpers::FObjectFinder<UClass> ScrapBlueprint(TEXT("Class'/Game/Blueprints/Scrap_BP.Scrap_BP_C'"));
	if (ScrapBlueprint.Object)
	{
		ScrapClass = (UClass*)ScrapBlueprint.Object;
	}

	// Ensure replication
	bReplicates = true;
	bReplicateMovement = true;
	bReplicateInstigator = true;
	AimMesh->SetIsReplicated(true);
	Mesh->SetIsReplicated(true);
	ParticleSystem->SetIsReplicated(true);
	SetActorTickEnabled(true);
	this->SetOwner(Controller);
}

void ADieselandCharacter::ReceiveBeginPlay()
{
    CharSelectMusic->Play();
	AimBarMaterial = UMaterialInstanceDynamic::Create(AimBarMatStatic, this);

	//AimBar->SetWorldLocation(FVector(0, 0, -50));
	AimBar->SetWorldScale3D(FVector(4.0f, 1.0, 0.01));
	AimBar->CastShadow = false;
	AimBar->Materials.Add(AimBarMaterial);

	if (IsFox)
	{
		AimBar->AddLocalRotation(FRotator(90, 0, 0));
	}

	MiniMapMaterial = UMaterialInstanceDynamic::Create(IconMatStatic, this);
	MiniMapIcon->SetWorldScale3D(FVector(12.0f, 12.0, 0.01f));
	MiniMapIcon->SetWorldRotation(FRotator(0, 90.0f, 0));

	MiniMapIcon->AddRelativeLocation(FVector(0.0f, 0.0f,1000.0f));
	MiniMapIcon->CastShadow = false;



	Cast<UMaterialInstanceDynamic>(AimBar->Materials[0])->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(0.0, 0.75f, 0.0f));
	Cast<UMaterialInstanceDynamic>(AimBar->Materials[0])->SetScalarParameterValue(FName(TEXT("Health percentage")), 1.0f);
	Cast<UMaterialInstanceDynamic>(AimBar->Materials[0])->SetScalarParameterValue(FName(TEXT("Opacity")), 0.2f);

	AimBar->SetHiddenInGame(true);
//	HealthBarMaterial = UMaterialInstanceDynamic::Create(HealthBarMatStatic, this);
//	HealthBar->AddElement(HealthBarMaterial, NULL, false, 10.0f, 75.0f, NULL);
//	HealthBar->AddElement(HealthBarBackMatStatic, NULL, false, 10.0f, 75.0f, NULL);
	HealthBarMaterial = UMaterialInstanceDynamic::Create(HealthBarMatStatic, this);
	HealthBar->AddElement(HealthBarMaterial, nullptr, false, 10.0f, 75.0f, nullptr);
	HealthBar->Elements[0].Material = HealthBarMaterial;
	//HealthBar->AddElement(HealthBarBackMatStatic, nullptr, false, 10.0f, 75.0f, nullptr);

	Cast<UMaterialInstanceDynamic>(HealthBarMaterial)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(0.000905f, 1.0f, 0.0f));
	if (PlayerState != nullptr)
	{
		UpdateTeamColor();
	}
}

void ADieselandCharacter::Tick(float DeltaSeconds)
{
	if (this == nullptr){
		return;
	}

    if(StopMusic == true)
    {
        StopMusic = false;
        CharSelectMusic->Stop();
    }

	if (Health <= 0)
	{
		OnHasBeenKilled(LatestDamageCauser);
	}

	// Every frame set the health display
	HealthLabel->SetText(FString::FromInt(Health));
	HealthLabel->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));

	if (HealthBarMaterial){
		HealthPercentage = ((float)Health / (float)MaxHealth);
		Cast<UMaterialInstanceDynamic>(HealthBarMaterial)->SetScalarParameterValue(FName(TEXT("Health percentage")), HealthPercentage);
		if (PlayerState && GetTeamNumber() != CharacterTeam)
		{
			CharacterTeam = GetTeamNumber();
			UpdateTeamColor();
		}
	}

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
	if (SlowRemaining == 0 && StatusEffects.Contains(FString("SmokeScreen"))){

		StatusEffects.Remove(FString("SmokeScreen"));
		ResetCamera();

	}
}

void ADieselandCharacter::SkillOneAim()
{
	AimBar->SetHiddenInGame(false);
}

void ADieselandCharacter::SkillOneAimRelease()
{
	AimBar->SetHiddenInGame(true);
	AimSphere->SetHiddenInGame(true);
}

void ADieselandCharacter::SkillTwoAim()
{
	AimBar->SetHiddenInGame(false);
}

void ADieselandCharacter::SkillTwoAimRelease()
{
	AimBar->SetHiddenInGame(true);
	AimSphere->SetHiddenInGame(true);
}

void ADieselandCharacter::SkillThreeAim()
{
	AimBar->SetHiddenInGame(false);
	}

void ADieselandCharacter::SkillThreeAimRelease()
{
	AimBar->SetHiddenInGame(true);
	AimSphere->SetHiddenInGame(true);
}


void ADieselandCharacter::UpdateTeamColor()
{
	if (HealthBarMaterial != nullptr){

		Cast<UMaterialInstanceDynamic>(HealthBarMaterial)->SetVectorParameterValue(FName(TEXT("TeamColor")), Cast<ADieselandPlayerState>(PlayerState)->GetTeamColor());

		/*switch (Cast<ADieselandPlayerState>(PlayerState)->TeamNumber)
		{
		case 0:
				Cast<UMaterialInstanceDynamic>(HealthBarMaterial)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(0.000905f, 1.0f, 0.0f));
			break;
		case 1:
				Cast<UMaterialInstanceDynamic>(HealthBarMaterial)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(0.035f, 0.005232f, 0.004898f));
			break;
		case 2:
				Cast<UMaterialInstanceDynamic>(HealthBarMaterial)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(0.0f, 0.035871f, 1.0f));
			break;
		case 3:
				Cast<UMaterialInstanceDynamic>(HealthBarMaterial)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(1.0f, 1.0f, 1.0f));
			break;
		case 4:
				Cast<UMaterialInstanceDynamic>(HealthBarMaterial)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(0.0f, 0.828977f, 1.0f));
			break;
		case 5:
				Cast<UMaterialInstanceDynamic>(HealthBarMaterial)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(1.0f, 0.935999f, 0.0f));
			break;
		case 6:
				Cast<UMaterialInstanceDynamic>(HealthBarMaterial)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(0.747108f, 0.0f, 1.0f));
			break;
		case 7:
				Cast<UMaterialInstanceDynamic>(HealthBarMaterial)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(1.0f, 0.305141f, 0.0f));
			break;
		case 8:
				Cast<UMaterialInstanceDynamic>(HealthBarMaterial)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(1.0f, 0.131611f, 0.925403f));
			break;
		default:
				Cast<UMaterialInstanceDynamic>(HealthBarMaterial)->SetVectorParameterValue(FName(TEXT("TeamColor")), FVector(0.000905f, 1.0f, 0.0f));
		}*/
	}
}

// CORE ATTRIBUTE CALCULATION FUNCTION
void ADieselandCharacter::CalculateStats_Implementation()
{
	if (this->ActorHasTag(FName(TEXT("Player"))))
	{
		//This increases the character level for every 3 stat points accrewed
		if (Cast<ADieselandPlayerController>(Controller)->StatPlusCount >= 3){
			CharacterLevel++;
			Cast<ADieselandPlayerState>(PlayerState)->SetCharacterLevel(CharacterLevel);
			Cast<ADieselandPlayerController>(Controller)->StatPlusCount = 0;
		}
		if (IsPoisoned == false){
			//adjustments for health
			MaxHealth = BaseHealth + (Constitution * 20.0f) + (Strength * 3.0f);
			//adjustments for health regeneration
			HealthRegeneration = 4.0f + (Constitution / 8.0f) + (Strength / 16.0f);

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

//Returns bool 
bool ADieselandCharacter::CalculateStats_Validate()
{
	return true;
}


void ADieselandCharacter::ResetCamera_Implementation()
{
	TopDownCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = 0;
	TopDownCameraComponent->PostProcessSettings.bOverride_ColorGradingIntensity = 0;
	TopDownCameraComponent->PostProcessSettings.bOverride_ColorGradingLUT = 0;
	TopDownCameraComponent->PostProcessSettings.bOverride_SceneFringeIntensity = 0;
	TopDownCameraComponent->PostProcessSettings.VignetteIntensity = 0;
	TopDownCameraComponent->PostProcessSettings.bOverride_GrainIntensity = 0;
	TopDownCameraComponent->PostProcessSettings.GrainIntensity = 0;
}

bool ADieselandCharacter::ResetCamera_Validate()
{
	return true;
}
//This function edits the health of the player
void ADieselandCharacter::EditHealth(int32 Amt, AActor* Causer)
{
	if (this != nullptr && Health > 0)
	{
		Health += Amt;

		if (Health <= 0 && !Cast<ADieselandPlayerController>(Controller)->PauseGameInput)
		{
			OnHasBeenKilled(Causer);

			OnRagdollNeeded();
			GetWorldTimerManager().SetTimer(this, &ADieselandCharacter::PostSpawnRagdoll, 3.0f, false);
			this->SetActorHiddenInGame(true);
			Cast<ADieselandPlayerController>(Controller)->PauseGameInput = true;
		}

		if (Causer->ActorHasTag(FName(TEXT("Player"))) /*|| Causer->ActorHasTag(FName(TEXT("KillFloor")))*/)
		{
			LatestDamageCauser = Causer;
		}

		if (Role < ROLE_Authority)
		{
			ServerEditHealth(Amt, Causer);
		}
	}
}

void ADieselandCharacter::ServerEditHealth_Implementation(int32 Amt, AActor* Causer)
{
	EditHealth(Amt, Causer);
}

bool ADieselandCharacter::ServerEditHealth_Validate(int32 Amt, AActor* Causer)
{
	return true;
}

void ADieselandCharacter::OnHasBeenKilled(AActor* Causer)
{
	if (Role == ROLE_Authority)
	{
		if (Causer != nullptr && Causer != nullptr && Causer->ActorHasTag(FName(TEXT("Player"))))
		{
			ADieselandPlayerState* TempPlayerState = Cast<ADieselandPlayerState>((Cast<ADieselandCharacter>(Causer)->PlayerState));
			TempPlayerState->SetKillNum(TempPlayerState->Kills += 1);
			Cast<ADieselandCharacter>(Causer)->Scrap += 250;
			
		}
		//Cast<ADieselandPlayerController>(Controller)->RespawnPawn();
	}
	
}

void ADieselandCharacter::PostSpawnRagdoll()
{
	if (Role == ROLE_Authority)
	{
		Cast<ADieselandPlayerController>(Controller)->RespawnPawn();
		this->SetActorHiddenInGame(false);
		Cast<ADieselandPlayerController>(Controller)->PauseGameInput = false;
	}
}

//function for adjusting speed and health, currently using this for strykers posions, I put the function here so it is extendable to other characters in case
//we want to use it again in the future
void ADieselandCharacter::EditSpeedDamage(int32 Speed, int32 Damage, AActor* Target)
{
	this->IsPoisoned = true;
	this->CharacterMovement->MaxWalkSpeed = this->CharacterMovement->MaxWalkSpeed * (Speed / 100.0f);
	this->BasicAttackDamage = this->BasicAttackDamage * (Damage / 100.0f);
		if (Role < ROLE_Authority)
		{
		ServerEditSpeedDamage(Speed, Damage, Target);
		}
	}

void ADieselandCharacter::ServerEditSpeedDamage_Implementation(int32 Speed, int32 Damage, AActor* Target)
	{
	// Edit the health of the specific pawn
	EditSpeedDamage(Speed, Damage, Target);

	}

bool ADieselandCharacter::ServerEditSpeedDamage_Validate(int32 Speed, int32 Damage, AActor* Target)
	{
	return true;
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
			for (int32 x = 0; x < 1; x++)
			{
               RandomX = FMath::RandRange(-30, 30);
                RandomY = FMath::RandRange(-30, 30);
				AActor* Scrap = UDieselandStaticLibrary::SpawnBlueprint<AActor>(World, ScrapClass, FVector(TempEnemyLoc.X + RandomX, TempEnemyLoc.Y + RandomY, TempEnemyLoc.Z), FRotator(0.0f, 0.0f, 0.0f));
				Cast<AScrap>(Scrap)->ScrapValue = 250;
				//Alternatively used to spawn c++ class
				//AScrap* const Scrap = World->SpawnActor<AScrap>(AScrap::StaticClass(), FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + (70.0f * x)), FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
				// Cast<AScrap>(GetPawn());
			}
		}
	}
}

void ADieselandCharacter::ServerChangeSpeedDamageEnemy_Implementation(int32 Speed, int32 Damage, AActor* Target)
{
	Cast<ADieselandEnemyBot>(Target)->IsPoisoned = true;
	Cast<ADieselandEnemyBot>(Target)->CharacterMovement->MaxWalkSpeed = Cast<ADieselandEnemyBot>(Target)->CharacterMovement->MaxWalkSpeed * (Speed / 100.0f);
	Cast<ADieselandEnemyBot>(Target)->BasicAttackDamage = Cast<ADieselandEnemyBot>(Target)->BasicAttackDamage * (Damage / 100.0f);
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
			Cast<ADieselandCharacter>(CurActor)->EditHealth(-1 * BasicAttackDamage, this);
		}
	}
	OnBasicAttack();
}

void ADieselandCharacter::SkillOne()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		AimBar->SetHiddenInGame(true);
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
	AimBar->SetHiddenInGame(true);
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
	AimBar->SetHiddenInGame(true);
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
			Cast<ADieselandCharacter>(CurActor)->EditHealth(-1 * BasicAttackDamage, this);
		}
	}
}

void ADieselandCharacter::OnBasicAttack_Implementation()
{

}

void ADieselandCharacter::OnSkillOne_Implementation()
{

}

void ADieselandCharacter::OnSkillTwo_Implementation()
{

}

void ADieselandCharacter::OnSkillThree_Implementation()
{

}

//all necessary audio functions
void ADieselandCharacter::Taunt()
{
	TauntSound->Play();
}

void ADieselandCharacter::Laugh()
{
	LaughSound->Play();
}
void ADieselandCharacter::Comment()
{
	CommentSound->Play();
}

int32 ADieselandCharacter::GetTeamNumber()
{
	if (this != nullptr && PlayerState != nullptr)
	{
		return Cast<ADieselandPlayerState>(PlayerState)->GetTeamNum();
	}
	else
	{
		return 0;
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

void ADieselandCharacter::ServerActivateAOEParticle_Implementation(UParticleSystem* Particle)
{
	AOEParticleSystem->SetTemplate(Particle);
	AOEParticleSystem->ActivateSystem();
}

bool ADieselandCharacter::ServerActivateAOEParticle_Validate(UParticleSystem* Particle)
{
	return true;
}

void ADieselandCharacter::MulticastMusicOff_Implementation()
{
    CharSelectMusic->Stop();
}

void ADieselandCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ADieselandCharacter, Health);
	DOREPLIFETIME(ADieselandCharacter, MaxHealth);
	DOREPLIFETIME(ADieselandCharacter, HealthRegeneration);

	DOREPLIFETIME(ADieselandCharacter, CharacterLevel);

	DOREPLIFETIME(ADieselandCharacter, Scrap);
	DOREPLIFETIME(ADieselandCharacter, Kills);

	DOREPLIFETIME(ADieselandCharacter, AimMesh);
	DOREPLIFETIME(ADieselandCharacter, AimRotation);
    
	DOREPLIFETIME(ADieselandCharacter, IsMelee);
    DOREPLIFETIME(ADieselandCharacter, StopMusic);

	DOREPLIFETIME(ADieselandCharacter, MoveSpeed);

	DOREPLIFETIME(ADieselandCharacter, ParticleSystem);

	DOREPLIFETIME(ADieselandCharacter, LatestDamageCauser);

	DOREPLIFETIME(ADieselandCharacter, LingerTimer);
    
    DOREPLIFETIME(ADieselandCharacter, RandomX);
    DOREPLIFETIME(ADieselandCharacter, RandomY);

	DOREPLIFETIME(ADieselandCharacter, BasicAttackTimer);
	DOREPLIFETIME(ADieselandCharacter, BasicAttackReloadTimer);
	DOREPLIFETIME(ADieselandCharacter, SkillOneTimer);
	DOREPLIFETIME(ADieselandCharacter, SkillTwoTimer);
	DOREPLIFETIME(ADieselandCharacter, SkillThreeTimer);
	DOREPLIFETIME(ADieselandCharacter, SkillOneCooldown);
	DOREPLIFETIME(ADieselandCharacter, SkillTwoCooldown);
	DOREPLIFETIME(ADieselandCharacter, SkillThreeCooldown);
	DOREPLIFETIME(ADieselandCharacter, TauntTimer);
	DOREPLIFETIME(ADieselandCharacter, LaughTimer);
	DOREPLIFETIME(ADieselandCharacter, CommentTimer);

	DOREPLIFETIME(ADieselandCharacter, BasicAttackActive);
	DOREPLIFETIME(ADieselandCharacter, BasicAttackDamage);
	DOREPLIFETIME(ADieselandCharacter, BasicAttackReloadSpeed);
	DOREPLIFETIME(ADieselandCharacter, BasicAttackAmmo);

	DOREPLIFETIME(ADieselandCharacter, BaseDamage);
	DOREPLIFETIME(ADieselandCharacter, BaseCooldownSpeed);
	DOREPLIFETIME(ADieselandCharacter, BaseHealth);

	DOREPLIFETIME(ADieselandCharacter, StatusEffects);
	DOREPLIFETIME(ADieselandCharacter, StunRemaining);
	DOREPLIFETIME(ADieselandCharacter, SlowRemaining);

	// Necessary
	DOREPLIFETIME(ADieselandCharacter, Strength);
	DOREPLIFETIME(ADieselandCharacter, Constitution);
	DOREPLIFETIME(ADieselandCharacter, Dexterity);
	DOREPLIFETIME(ADieselandCharacter, Intelligence);

}