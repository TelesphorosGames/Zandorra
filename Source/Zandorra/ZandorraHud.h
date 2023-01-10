// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ZandorraHud.generated.h"

/**
 * 
 */


UCLASS()
class ZANDORRA_API AZandorraHud : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* CrosshairsCenter;
	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* CrosshairsLeft;
	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* CrosshairsRight;
	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* CrosshairsTop;
	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* CrosshairsBottom;

	
	
	
	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;

	FLinearColor CrosshairsColor = FColor::White;

	virtual void DrawHUD() override;

	//
	// UPROPERTY(EditAnywhere,Category=Stats)
	// TSubclassOf<class UUserWidget> CharacterOverlayClass;
	// UPROPERTY()
	// class UCharacterOverlay* CharacterOverlay{};

	UFUNCTION()
	void DrawCrosshairs(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);


	FORCEINLINE void SetCrosshairSpread(const float InSpread) {CrosshairSpread = InSpread;}
	FORCEINLINE void SetCrosshairsColor(const FLinearColor InColor) {CrosshairsColor = InColor;}

	
protected:

	
private:
	float CrosshairSpread;
	
	
};
