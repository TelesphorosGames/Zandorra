// Fill out your copyright notice in the Description page of Project Settings.


#include "MurdockCharacter.h"

#include "ShieldAbilityComponent.h"
#include "Components/BoxComponent.h"

AMurdockCharacter::AMurdockCharacter()
{
	ShieldAbilityComponent = CreateDefaultSubobject<UShieldAbilityComponent>(TEXT("ShieldAbilityComponent"));

	ShieldCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldCollisionBox"));

}
