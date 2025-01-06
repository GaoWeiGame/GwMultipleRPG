// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GwMultipleRPG/Cosmetics/GwCosmeticAnimationTypes.h"
#include "GwMultipleRPG/Equipment/GwEquipmentInstance.h"
#include "GwWeaponInstance.generated.h"

struct FGameplayTagContainer;
/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API UGwWeaponInstance : public UGwEquipmentInstance
{
	GENERATED_BODY()
public:
	UGwWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FGwAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FGwAnimLayerSelectionSet UneuippedAnimSet;
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Animation)
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const;
};
