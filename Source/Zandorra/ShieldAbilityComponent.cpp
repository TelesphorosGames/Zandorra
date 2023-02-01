#include "ShieldAbilityComponent.h"

#include "ZandorraCharacter.h"
#include "Components/BoxComponent.h"

UShieldAbilityComponent::UShieldAbilityComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	
}


float UShieldAbilityComponent::GetShieldChargePercentage()
{
	return ShieldCharge / ShieldChargeMax;
}

void UShieldAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void UShieldAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UShieldAbilityComponent::StartShield()
{
	
}

void UShieldAbilityComponent::FinishShield()
{
	
}

void UShieldAbilityComponent::AdjustShieldCharge(float AmountToAdjustBy)
{
	
}

