// Fill out your copyright notice in the Description page of Project Settings.


#include "MurdockCharacter.h"

#include "HealthComponent.h"
#include "ShieldAbilityComponent.h"
#include "Components/BoxComponent.h"

AMurdockCharacter::AMurdockCharacter()
{
	ShieldAbilityComponent = CreateDefaultSubobject<UShieldAbilityComponent>(TEXT("ShieldAbilityComponent"));

	ShieldCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldCollisionBox"));
	ShieldCollisionBox->SetupAttachment(GetMesh(), "ShieldSocket");
	ShieldCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void AMurdockCharacter::AddDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	float AmountToDamage = Damage;
	float RemainingDamage = 0.f;
	
	if(ShieldAbilityComponent->GetShieldActive())
	{
		float CurrentCharge = ShieldAbilityComponent->GetShieldCharge();
		if(AmountToDamage<CurrentCharge)
		{
			ShieldAbilityComponent->AdjustShieldCharge(AmountToDamage, 1);
		}
		else
		{
			RemainingDamage = AmountToDamage - CurrentCharge;
			ShieldAbilityComponent->AdjustShieldCharge(CurrentCharge, 1);
			if(HealthComponent)
			{
				HealthComponent->AdjustHealth(-RemainingDamage);
			}
		}
	}
	else
	{
		if(HealthComponent)
		{
			HealthComponent->AdjustHealth(-AmountToDamage);
		}
	}
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



