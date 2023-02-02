// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZandorraCharacter.h"
#include "DekkerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ZANDORRA_API ADekkerCharacter : public AZandorraCharacter
{
	GENERATED_BODY()

public:

	ADekkerCharacter();

	virtual class UBeamAttackComponent* GetBeamAttackComponent() override;
protected:

	virtual void AbilityButtonPressed() override;
	virtual void AbilityButtonReleased() override;
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UBeamAttackComponent* BeamAttackComponent;
	
};
