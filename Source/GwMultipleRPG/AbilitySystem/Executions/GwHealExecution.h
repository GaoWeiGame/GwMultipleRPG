// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GwHealExecution.generated.h"

/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API UGwHealExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:

	UGwHealExecution();

protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
