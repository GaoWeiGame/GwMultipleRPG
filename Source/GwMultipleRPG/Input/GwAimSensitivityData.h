// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GwAimSensitivityData.generated.h"

enum class EGwGamepadSensitivity : uint8;
/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API UGwAimSensitivityData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	const float SensitivtyEnumToFloat(const EGwGamepadSensitivity InSensitivity) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EGwGamepadSensitivity, float> SensitivityMap;
};
