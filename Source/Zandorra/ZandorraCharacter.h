#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "InteractWithCrosshairs.h"
#include "GameFramework/Character.h"


#include "ZandorraCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FSetCanFireDelegate, bool);

UENUM(BlueprintType)
enum class ECharacterMovementState : uint8
{
	ECMS_Idle UMETA(DisplayName = "Idle"),
	ECMS_Sprinting UMETA(DisplayName = "Sprinting"),
	ECMS_Stunned UMETA(DisplayName = "Stunned"),

	ECMS_MAX UMETA(DisplayName= "DefaultMax")
};


UCLASS(config=Game)
class AZandorraCharacter : public ACharacter, public IInteractWithCrosshairs, public IDamageable
{
	GENERATED_BODY()

public:
	
	AZandorraCharacter();

	/* OVERRIDES
	 */
	
	virtual void PostInitializeComponents() override;
	
	virtual void AddDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) override;
	
	/* PUBLIC FUNCTIONS
	 */

	void TakeCharacterDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	
	/* PUBLIC VARIABLES
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MoveForwardAxisValue = 0.f;
	UPROPERTY()
	FVector CrosshairsTarget{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bUsingBeamAttack = false;
	UPROPERTY()
	bool bAbilityButtonHeld = false;
		
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UHealthComponent* HealthComponent;
	
	FSetCanFireDelegate SetCanFireDelegate;
	
	/* GETTERS AND SETTERS
	 */
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UAnimMontage* GetAttackMontage() const {return AttackMontage; }
	FORCEINLINE class UCombatComponent* GetCombatComponent() const {return CombatComponent; }
	FORCEINLINE FVector GetCrosshairsTarget() const {return CrosshairsTarget; }

	virtual class UBeamAttackComponent* GetBeamAttackComponent();

	UFUNCTION(BlueprintGetter)
	float GetStaminaPercentage();
	
protected:

	
	/* OVERRIDES
	 */
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetStaminaDrainRate(float DeltaSeconds);
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

	void SprintButtonPressed();
	void SprintButtonReleased();
	
	
	void AimButtonPressed();
	void AimButtonReleased();

	void FireButtonPressed();
	void FireButtonReleased();

	

	virtual void AbilityButtonPressed();
	virtual void AbilityButtonReleased();

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
	UPROPERTY(EditAnywhere)
	float DefaultMaxWalkSpeed =450.f;
	UPROPERTY(EditAnywhere)
	float AdjustedMaxWalkSpeed = 200.f;
	UPROPERTY(EditAnywhere)
	float SprintMaxWalkSpeed =600.f;
	UPROPERTY(EditAnywhere)
	float Stamina;

	UPROPERTY(EditAnywhere)
	float MaxStamina;

	UPROPERTY(EditAnywhere)
	float StaminaDrainRate = 10.f;
	UPROPERTY(EditAnywhere)
	float StaminaRegenRate = 8.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	ECharacterMovementState CharacterMovementState = ECharacterMovementState::ECMS_Idle;
	
private:

	
	
};



