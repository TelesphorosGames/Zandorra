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

	AMurdockCharacter();

	
protected:

	
private:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UShieldAbilityComponent> ShieldAbilityComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UBoxComponent> ShieldCollisionBox;
	
		
};
