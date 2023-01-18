#include "HealthComponent.h"
#include "ZandorraCharacter.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UHealthComponent::AdjustHealth(float Adjustment)
{
	Health += Adjustment;
	if(Health <0)
	{
		Health = FMath::Clamp(0, Health, Health);
	}
	UE_LOG(LogTemp, Warning, TEXT("Health : %f"), Health);
}

const float UHealthComponent::GetHealth()
{
	return Health;
}

