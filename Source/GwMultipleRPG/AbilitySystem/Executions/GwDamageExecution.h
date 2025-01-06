﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GwDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API UGwDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:

	UGwDamageExecution();

protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
