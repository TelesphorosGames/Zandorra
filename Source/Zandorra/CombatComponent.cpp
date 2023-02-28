// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

#include "BeamAttackComponent.h"
#include "InteractWithCrosshairs.h"
#include "NiagaraFunctionLibrary.h"
#include "Projectile.h"
#include "ZandorraCharacter.h"
#include "ZandorraHud.h"
#include "ZandorraPlayerController.h"
#include "Animation/AnimInstance.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Zandorra.h"
#include "ZandorraGameMode.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::SetCanFire(bool bCanFireSetTo)
{
	bCanFire = bCanFireSetTo; 
	if(bCanFireSetTo)
	{
		UE_LOG(LogTemp, Warning, TEXT("Called from Combat side yes"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Called from Combat side no"));
	}
	
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ZCharacter == nullptr)
	{
		ZCharacter = Cast<AZandorraCharacter>(GetOwner());
	}
	if (ZCharacter)
	{
		AZandorraGameMode* ZandorraGameMode = Cast<AZandorraGameMode>(GetWorld()->GetAuthGameMode());
		if(ZandorraGameMode)
		{
			ZPlayerController = Cast<AZandorraPlayerController>(ZandorraGameMode->GetPlayableController());
            		if(ZPlayerController)
            		{
            			ZHud = Cast<AZandorraHud>(ZPlayerController->GetHUD());
            			UE_LOG(LogTemp, Warning, TEXT("%s"), *ZPlayerController->GetName());
            		}
		}
		
	}
	const USkeletalMeshSocket* BarrelSocket = ZCharacter->GetMesh()->GetSocketByName("Barrel");
	if(BarrelSocket)
	{
		SocketTransform = BarrelSocket->GetSocketTransform(ZCharacter->GetMesh());
	}
	
            
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SetHudCrosshairs(DeltaTime);
	FHitResult HitResult;
	TraceUnderCrosshairs(HitResult);
		
}

void UCombatComponent::SetHudCrosshairs(float DeltaTime)
{
	if (ZCharacter == nullptr || ZCharacter->Controller == nullptr || ZHud == nullptr) return;

	const FVector2D WalkSpeedRange = {0.f, ZCharacter->GetCharacterMovement()->MaxWalkSpeed};
	const FVector2D VelocityMultiplierRange(0.f, 1.f);
	FVector Velocity = ZCharacter->GetVelocity();
	Velocity.Z = 0.f;

	CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange,
	                                                            Velocity.Size());

	if (ZCharacter->GetCharacterMovement()->IsFalling())
	{
		CrosshairFallingFactor = FMath::FInterpTo(CrosshairFallingFactor, 2.25f, DeltaTime, 2.25f);
	}
	else
	{
		CrosshairFallingFactor = FMath::FInterpTo(CrosshairFallingFactor, 0.f, DeltaTime, 30.f);
	}
	if(CrosshairMovingFactor <= 0.1f && CrosshairVelocityFactor <= 0.1f )
	{
		if(bIsAiming)
		{
			CrosshairAimingFactor = FMath::FInterpTo(CrosshairAimingFactor, 2.f, DeltaTime, .5f);
		}
		else
		{
			CrosshairAimingFactor = FMath::FInterpTo(CrosshairAimingFactor, 0, DeltaTime, 30.f);
		}
		
	}

	if (ZCharacter->GetCharacterMovement()->IsCrouching())
	{
		CrosshairCrouchingFactor = FMath::FInterpTo(CrosshairCrouchingFactor, 1.f, DeltaTime, 2.5f);
	}
	else
	{
		CrosshairCrouchingFactor = FMath::FInterpTo(CrosshairCrouchingFactor, 0.f, DeltaTime, 2.5f);
	}

	CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 2.f);

	ZHud->SetCrosshairSpread((CrosshairVelocityFactor + CrosshairFallingFactor) - (CrosshairTargetFactor +
		CrosshairCrouchingFactor + CrosshairAimingFactor + CrosshairShootingFactor));
}

bool UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
    if(!ZCharacter) return false;
    
	if(!ZPlayerController)
	{
		AZandorraGameMode* ZandorraGameMode = Cast<AZandorraGameMode>(GetWorld()->GetAuthGameMode());
		if(ZandorraGameMode)
		{
			ZPlayerController = Cast<AZandorraPlayerController>(ZandorraGameMode->GetPlayableController());
		}
	    
	}
	 
	 
	
	FVector2D ViewportSize;

	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	const FVector2D CrosshairLocation = {ViewportSize.X / 2.f , ViewportSize.Y/2.f };
	FVector CrosshairWorldDirection;

	const bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		CrosshairLocation,
		CrosshairWorldPostition,
		CrosshairWorldDirection
	);

	if(bScreenToWorld)
	{
		FVector Start = CrosshairWorldPostition;

		if(ZCharacter)
		{
			const float DistanceToCharacter = (ZCharacter->GetActorLocation() - Start).Size();

			// Ensure that the line trace starts a bit in front of character to amount for wacky ik results
			Start+=CrosshairWorldDirection * (DistanceToCharacter + 20.f);
			// DrawDebugSphere(GetWorld(),Start, 16.f, 12, FColor::Red);
		}
		
		const FVector End = Start + CrosshairWorldDirection * 5'000;

	
		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECC_Visibility
		
			);

		if(TraceHitResult.GetActor() && TraceHitResult.GetActor()->Implements<UInteractWithCrosshairs>() && TraceHitResult.GetActor() != ZCharacter)
		{
			if(ZHud == nullptr)
			{
				ZHud = Cast<AZandorraHud>(ZPlayerController->GetHUD());
			}
			if(ZHud)
			{
				ZHud->SetCrosshairsColor(FLinearColor::Red);
			}
			CrosshairTargetFactor = .5f;
		}
		else
		{
			if(ZHud == nullptr)
			{
				ZHud = Cast<AZandorraHud>(ZPlayerController->GetHUD());
			}
			if(ZHud)
			{
				ZHud->SetCrosshairsColor(FLinearColor::White);
			}
			CrosshairTargetFactor = 0.f;
		}

		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
			CrosshairsTarget = End;
			return true;
		}
		if(TraceHitResult.GetActor() != ZCharacter)
		{
			CrosshairsTarget = TraceHitResult.ImpactPoint;
			return true;
		}
		CrosshairsTarget =TraceHitResult.TraceEnd;
		return true;
	}
	UE_LOG(LogTemp,Warning,TEXT("BScreenToWorld FaileD!"));
	return false;
}


void UCombatComponent::SetBackwardsMovementTarget()
{
	if(ZCharacter->GetVelocity().Size() > 0 && ZCharacter->MoveForwardAxisValue <= -.3f && ZCharacter->GetCharacterMovementState() != ECharacterMovementState::ECMS_LockedOn)
	{
		const USkeletalMeshSocket* BarrelSocket = ZCharacter->GetMesh()->GetSocketByName("Barrel");
		if(BarrelSocket)
		{
			SocketTransform = BarrelSocket->GetSocketTransform(ZCharacter->GetMesh());
			const FVector BarrelEnd = SocketTransform.GetLocation() + (SocketTransform.GetUnitAxis(EAxis::X) *5000);
			CrosshairsTarget = BarrelEnd;
		}
	}
}

void UCombatComponent::FireWeaponPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
	
	if(CanFireWeapon() && bFireButtonPressed)
	{
		bCanFire = false;
		StartFireTimer();
		
		SetBackwardsMovementTarget();
		
		LaunchProjectile(CrosshairsTarget);
		
	}
}

void UCombatComponent::AbilityButtonPressed(bool bPressed)
{
	// SetBackwardsMovementTarget();
}

void UCombatComponent::LaunchProjectile(const FVector& HitTarget)
{
	if(ZCharacter)
	{
		UWorld* World = GetWorld();

		
		UAnimInstance* AnimInstance = ZCharacter->GetMesh()->GetAnimInstance();
        if(ZCharacter->GetAttackMontage() && AnimInstance)
        {
        	AnimInstance->Montage_Play(ZCharacter->GetAttackMontage());
        	AnimInstance->Montage_JumpToSection("Fast");
        }
		
		const USkeletalMeshSocket* BarrelSocket = ZCharacter->GetMesh()->GetSocketByName("Barrel");
		if(BarrelSocket && World)
		{
			SocketTransform = BarrelSocket->GetSocketTransform(ZCharacter->GetMesh());
            
			// Muzzle socket to hit location from Trace Under Crosshairs
			const FVector ToTarget = HitTarget - SocketTransform.GetLocation();
			const FRotator TargetRotation = ToTarget.Rotation();
            
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
            
			World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(ZCharacter->FiringCameraShake);
		}
	}
}

bool UCombatComponent::CanFireWeapon()
{
	return bCanFire;
}

void UCombatComponent::StartFireTimer()
{
	if(ZCharacter)
	{
		ZCharacter->GetWorldTimerManager().SetTimer(FireWeaponTimer, this, &UCombatComponent::FireTimerFinished, WeaponFireDelay);
	}
}

void UCombatComponent::FireTimerFinished()
{
	if(!ZCharacter->bUsingBeamAttack)
	{
		bCanFire = true;
	}
	if(bFireButtonPressed && bCanFire)
	{
		bCanFire=false;
		SetBackwardsMovementTarget();
		LaunchProjectile(CrosshairsTarget);
		StartFireTimer();
	}
}

