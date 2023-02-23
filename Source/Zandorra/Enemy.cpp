// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "HealthComponent.h"

#include "Zandorra.h"
#include "ZandorraCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemy::AEnemy()
{

	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);

	EnemyHealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	
	MeleeCombatCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Melee Combat Collision Box"));
	MeleeCombatCollisionBox->SetupAttachment(GetMesh(), FName("headSocket"));

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnemy::AddDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
	AActor* DamageCauser)
{
	EnemyHealthComponent->AdjustHealth(-Damage);
	if(EnemyHealthComponent->Health <= 0.f)
	{
		NoMoreHealth(DamageCauser);
	}
	
}

void AEnemy::CycleTargetAfterNoMoreHealth(AActor* KillingChar)
{
	AZandorraCharacter* Attacker= Cast<AZandorraCharacter>(KillingChar);
	if(Attacker && Attacker->GetCharacterMovementState() == ECharacterMovementState::ECMS_LockedOn)
	{
		Attacker->LockOnButtonPressed();
	}
}

void AEnemy::NoMoreHealth(AActor* Causer)
{
	bAlive = false;
	GetCharacterMovement()->StopMovementImmediately();
	
	CycleTargetAfterNoMoreHealth(Causer);
	SetDestroyTimer();
	
}

void AEnemy::Attack()
{
	if(GetMesh()->GetAnimInstance() && CombatMontage)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(CombatMontage);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection("AttackOne", CombatMontage);
	}
	
}

void AEnemy::AttackTwo()
{
	if(GetMesh()->GetAnimInstance() && CombatMontage)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(CombatMontage);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection("AttackTwo", CombatMontage);
	}
}

void AEnemy::AttackThree()
{
	if(GetMesh()->GetAnimInstance() && CombatMontage)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(CombatMontage);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection("AttackThree", CombatMontage);
	}
}

void AEnemy::AttackEnd()
{
	DeActivateMeleeCollision();
}

void AEnemy::DeathEnd()
{
	Destroy();
}

void AEnemy::ActivateMeleeCollision()
{
	MeleeCombatCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::DeActivateMeleeCollision()
{
	MeleeCombatCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
