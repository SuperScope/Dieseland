

#include "Dieseland.h"
#include "DieselandSaveGame.h"


UDieselandSaveGame::UDieselandSaveGame(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	Username = TEXT("Player");

	SaveSlotName = TEXT("SaveSlot0");
	UserIndex = 0;
}
