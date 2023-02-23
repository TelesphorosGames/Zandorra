// Fill out your copyright notice in the Description page of Project Settings.


#include "ZandorraPlayerController.h"

#include "ZandorraGameMode.h"

void AZandorraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AZandorraGameMode* ZandorraGameMode = Cast<AZandorraGameMode>(GetWorld()->GetAuthGameMode());
	if(ZandorraGameMode)
	{
		ZandorraGameMode->SetPlayerController(this);
	}
	
}
