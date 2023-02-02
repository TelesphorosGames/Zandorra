#include "ShieldAbilityComponent.h"

#include "MurdockCharacter.h"
#include "ZandorraCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

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

	ZCharacter = Cast<AMurdockCharacter>(GetOwner());
	
	
}

void UShieldAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UShieldAbilityComponent::StartShield()
{
	Shield = UGameplayStatics::SpawnEmitterAttached(ShieldSystem, ZCharacter->ShieldCollisionBox);
	ZCharacter->bUsingBeamAttack = true;
	
}

void UShieldAbilityComponent::FinishShield()
{
	Shield->Deactivate();
	ZCharacter->bUsingBeamAttack = false;
}

void UShieldAbilityComponent::AdjustShieldCharge(float AmountToAdjustBy)
{
	
}

