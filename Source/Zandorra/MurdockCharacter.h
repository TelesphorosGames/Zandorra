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

	virtual void AddDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) override;

	virtual void BeginPlay() override;
	
protected:

	virtual void AbilityButtonPressed() override;
	virtual void AbilityButtonReleased() override;

	virtual void ToggleCharacter() override;

	
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UShieldAbilityComponent> ShieldAbilityComponent;
	
	
	
		
};
