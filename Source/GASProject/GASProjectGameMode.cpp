// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASProjectGameMode.h"
#include "GASProjectHUD.h"
#include "GASProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGASProjectGameMode::AGASProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AGASProjectHUD::StaticClass();
}
