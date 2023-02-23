// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZandorraGameMode.h"

#include "FriendlyAIController.h"
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

void AZandorraGameMode::SwitchControlledCharacters()
{
	PlayableController->UnPossess();
	FriendlyAIController->UnPossess();
	if(bWasControllingCharacterZero)
	{
		{
			PlayableController->Possess(PossessableCharacters[1]);
			bWasControllingCharacterZero = false;
			FriendlyAIController->Possess(PossessableCharacters[0]);
		}
	}
	else
	{
		{
			PlayableController->Possess(PossessableCharacters[0]);
			bWasControllingCharacterZero = true;
			FriendlyAIController->Possess(PossessableCharacters[1]);
		}
	}
}
