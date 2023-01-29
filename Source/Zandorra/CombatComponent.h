// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


class AProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZANDORRA_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

		UCombatComponent();
	
	/* OVERRIDES
	 */
	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* PUBLIC FUNCTIONS
	 */
	
	bool TraceUnderCrosshairs(FHitResult& TraceHitResult);
	
	void FireWeaponPressed(bool bPressed);
	
	void AbilityButtonPressed(bool bPressed);
	
	void SetBackwardsMovementTarget();


	
	/* PUBLIC VARIABLES
	 */
	UPROPERTY()
	class AZandorraCharacter* ZCharacter{};
	
	// Used to determine which direction the barrel of the weapon should face, depending on which direction character is facing
	FTransform SocketTransform;

	/* GETTERS AND SETTERS
	 */

	FORCEINLINE FVector GetCrosshairsTarget() const {return CrosshairsTarget; }
	
	FORCEINLINE	void SetAiming(bool bAimSetTo)	{bIsAiming = bAimSetTo; }

	UFUNCTION()
	void SetCanFire(bool bCanFireSetTo); 

	
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

	/* PRIVATE FUNCTIONS
	 */

	void LaunchProjectile(const FVector& HitTarget);
	bool CanFireWeapon();
	void StartFireTimer();

	void FireTimerFinished();
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FVector CrosshairsTarget;
	
	bool bFireButtonPressed = false;
	
	UPROPERTY()
	bool bCanFire = true;
	
	FTimerHandle FireWeaponTimer;

	UPROPERTY(EditAnywhere)
	float WeaponFireDelay = 4.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;

};
