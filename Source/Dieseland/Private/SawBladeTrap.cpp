

#include "Dieseland.h"
#include "SawBladeTrap.h"


ASawBladeTrap::ASawBladeTrap(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//base Damage done
	damageGiven = 50.f;
}

/*void ASawBladeTrap::OnActivateTrap_Implementation()
{
	//Call Parent Implementation
	Super::OnActivateTrap_Implementation();
	//Detroy trap (Added for following tutorial. MUST BE REMOVED) 
	Destroy();
}*/
