// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZandorraCharacter.h"

#include "CombatComponent.h"
#include "HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Zandorra.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

AZandorraCharacter::AZandorraCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	TurnRateGamepad = 50.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->SetFieldOfView(DefaultFOV);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);

	MaxStamina = 100.f;
	Stamina = MaxStamina;

	DamageableDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DamageableDetectionSphere"));
	
}

void AZandorraCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AZandorraCharacter::AddDamage);
	SetCanFireDelegate.BindUFunction(CombatComponent, "SetCanFire");

	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
	
}

void AZandorraCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AZandorraCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AZandorraCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AZandorraCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AZandorraCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AZandorraCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AZandorraCharacter::TouchStopped);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AZandorraCharacter::SprintButtonPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AZandorraCharacter::SprintButtonReleased);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AZandorraCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AZandorraCharacter::AimButtonReleased);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AZandorraCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AZandorraCharacter::FireButtonReleased);

	PlayerInputComponent->BindAction("Beam", IE_Pressed, this, &AZandorraCharacter::AbilityButtonPressed);
	PlayerInputComponent->BindAction("Beam", IE_Released, this, &AZandorraCharacter::AbilityButtonReleased);

	PlayerInputComponent->BindAction("LockOn", IE_Pressed, this, &AZandorraCharacter::LockOnButtonPressed);
}

void AZandorraCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	InterpFOV(DeltaSeconds);
	
	if(CombatComponent)
	{
		CrosshairsTarget=CombatComponent->GetCrosshairsTarget();
	}

	CharacterMovementTick(DeltaSeconds);
	
}

void AZandorraCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CombatComponent)
	{
		CombatComponent->ZCharacter = this;
	}
	if(HealthComponent)
	{
	
	}
}

void AZandorraCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AZandorraCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

// This is overriden in Dekker Player Character Class and returns Dekker's beam attack component
 UBeamAttackComponent* AZandorraCharacter::GetBeamAttackComponent()
{
	return nullptr;
}

float AZandorraCharacter::GetStaminaPercentage()
{
	return Stamina / MaxStamina;
}


void AZandorraCharacter::CharacterMovementTick(float DeltaSeconds)
{
	
	switch (CharacterMovementState)
	{
	
	case ECharacterMovementState::ECMS_Idle:
		Stamina += StaminaRegenRate  * DeltaSeconds;
		Stamina = FMath::Clamp(Stamina, 0, MaxStamina);
		break;
	case ECharacterMovementState::ECMS_Sprinting:
		if(GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			Stamina -= StaminaDrainRate * DeltaSeconds;
		}
		if(Stamina <= 0.f)
		{
			CharacterMovementState = ECharacterMovementState::ECMS_Idle;
			GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
		}
		break;
	case ECharacterMovementState::ECMS_Stunned:
		
		break;
	case ECharacterMovementState::ECMS_LockedOn:
		SetLockOnCameraRotation(DeltaSeconds);

		break;
		
	case ECharacterMovementState::ECMS_MAX:
		break;
	default: ;
	}
}

void AZandorraCharacter::SprintButtonPressed()
{
	if(GetCharacterMovement())
	{
		CharacterMovementState = ECharacterMovementState::ECMS_Sprinting;
		GetCharacterMovement()->MaxWalkSpeed = SprintMaxWalkSpeed;
	}
}

void AZandorraCharacter::SprintButtonReleased()
{
	if(GetCharacterMovement())
	{
		CharacterMovementState = ECharacterMovementState::ECMS_Idle;
		GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
		CurrentlyLockedOnTarget = nullptr;
	}
}

void AZandorraCharacter::AimButtonPressed()
{
	if (CombatComponent)
	{
		CombatComponent->SetAiming(true);
		bAiming = true;
	}
}

void AZandorraCharacter::AimButtonReleased()
{
	if (CombatComponent)
	{
		CombatComponent->SetAiming(false);
		bAiming = false;
	}
}

void AZandorraCharacter::FireButtonPressed()
{
	if(CharacterMovementState == ECharacterMovementState::ECMS_Sprinting || CharacterMovementState == ECharacterMovementState::ECMS_Stunned)
	{
		return;
	}
	if (CombatComponent)
	{
		CombatComponent->FireWeaponPressed(true);
	}
}

void AZandorraCharacter::FireButtonReleased()
{
	if (CombatComponent)
	{
		CombatComponent->FireWeaponPressed(false);
	}
}

void AZandorraCharacter::AbilityButtonPressed()
{
	if(CharacterMovementState == ECharacterMovementState::ECMS_Sprinting || CharacterMovementState == ECharacterMovementState::ECMS_Stunned)
	{
		return;
	}
	bAbilityButtonHeld = true;
	if (CombatComponent)
	{
		CombatComponent->AbilityButtonPressed(true);
	}
	
}

void AZandorraCharacter::AbilityButtonReleased()
{
	bAbilityButtonHeld = false;
	if (CombatComponent)
	{
		CombatComponent->AbilityButtonPressed(false);
	}
}

bool AZandorraCharacter::LockOffButtonPressed()
{
	if(CharacterMovementState == ECharacterMovementState::ECMS_LockedOn)
	{
		CharacterMovementState = ECharacterMovementState::ECMS_Idle;
		CurrentlyLockedOnTarget = nullptr;
		return true;
	}
	return false;
}

void AZandorraCharacter::LockFirstAvailableTarget()
{
	CurrentlyLockedOnTarget = ActorsWithinLockOnRange[0];
	LockedOnActorIndex = 0;
	CharacterMovementState = ECharacterMovementState::ECMS_LockedOn;
	UE_LOG(LogTemp, Warning, TEXT("Currently Locked Onto : %s"), *CurrentlyLockedOnTarget->GetName());
}

void AZandorraCharacter::LockOnButtonPressed()
{
	// Check to see which actors are overlapping our Damagable Detection Sphere
	DamageableDetectionSphere->GetOverlappingActors(ActorsWithinLockOnRange);
	// None? return
	if(ActorsWithinLockOnRange.Num() == 0)
	{
		return;
	}
	// One? Set our Currently locked target to it
	if(ActorsWithinLockOnRange.Num()==1)
	{
		if(ActorsWithinLockOnRange[0] != CurrentlyLockedOnTarget)
        {
        	LockFirstAvailableTarget();
        }
	}
	if(ActorsWithinLockOnRange.Num()>1)
	{
		if(ActorsWithinLockOnRange[LockedOnActorIndex] == CurrentlyLockedOnTarget)
		{
			LockedOnActorIndex++;
			if(ActorsWithinLockOnRange.Num() == LockedOnActorIndex)
			{
				CurrentlyLockedOnTarget = ActorsWithinLockOnRange[0];
				LockedOnActorIndex = 0;
				CharacterMovementState = ECharacterMovementState::ECMS_LockedOn;
				UE_LOG(LogTemp, Warning, TEXT("Currently Locked Onto : %s"), *CurrentlyLockedOnTarget->GetName());
			}
			CurrentlyLockedOnTarget = ActorsWithinLockOnRange[LockedOnActorIndex];
			
			CharacterMovementState = ECharacterMovementState::ECMS_LockedOn;
			UE_LOG(LogTemp, Warning, TEXT("Currently Locked Onto : %s"), *CurrentlyLockedOnTarget->GetName());
		}
		else if(ActorsWithinLockOnRange[0] != CurrentlyLockedOnTarget)
		{
			CurrentlyLockedOnTarget = ActorsWithinLockOnRange[0];
			LockedOnActorIndex = 0;
			CharacterMovementState = ECharacterMovementState::ECMS_LockedOn;
			UE_LOG(LogTemp, Warning, TEXT("Currently Locked Onto : %s"), *CurrentlyLockedOnTarget->GetName());
		}
	}
	
	
}

void AZandorraCharacter::SetLockOnCameraRotation(float DeltaSeconds)
{
	const FRotator LookAtRotation = (CurrentlyLockedOnTarget->GetActorLocation() - GetActorLocation()).Rotation();
	FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtRotation, DeltaSeconds, 12.f);
	InterpRotation.Pitch = 0.f;
	SetActorRotation(InterpRotation);
		
	const FRotator CurrentCamSpot = GetControlRotation();
	const FRotator CameraLookAtRotation = UKismetMathLibrary::FindLookAtRotation(FollowCamera->GetComponentLocation(), CurrentlyLockedOnTarget->GetActorLocation());
	const FRotator InterpToCamSpot = FMath::RInterpTo(CurrentCamSpot, CameraLookAtRotation, DeltaSeconds, 12.f);
	Controller->SetControlRotation(InterpToCamSpot);

	UE_LOG(LogTemp, Warning, TEXT("WTF?"));
}

void AZandorraCharacter::InterpFOV(float DeltaTime)
{
	if (bAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, ZoomedInFOV, DeltaTime, ZoomInterpSpeed);
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);
	}
	if (FollowCamera)
	{
		FollowCamera->SetFieldOfView(CurrentFOV);
	}
}

void AZandorraCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AZandorraCharacter::LookUpAtRate(float Rate)
{

	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());

}

void AZandorraCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
	MoveForwardAxisValue = Value;
}

void AZandorraCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void AZandorraCharacter::TakeCharacterDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	
}

void AZandorraCharacter::AddDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if(HealthComponent)
	{
		HealthComponent->AdjustHealth(-Damage);
	}
}

