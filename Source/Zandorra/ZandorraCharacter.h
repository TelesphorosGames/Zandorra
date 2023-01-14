#pragma once

#include "CoreMinimal.h"
#include "InteractWithCrosshairs.h"
#include "GameFramework/Character.h"


#include "ZandorraCharacter.generated.h"


UCLASS(config=Game)
class AZandorraCharacter : public ACharacter, public IInteractWithCrosshairs
{
	GENERATED_BODY()

public:
	
	AZandorraCharacter();

	/* OVERRIDES
	 */
	
	virtual void PostInitializeComponents() override;

	/* PUBLIC FUNCTIONS
	 */
	
	/* PUBLIC VARIABLES
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MoveForwardAxisValue = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bUsingBeamAttack = false;

	
	
	/* GETTERS AND SETTERS
	 */
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE class UAnimMontage* GetAttackMontage() const {return AttackMontage; }
	
protected:

	
	/* OVERRIDES
	 */
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
 	
	/* PROTECTED FUNCTIONS
	 */

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void AimButtonPressed();
	void AimButtonReleased();

	void FireButtonPressed();
	void FireButtonReleased();

	void BeamButtonPressed();
	void BeamButtonReleased();

	void InterpFOV(float DeltaTime);

	
	/* PROTECTED VARIABLES
	 */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere)
	float DefaultFOV = 60.f;
	UPROPERTY(EditAnywhere)
	float ZoomedInFOV = 30.f;
	float CurrentFOV{};
	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed =4.f;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UCombatComponent* CombatComponent;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	class UAnimMontage* AttackMontage;

	bool bAiming = false;


private:
	
	
	
};


