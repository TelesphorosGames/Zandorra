// Fill out your copyright notice in the Description page of Project Settings.


#include "MurdockCharacter.h"

#include "ShieldAbilityComponent.h"
#include "Components/BoxComponent.h"

AMurdockCharacter::AMurdockCharacter()
{
	ShieldAbilityComponent = CreateDefaultSubobject<UShieldAbilityComponent>(TEXT("ShieldAbilityComponent"));

	ShieldCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldCollisionBox"));
	ShieldCollisionBox->SetupAttachment(GetMesh(), "ShieldSocket");
	ShieldCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void AMurdockCharacter::AbilityButtonPressed()
{
	Super::AbilityButtonPressed();

	ShieldAbilityComponent->StartShield();
	ShieldCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
}

void AMurdockCharacter::AbilityButtonReleased()
{

	Super::AbilityButtonReleased();

	ShieldAbilityComponent->FinishShield();
	ShieldCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}
