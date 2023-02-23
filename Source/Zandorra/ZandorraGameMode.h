// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZandorraGameMode.generated.h"

class AFriendlyAIController;
UCLASS(minimalapi)
class AZandorraGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AZandorraGameMode();

	TArray<class AZandorraCharacter*> PossessableCharacters;

	FORCEINLINE void SetPlayerController(AController* InController) {PlayableController = InController; }
	FORCEINLINE void SetAIController(AFriendlyAIController* InAIController) {FriendlyAIController = InAIController; }

	FORCEINLINE AController* GetPlayableController() const {return PlayableController; }
	void SwitchControlledCharacters();

private:
	
	TObjectPtr<AController> PlayableController;

	TObjectPtr<class AFriendlyAIController> FriendlyAIController;

	bool bWasControllingCharacterZero = true;
	
};



