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
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
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
	
}

void AZandorraCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AZandorraCharacter::AddDamage);
	SetCanFireDelegate.BindUFunction(CombatComponent, "SetCanFire");
	
}

UBeamAttackComponent* AZandorraCharacter::GetBeamAttackComponent()
{
	return nullptr;
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

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AZandorraCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AZandorraCharacter::AimButtonReleased);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AZandorraCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AZandorraCharacter::FireButtonReleased);

	PlayerInputComponent->BindAction("Beam", IE_Pressed, this, &AZandorraCharacter::AbilityButtonPressed);
	PlayerInputComponent->BindAction("Beam", IE_Released, this, &AZandorraCharacter::AbilityButtonReleased);
}

void AZandorraCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	InterpFOV(DeltaSeconds);
	
	if(CombatComponent)
	{
		CrosshairsTarget=CombatComponent->GetCrosshairsTarget();
	}
	
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

