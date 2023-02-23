// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FriendlyAIController.generated.h"

/**
 * 
 */
UCLASS()
class ZANDORRA_API AFriendlyAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
};
