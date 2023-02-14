// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "InteractWithCrosshairs.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class AZandorraCharacter;
UENUM(BlueprintType)
enum class EEnemyMovementStatus : uint8
{
	EMS_Idle UMETA(DisplayName="Idle"),
	EMS_MoveToTarget UMETA(DisplayName="MoveToTarget"),
	EMS_Attacking UMETA(DisplayName="Attacking"),
	EMS_Dead UMETA(DisplayName="Dead"),

	EMS_MAX UMETA(DisplayName="DefaultMax")
};

UCLASS()
class ZANDORRA_API AEnemy : public ACharacter, public IInteractWithCrosshairs, public IDamageable
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus(){return EnemyMovementStatus;}
	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status){EnemyMovementStatus = Status;}
	FORCEINLINE bool GetAlive() {return bAlive;}

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "My Stuff")
	TObjectPtr<class UHealthComponent> EnemyHealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "My Stuff")
	TObjectPtr<UAnimMontage> CombatMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "My Stuff")
	TObjectPtr<class UBoxComponent> MeleeCombatCollisionBox;

	EEnemyMovementStatus EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;

	virtual void AddDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) override;
	
	virtual void NoMoreHealth(AActor* Causer);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "My Stuff")
	TObjectPtr<UParticleSystem> HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	TObjectPtr<class USoundCue> HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "My Stuff")
	float MeleeDamage =0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	TSubclassOf<UDamageType> MeleeDamageTypeClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff");
	TObjectPtr<AZandorraCharacter> CombatTarget;

	UFUNCTION(BlueprintCallable)
	virtual void ActivateMeleeCollision();
	
	UFUNCTION(BlueprintCallable)
	virtual void DeActivateMeleeCollision();

	virtual void Attack();

	virtual void AttackTwo();

	virtual void AttackThree();
	

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	virtual void DeathEnd();

	bool bAlive = false;
	


};
