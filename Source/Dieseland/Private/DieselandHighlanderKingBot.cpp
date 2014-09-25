


#include "Dieseland.h"
#include "UnrealNetwork.h"
#include "DieselandHighlanderKingBot.h"




ADieselandHighlanderKingBot::ADieselandHighlanderKingBot(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

void ADieselandHighlanderKingBot::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	// Replicate to everyone
	DOREPLIFETIME(ADieselandHighlanderKingBot, CannonZoneCollision);
}