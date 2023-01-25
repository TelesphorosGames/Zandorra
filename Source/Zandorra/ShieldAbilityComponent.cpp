#include "ShieldAbilityComponent.h"

UShieldAbilityComponent::UShieldAbilityComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}


void UShieldAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UShieldAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

