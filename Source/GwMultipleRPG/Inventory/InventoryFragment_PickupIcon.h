// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GwInventoryItemFragment.h"
#include "InventoryFragment_PickupIcon.generated.h"

/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API UInventoryFragment_PickupIcon : public UGwInventoryItemFragment
{
	GENERATED_BODY()
public:
	UInventoryFragment_PickupIcon();

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	// TObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FLinearColor PadColor;
};
