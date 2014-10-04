

#include "Dieseland.h"
#include "EngletonCharacter.h"


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
}


