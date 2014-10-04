

#include "Dieseland.h"
#include "EngletonCharacter.h"
#include "EngletonCrazyLaser.h"


AEngletonCrazyLaser::AEngletonCrazyLaser(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//Engelton crazy laser moves very quickly and deals lots of deeps
	ProjectileMovement->InitialSpeed = 1300.0f;
	InitialLifeSpan = 1.5f;
}


