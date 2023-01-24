// Fill out your copyright notice in the Description page of Project Settings.


#include "DekkerCharacter.h"

#include "BeamAttackComponent.h"

ADekkerCharacter::ADekkerCharacter()
{
	BeamAttackComponent = CreateDefaultSubobject<UBeamAttackComponent>(TEXT("BeamAttackComponent"));
}

UBeamAttackComponent* ADekkerCharacter::GetBeamAttackComponent()
{
	if(BeamAttackComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Override Called!!!!"));
		return BeamAttackComponent;
	}
	return nullptr;
	
}

void ADekkerCharacter::AbilityButtonPressed()
{
	Super::AbilityButtonPressed();
	if(CharacterMovementState == ECharacterMovementState::ECMS_Sprinting || CharacterMovementState == ECharacterMovementState::ECMS_Stunned)
	{
		return;
	}
	
	if(!BeamAttackComponent)
	{
		return;
	}
	
	const bool ExecutedDone = SetCanFireDelegate.ExecuteIfBound(false);
	if(ExecutedDone)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability Button Pressed Called FALSE"));
	}
	BeamAttackComponent->StartBeamAttack();
	
}

void ADekkerCharacter::AbilityButtonReleased()
{
	Super::AbilityButtonReleased();
	if(!BeamAttackComponent)
	{
		return;
	}
	
	const bool ExecutedDone = SetCanFireDelegate.ExecuteIfBound(true);
	if(ExecutedDone)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability Button Pressed Called TRUE"));
	}
	BeamAttackComponent->BeamAttackFinished();
}
