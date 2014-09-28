

#include "Dieseland.h"
#include "DieselandHighlanderKingBot.h"
#include "DieselandHighlanderKingAI.h"


ADieselandHighlanderKingAI::ADieselandHighlanderKingAI(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

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

