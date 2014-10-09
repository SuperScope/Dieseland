

#include "Dieseland.h"
#include "StrykerCharacter.h"
#include "DieselandPlayerController.h"
#include "StrykerPoisionProjectile.h"


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

	//base cooldowns
	BaseSkillOneCooldown = 1.0f;
	BaseSkillTwoCooldown = 1.0f;
	BaseSkillThreeCooldown = 1.0f;

	//adjustments for cooldowns
	SkillOneCooldown = BaseSkillOneCooldown / (1 + Intelligence / 100);
	SkillTwoCooldown = BaseSkillTwoCooldown / (1 + Intelligence / 100);
	SkillThreeCooldown = BaseSkillThreeCooldown / (1 + Intelligence / 100);

	//here I set melee to false so that Stryker only uses ranged attacks
	IsMelee = true;
}


// Stryker Assasinate
void AStrykerCharacter::SkillOne()
{

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