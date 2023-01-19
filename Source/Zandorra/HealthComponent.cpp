#include "HealthComponent.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MaxHealth = 100.f;
	Health = MaxHealth;
	
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
		Health = FMath::Clamp(Health, 0, MaxHealth);
	}
	UE_LOG(LogTemp, Warning, TEXT("Health : %f"), Health);
}

float UHealthComponent::GetHealthPercentage() const
{
	return Health / MaxHealth;
}

