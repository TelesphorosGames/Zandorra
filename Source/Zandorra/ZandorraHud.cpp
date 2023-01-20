#include "ZandorraHud.h"

#include "Blueprint/UserWidget.h"


AZandorraHud::AZandorraHud()
{
	
}

void AZandorraHud::DrawHUD()
{

	Super::DrawHUD();

	
	FVector2D ViewportSize;
	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		

		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

		const float BaseSpread= 45.f;
		const float SpreadScaled = (CrosshairSpreadMax * CrosshairSpread) + BaseSpread;
			
		
		if(CrosshairsCenter != nullptr)
		{
			const FVector2D Spread{0.f, 0.f};
			DrawCrosshairs(CrosshairsCenter, ViewportCenter, Spread, CrosshairsColor);
		}
		
		if(CrosshairsLeft != nullptr)
		{
			const FVector2D Spread{-SpreadScaled,0.f};
			DrawCrosshairs(CrosshairsLeft, ViewportCenter, Spread, CrosshairsColor);
		}
		if(CrosshairsRight != nullptr)
		{
			const FVector2D Spread{SpreadScaled, 0.f};
			DrawCrosshairs(CrosshairsRight, ViewportCenter, Spread, CrosshairsColor);
		}
		if(CrosshairsTop != nullptr)
		{
			const FVector2D Spread{0.f, -SpreadScaled};
			DrawCrosshairs(CrosshairsTop, ViewportCenter, Spread, CrosshairsColor);
		}
		if(CrosshairsBottom != nullptr)
		{
			const FVector2D Spread{0.f, SpreadScaled};
			DrawCrosshairs(CrosshairsBottom, ViewportCenter, Spread, CrosshairsColor);
		}
	}
}

void AZandorraHud::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetOwningPlayerController();
	if(CharacterOverlayClass && PlayerController)
	{
		CharacterOverlay = CreateWidget<UUserWidget>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
	
	
}

void AZandorraHud::DrawCrosshairs(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread,
                                  FLinearColor CrosshairColor)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint(ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,	ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y);
	DrawTexture(Texture, TextureDrawPoint.X, TextureDrawPoint.Y, TextureWidth, TextureHeight, 0.f, 0.f, 1.f,1.f, CrosshairColor);	
}

