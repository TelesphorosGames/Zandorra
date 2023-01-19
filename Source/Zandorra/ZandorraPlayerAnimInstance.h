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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float RotateRootAmount = 0.f;
	
	protected:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	private:

	UPROPERTY(VisibleAnywhere)
	class AZandorraCharacter* ZCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category= "My Stuff", meta = (AllowPrivateAccess=true))
	bool bCharIsDead;

	
};
