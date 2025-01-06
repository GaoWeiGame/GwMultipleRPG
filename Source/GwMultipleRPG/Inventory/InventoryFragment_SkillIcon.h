// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GwInventoryItemFragment.h"
#include "InventoryFragment_SkillIcon.generated.h"

/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API UInventoryFragment_SkillIcon : public UGwInventoryItemFragment
{
	GENERATED_BODY()
public:
	UInventoryFragment_SkillIcon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateBrush Brush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	float Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FName Keyboard;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FName QuickBarName;
};
