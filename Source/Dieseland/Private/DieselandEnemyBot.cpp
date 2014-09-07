

#include "Dieseland.h"
#include "DieselandEnemyBot.h"
#include "DieselandEnemyAI.h"


ADieselandEnemyBot::ADieselandEnemyBot(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	AIControllerClass = ADieselandEnemyAI::StaticClass();

	
}