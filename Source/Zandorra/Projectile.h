// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class ZANDORRA_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:

	
	AProjectile();
	
	/* OVERRIDES
	 */
	
	virtual void Tick(float DeltaTime) override;
	
	/* PUBLIC FUNCTIONS
	 */
	
	/* PUBLIC VARIABLES
	 */
	
	/* GETTERS AND SETTERS
	 */

protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;
	
	UPROPERTY(EditAnywhere)
	class USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* HitPlayerParticles;
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;
	
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* TrailSystem;
	
	UPROPERTY()
	class UNiagaraComponent* TrailSystemComponent;
	
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* ProjectileMesh;

	UFUNCTION()
	void SpawnTrailSystem();

private:

	UPROPERTY(EditAnywhere)
	class UParticleSystem* Tracer;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* TracerComponent;

	



};
