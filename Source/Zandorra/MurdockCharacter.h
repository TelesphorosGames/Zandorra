// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZandorraCharacter.h"
#include "MurdockCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ZANDORRA_API AMurdockCharacter : public AZandorraCharacter
{
	GENERATED_BODY()

public:

protected:

	
private:

	UPROPERTY()
	class UShieldAbilityComponent* ShieldAbilityComponent;
	
};
