// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BeamAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZANDORRA_API UBeamAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UBeamAttackComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	void StartBeamAttack();
	void BeamAttackFinished();

	
	UPROPERTY()
	class AZandorraCharacter* ZCharacter{};

	UPROPERTY()
	class UCombatComponent* CombatComponent{};

protected:

	virtual void BeginPlay() override;

private:
	
	void StartBeamTimer();
	void BeamTimerFinished();
	
	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit);
	
	FTimerHandle BeamTimer;
		
	UPROPERTY(EditAnywhere)
	float BeamFireDelay =.05f;
	
	UPROPERTY(EditAnywhere)
	float BeamDamageAmount =.5f;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* BeamSystem;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* BeamImpactParticles;
	
	UPROPERTY(EditAnywhere)
	class USoundCue* BeamImpactSound;
	
	bool bHeldFire = false;
	
	UPROPERTY(EditAnywhere)
	float WeaponCharge;
	
	UPROPERTY(EditAnywhere)
	float WeaponChargeMax;

	UFUNCTION(BlueprintGetter)
	float GetWeaponChargePercentage();
	
	// To be multiplied be delta time , amount per second
	UPROPERTY(EditAnywhere)
	float WeaponDrainRate=10.f;
	
	UPROPERTY(EditAnywhere)
	float WeaponProjectileCost=2.f;
		
		
};
