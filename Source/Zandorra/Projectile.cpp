#include "Projectile.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Damageable.h"


AProjectile::AProjectile()
{
 	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	// CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	ProjectileMovementComponent->OnProjectileStop.AddDynamic(this, &AProjectile::OnStop);

	if(Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			Tracer,
			CollisionBox,
			FName(""),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition
		);
	}
	if(BarrelFlashParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BarrelFlashParticles, GetActorLocation(), GetActorRotation());
	}

	if(LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}
	
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::SpawnTrailSystem()
{
	if(TrailSystem)
	{
		TrailSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(TrailSystem, GetRootComponent(), FName(), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(ImpactParticles && HitPlayerParticles)
	{
		if(OtherComp->GetCollisionObjectType() == ECC_Pawn)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitPlayerParticles, GetActorTransform());
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
		}
    	
	}
	
	if(ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	IDamageable* DamageableObject = Cast<IDamageable>(OtherActor);
	if(DamageableObject)
	{
		DamageableObject->AddDamage(OtherActor, DamageAmount, DamageType, nullptr, GetOwner());
		// UE_LOG(LogTemp, Warning, TEXT("Hit Character : %s , Damage Applied : %f"), , DamageAmount);
	}
	
	Destroy();
}

void AProjectile::OnStop(const FHitResult& ImpactResult)
{
	if(ImpactParticles && HitPlayerParticles)
	{
		if(ImpactResult.Component->GetCollisionObjectType() == ECC_Pawn)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitPlayerParticles, GetActorTransform());
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
		}
    	
	}
	
	if(ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	IDamageable* DamageableObject = Cast<IDamageable>(ImpactResult.GetActor());
	if(DamageableObject)
	{
		DamageableObject->AddDamage(ImpactResult.GetActor(), DamageAmount, DamageType, GetOwner()->GetInstigatorController(), GetOwner());
		// UE_LOG(LogTemp, Warning, TEXT("Hit Character : %s , Damage Applied : %f"), , DamageAmount);
	}
	
	Destroy();
}
