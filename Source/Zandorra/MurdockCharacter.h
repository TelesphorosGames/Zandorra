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
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UBoxComponent> ShieldCollisionBox;
	
protected:

	virtual void AbilityButtonPressed() override;
	virtual void AbilityButtonReleased() override;

	
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UShieldAbilityComponent> ShieldAbilityComponent;
	
	
	
		
};
