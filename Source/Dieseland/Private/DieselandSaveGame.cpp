

#include "Dieseland.h"
#include "DieselandSaveGame.h"


UDieselandSaveGame::UDieselandSaveGame(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	LastCharacter = TEXT("BasicCharacter");

	SaveSlotName = TEXT("SaveSlot0");
	UserIndex = 0;
}
