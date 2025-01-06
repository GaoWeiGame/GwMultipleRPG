// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GwMultipleRPG/UI/GwTaggedWidget.h"
#include "W_GwMinMapPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API UW_GwMinMapPlayer : public UGwTaggedWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	APawn* Pawn;
};
