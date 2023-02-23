// Fill out your copyright notice in the Description page of Project Settings.


#include "BeamAttackComponent.h"

#include "ZandorraCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

UBeamAttackComponent::UBeamAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBeamAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	ZCharacter = Cast<AZandorraCharacter>(GetOwner());
}

void UBeamAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBeamAttackComponent::StartBeamTimer()
{
	if(ZCharacter)
	{
		ZCharacter->GetWorldTimerManager().SetTimer(BeamTimer, this, &UBeamAttackComponent::BeamTimerFinished, BeamFireDelay);
	}
}

void UBeamAttackComponent::BeamTimerFinished()
{
	if(ZCharacter->bAbilityButtonHeld)
	{
		bHeldFire = true;
		StartBeamAttack();
	}
	else if(bHeldFire)
	{
		BeamAttackFinished();
		bHeldFire = false;
	}
}

void UBeamAttackComponent::StartBeamAttack()
{
	
	if(!ZCharacter) return;

	if(WeaponCharge <=0.f)
	{
		BeamAttackFinished();
		return;
	}
	
	ZCharacter->bUsingBeamAttack = true;
	WeaponCharge -= BeamEnergyCost;
	StartBeamTimer();
	
	UAnimInstance* AnimInstance = ZCharacter->GetMesh()->GetAnimInstance();

	if(bHeldFire)
	{
		if(ZCharacter->GetAttackMontage() && AnimInstance)
		{
			AnimInstance->Montage_Play(ZCharacter->GetAttackMontage());
			AnimInstance->Montage_JumpToSection("BeamHold");			
		}
	}
	
	else
	{
		if(ZCharacter->GetAttackMontage() && AnimInstance)
        {
        	AnimInstance->Montage_Play(ZCharacter->GetAttackMontage());
        	AnimInstance->Montage_JumpToSection("Beam");
        }
	}
	
	const USkeletalMeshSocket* BarrelSocket = ZCharacter->GetMesh()->GetSocketByName("Barrel");
	if(BarrelSocket)
	{
		FTransform BarrelSocketTransform = BarrelSocket->GetSocketTransform(ZCharacter->GetMesh());
		FHitResult Result;

		// if(ZCharacter->GetVelocity().Size() > 0 && ZCharacter->MoveForwardAxisValue <= -.3f && ZCharacter->GetCharacterMovementState() != ECharacterMovementState::ECMS_LockedOn)
		// {
		// 	const FVector BarrelEnd = BarrelSocketTransform.GetLocation() + (BarrelSocketTransform.GetUnitAxis(EAxis::X) *5000);
		// 	WeaponTraceHit(BarrelSocketTransform.GetLocation(), BarrelEnd, Result);
		// 	return;
		// }
		WeaponTraceHit(BarrelSocketTransform.GetLocation(), ZCharacter->GetCrosshairsTarget(), Result);
	}
}

void UBeamAttackComponent::BeamAttackFinished()
{
	if(!ZCharacter) return;
	ZCharacter->bUsingBeamAttack = false;
	UAnimInstance* AnimInstance = ZCharacter->GetMesh()->GetAnimInstance();
	if(ZCharacter->GetAttackMontage() && AnimInstance)
	{
		AnimInstance->Montage_Play(ZCharacter->GetAttackMontage());
		AnimInstance->Montage_JumpToSection("BeamHoldEnd");
	}
}

void UBeamAttackComponent::AdjustWeaponCharge(float AmountToAdjustBy)
{
}

void UBeamAttackComponent::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit)
{
	UWorld* World = GetWorld();
	if (World)
	{
		const FVector End = TraceStart + (HitTarget - TraceStart) * 1.25;
		
		World->LineTraceSingleByChannel(
			OutHit,
			TraceStart,
			End, 
			ECollisionChannel::ECC_Visibility
			);

		FVector BeamEnd = End;
		FVector BeamHitNormals;
		
		if(OutHit.bBlockingHit && OutHit.GetActor() != GetOwner())
		{
			BeamEnd= OutHit.ImpactPoint;
			BeamHitNormals = OutHit.Normal;

			IDamageable* DamageableObject = Cast<IDamageable>(OutHit.GetActor());
			if(DamageableObject)
			{
				const UDamageType* DamageType{};
				DamageableObject->AddDamage(OutHit.GetActor(), BeamDamageAmount, DamageType, nullptr, GetOwner());
				UE_LOG(LogTemp, Warning, TEXT("Hit Character : %s , Damage Applied : %f"), *OutHit.GetActor()->GetName(), BeamDamageAmount);
			}
		}
		
		// DrawDebugSphere(GetWorld(), BeamEnd, 16.f, 12, FColor::Magenta, false, -1);
		
		if(BeamSystem)
		{
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(World, BeamSystem, TraceStart, FRotator::ZeroRotator, true);
			if(Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
		}
		
		
		
		if(BeamImpactParticles && BeamEnd != End)
		{
			UGameplayStatics::SpawnEmitterAtLocation(World, BeamImpactParticles, BeamEnd, BeamHitNormals.Rotation(), true);
		}
		if(BeamImpactSound && BeamEnd != End )
		{
			UGameplayStatics::PlaySoundAtLocation(World, BeamImpactSound, BeamEnd);
		}
	}
}

float UBeamAttackComponent::GetWeaponChargePercentage()
{
	return WeaponCharge / WeaponChargeMax;
}


