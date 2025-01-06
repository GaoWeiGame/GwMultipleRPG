// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GwTaggedWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class GWMULTIPLERPG_API UGwTaggedWidget : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UGwTaggedWidget(const FObjectInitializer& ObjectInitializer);

};
