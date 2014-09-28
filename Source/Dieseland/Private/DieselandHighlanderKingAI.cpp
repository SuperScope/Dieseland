

#include "Dieseland.h"
#include "UnrealNetwork.h"
#include "DieselandHighlanderKingBot.h"
#include "Engine.h"
#include "DieselandHighlanderKingAI.h"


ADieselandHighlanderKingAI::ADieselandHighlanderKingAI(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{


	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("HighlanderKing"));

}
bool ADieselandHighlanderKingAI::ServerCannonAttack_Validate()
{
	return true;
}

void ADieselandHighlanderKingAI::ServerCannonAttack_Implementation()
{
	ADieselandHighlanderKingBot* DieselandPawn = Cast<ADieselandHighlanderKingBot>(GetPawn());
	DieselandPawn->CannonAttack();
}


void ADieselandHighlanderKingAI::UpdateCooldownTimers(float DeltaSeconds)
{
	if (Cast<ADieselandHighlanderKingBot>(GetPawn()) != nullptr){
		ADieselandHighlanderKingBot* DieselandPawn = Cast<ADieselandHighlanderKingBot>(GetPawn());
		// Update the attack pattern timer for the AI
		if (DieselandPawn->AttackPatternTimer > 0.0f){
			DieselandPawn->AttackPatternTimer -= DeltaSeconds;
		}
		//here I reset the attack pattern timer
		else{
			DieselandPawn->AttackPatternTimer = 20.0f;
		}
		
		//here I check to see if the boss has  entered its CannonFiringPhase
		if (DieselandPawn->AttackPatternTimer > 5.0f && DieselandPawn->AttackPatternTimer <= 10.0f){
			DieselandPawn->IsFiringCannons = true;
			DieselandPawn->CharacterMovement->MaxWalkSpeed = 1;
			DieselandPawn->CharacterMovement->RotationRate = FRotator(0.0f, 40.0f, 0.0f);
			if (DieselandPawn->CannonDamageUpdate > 0.0f)
			{
				DieselandPawn->CannonDamageUpdate -= DeltaSeconds;

				//here I actually implement damage when the cannon damage update timer reaches 0    
				if (DieselandPawn->CannonDamageUpdate < 0.0f)
				{
					ServerCannonAttack();
					DieselandPawn->CannonDamageUpdate = 0.2f;
				}
			}	
		}
		// end of attack patern check for check to see if the cannon is being fired
		//here I set the isfiringcannons to false for the highlander king
		else if (DieselandPawn->AttackPatternTimer > 4.0f && DieselandPawn->AttackPatternTimer <= 5.0f){
			DieselandPawn->CharacterMovement->MaxWalkSpeed = 400;
			DieselandPawn->CharacterMovement->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
			DieselandPawn->IsFiringCannons = false;
		}
	}//end of cast if statement

}

void ADieselandHighlanderKingAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateCooldownTimers(DeltaTime);
}
