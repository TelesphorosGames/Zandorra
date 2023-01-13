// Fill out your copyright notice in the Description page of Project Settings.


#include "ZandorraPlayerAnimInstance.h"

#include "ZandorraCharacter.h"
#include "Kismet/KismetMathLibrary.h"

UZandorraPlayerAnimInstance::UZandorraPlayerAnimInstance()
{
	
	
}


void UZandorraPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UZandorraPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	// UpdateAnimationProperties(DeltaSeconds);


}


void UZandorraPlayerAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	const FRotator AimRotation = ZCharacter->GetBaseAimRotation();
	const FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ZCharacter->GetVelocity());

	MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
}

void UZandorraPlayerAnimInstance::TurnInPlace()
{

}

