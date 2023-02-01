// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShieldAbilityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZANDORRA_API UShieldAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UShieldAbilityComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void StartShield();
	void FinishShield();

	UFUNCTION()
	void AdjustShieldCharge(float AmountToAdjustBy);
	
	UPROPERTY()
	class AZandorraCharacter* ZCharacter{};
	
	UFUNCTION(BlueprintGetter)
	float GetShieldChargePercentage();

protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* ShieldSystemComponent;
	
	UPROPERTY(EditAnywhere)
	float ShieldCharge;
	
	UPROPERTY(EditAnywhere)
	float ShieldChargeMax;
	
	UPROPERTY(EditAnywhere)
	float ShieldDrainRate=10.f;


		
};
