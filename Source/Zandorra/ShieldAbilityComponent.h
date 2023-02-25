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

	bool GetShieldActive() const {return bShieldActive;}

	UPROPERTY()
	TObjectPtr<class AMurdockCharacter> ZCharacter{};
	
	UFUNCTION(BlueprintGetter)
	float GetShieldChargePercentage();

	float GetShieldCharge() const {return ShieldCharge; }

	UFUNCTION()
	void AdjustShieldCharge(float AmountToAdjustBy, float DeltaTime);
	

protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	UParticleSystem* ShieldSystem;

	UPROPERTY()
	UParticleSystemComponent* Shield;
	
	UPROPERTY(EditAnywhere)
	float ShieldCharge;
	
	UPROPERTY(EditAnywhere)
	float ShieldChargeMax;
	
	UPROPERTY(EditAnywhere)
	float ShieldDrainRate=10.f;

	UPROPERTY(VisibleAnywhere)
	bool bShieldActive = false;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundCue> StartShieldSound;

	TObjectPtr<class UAudioComponent> ShieldSoundComponent;
	
	

		
};
