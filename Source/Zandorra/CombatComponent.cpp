// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

#include "InteractWithCrosshairs.h"
#include "ZandorraCharacter.h"
#include "ZandorraHud.h"
#include "ZandorraPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Zandorra.h"


UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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
		ZPlayerController = Cast<AZandorraPlayerController>(ZCharacter->GetLocalViewingPlayerController());
		if(ZPlayerController)
		{
			ZHud = Cast<AZandorraHud>(ZPlayerController->GetHUD());
			UE_LOG(LogTemp, Warning, TEXT("%s"), *ZPlayerController->GetName());
		}
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

void UCombatComponent::FireWeapon()
{
	
}

void UCombatComponent::SetAiming(bool bAimSetTo)
{
	bIsAiming = bAimSetTo;
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

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	if(!ZPlayerController || !ZCharacter) return;
	
	FVector2D ViewportSize;

	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	const FVector2D CrosshairLocation = {ViewportSize.X / 2.f , ViewportSize.Y/2.f };
	FVector CrosshairWorldPostition;
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
			Start+=CrosshairWorldDirection * (DistanceToCharacter + 30.f);
			// DrawDebugSphere(GetWorld(),Start, 16.f, 12, FColor::Red);
		}
		
		const FVector End = Start + CrosshairWorldDirection * 5'000;

	
		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECC_Visibility
		
			);

		if(TraceHitResult.GetActor() && TraceHitResult.GetActor()->Implements<UInteractWithCrosshairs>())
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
			
		}
		else
		{
		
		}
	}
	else
	{
		
	}	
}

