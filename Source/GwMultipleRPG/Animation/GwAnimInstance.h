// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "GwAnimInstance.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API UGwAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	UGwAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintPure,Category=Speed)
	float GetSpeed();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
	
private:
	float Speed;
};
