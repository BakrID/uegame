// Copyright Epic Games, Inc. All Rights Reserved.

#include "atgArenaGameMode.h"
#include "atgArenaPawn.h"
#include "atgArenaHud.h"

AatgArenaGameMode::AatgArenaGameMode()
{
	DefaultPawnClass = AatgArenaPawn::StaticClass();
	HUDClass = AatgArenaHud::StaticClass();
}
