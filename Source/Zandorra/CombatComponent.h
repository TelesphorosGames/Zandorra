// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZANDORRA_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	
	/* OVERRIDES
	 */
	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* PUBLIC FUNCTIONS
	 */
	void FireWeapon();
	
	/* PUBLIC VARIABLES
	 */
	UPROPERTY()
	class AZandorraCharacter* ZCharacter{};
		
	/* GETTERS AND SETTERS
	 */
	
	FORCEINLINE	void SetAiming(bool bAimSetTo);

		
protected:

	/* OVERRIDES
	 */
	virtual void BeginPlay() override;

	/* PROTECTED FUNCTIONS
	 */
	
	/* PROTECTED VARIABLES
	 */
	UPROPERTY()
	class AZandorraPlayerController* ZPlayerController{};
	
	UPROPERTY()
	class AZandorraHud* ZHud{};

	void SetHudCrosshairs(float DeltaTime);

	bool bIsAiming;
	
private:

	/* OVERRIDES
		 */

	/* PRIVATE FUNCTIONS
	 */
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	/* PRIVATE VARIABLES
	 */
	float CrosshairVelocityFactor{};
	float CrosshairAimingFactor{};
	float CrosshairFallingFactor{};
	float CrosshairCrouchingFactor{};
	float CrosshairShootingFactor{};
	float CrosshairMovingFactor{};
	float CrosshairTargetFactor{};

	FLinearColor CrosshairsColor;

	

	


		
};
