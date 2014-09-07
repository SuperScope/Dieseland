

#include "Dieseland.h"
#include "DieselandEnemy.h"
#include "DieselandEnemyController.h"


ADieselandEnemy::ADieselandEnemy(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	AIControllerClass = ADieselandEnemyController::StaticClass();
}


