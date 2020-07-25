// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "atgArenaHud.generated.h"


UCLASS(config = Game)
class AatgArenaHud : public AHUD
{
	GENERATED_BODY()

public:
	AatgArenaHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};
