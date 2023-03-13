// Copyright Epic Games, Inc. All Rights Reserved.

#include "PhysicsTestGameMode.h"
#include "PhysicsTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

APhysicsTestGameMode::APhysicsTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
