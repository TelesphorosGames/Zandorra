// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZandorraGameMode.h"
#include "ZandorraCharacter.h"
#include "UObject/ConstructorHelpers.h"

AZandorraGameMode::AZandorraGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
}
