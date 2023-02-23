// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendlyAIController.h"


#include "ZandorraGameMode.h"

void AFriendlyAIController::BeginPlay()
{
	Super::BeginPlay();

	AZandorraGameMode* ZandorraGameMode = Cast<AZandorraGameMode>(GetWorld()->GetAuthGameMode());
	if(ZandorraGameMode)
	{
		ZandorraGameMode->SetAIController(this);
	}
}
