// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ZandorraPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ZANDORRA_API UZandorraPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	public:

	UZandorraPlayerAnimInstance();

		
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float RotateRootAmount = 0.f;
	
	protected:

	void TurnInPlace();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	private:

	UPROPERTY(VisibleAnywhere)
	class AZandorraCharacter* ZCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "My Stuff", meta = (AllowPrivateAccess = true))
	float MovementOffsetYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "My Stuff", meta = (AllowPrivateAccess = true))
	float LastMovementOffsetYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "My Stuff", meta = (AllowPrivateAccess = true))
	float LastFrameMovementOffsetYaw;

	// Yaw of the Character This frame - used for turn in place anims
	float CharacterYaw;

	//Yaw of the character in the previous frame - used for turn in place anims
	float CharacterYawLastFrame;
	float YawExcess = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "My Stuff", meta = (AllowPrivateAccess = true))
	float RootYawOffset;
	//Rotation Curve Value used for turn in place anims
	float RotationCurve;
	// Rotation Curve value held from last frame used for turn in place anims
	float RotationCurveLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category= "My Stuff", meta = (AllowPrivateAccess=true))
	bool bTurningInPlace;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Category= "My Stuff", meta = (AllowPrivateAccess = true))
	bool bShouldUseFabrik;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category= "My Stuff", meta = (AllowPrivateAccess=true))
	bool bCharIsDead;

	
};
