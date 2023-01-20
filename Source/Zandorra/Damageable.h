// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZANDORRA_API IDamageable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void AddDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser);
	
};
