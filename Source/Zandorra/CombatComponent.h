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

	void SetBackwardsMovementTarget();

	void LaunchProjectile(const FVector& HitTarget);
	bool CanFireWeapon();
	void StartFireTimer();
	void FireTimerFinished();
	void StartBeamAttack();
	void BeamAttackFinished();

	
	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit);


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

	bool bFireButtonPressed = false;

	bool bCanFire = true;

	bool bHeldFire = false;

	FTimerHandle FireWeaponTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FVector CrosshairsTarget;

	
	UPROPERTY(EditAnywhere)
	float WeaponFireDelay = 4.f;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* BeamSystem;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* BeamImpactParticles;
	
	UPROPERTY(EditAnywhere)
	class USoundCue* BeamImpactSound;

	FTransform SocketTransform;

	
            
	
	
		
};
