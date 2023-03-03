#include "ShieldAbilityComponent.h"

#include "MurdockCharacter.h"
#include "ZandorraCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

UShieldAbilityComponent::UShieldAbilityComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	
}


float UShieldAbilityComponent::GetShieldChargePercentage()
{
	return ShieldCharge / ShieldChargeMax;
}

void UShieldAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	ZCharacter = Cast<AMurdockCharacter>(GetOwner());
	
	
}

void UShieldAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if(bShieldActive)
	// {
	// 	AdjustShieldCharge(ShieldDrainRate, DeltaTime);
	// 	if(ShieldCharge <= 0)
	// 	{
	// 		FinishShield();
	// 	}
	// }
	//

}

void UShieldAbilityComponent::StartShield()
{
	if(!ZCharacter || !ZCharacter->ShieldCollisionBox)
	{
		return;
	}
	if(ShieldCharge > 5)
	{
		if(ShieldSystem)
		{
			Shield = UGameplayStatics::SpawnEmitterAttached(ShieldSystem, ZCharacter->ShieldCollisionBox);
		}
		if(StartShieldSound)
		{
			ShieldSoundComponent = UGameplayStatics::SpawnSoundAttached(StartShieldSound, ZCharacter->ShieldCollisionBox);
		}
		
		bShieldActive = true;
		ZCharacter->bUsingBeamAttack = true;
			
		if(ZCharacter->GetMesh()->GetAnimInstance())
		{
			ZCharacter->GetMesh()->GetAnimInstance()->Montage_Play(ZCharacter->GetAttackMontage());
			ZCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection("ShieldStart", ZCharacter->GetAttackMontage());
		}
	}

	
}

void UShieldAbilityComponent::FinishShield()
{
	bShieldActive=false;
	
	if(Shield)
	{
		Shield->Deactivate();
	}
	if(ZCharacter)
	{
		ZCharacter->bUsingBeamAttack = false;
	}
	if(ShieldSoundComponent)
	{
		ShieldSoundComponent->Deactivate();
	}
	if(ZCharacter && ZCharacter->GetMesh() && ZCharacter->GetMesh()->GetAnimInstance())
	{
		ZCharacter->GetMesh()->GetAnimInstance()->Montage_Play(ZCharacter->GetAttackMontage());
		ZCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection("ShieldFinish", ZCharacter->GetAttackMontage());
	}
	
}

void UShieldAbilityComponent::AdjustShieldCharge(float AmountToAdjustBy, float DeltaTime)
{
	if(bShieldActive)
	{
		ShieldCharge -= AmountToAdjustBy * DeltaTime;
		ShieldCharge = FMath::Clamp(ShieldCharge, 0, ShieldCharge);
	}
	else
	{
		ShieldCharge += AmountToAdjustBy * DeltaTime;
		ShieldCharge = FMath::Clamp(ShieldCharge, 0, ShieldChargeMax);
	}
	
	
}

