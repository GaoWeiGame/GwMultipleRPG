// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GwAttributeSet.h"
#include "GwCombatSet.generated.h"

/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API UGwCombatSet : public UGwAttributeSet
{
	GENERATED_BODY()
public:

	UGwCombatSet();

	ATTRIBUTE_ACCESSORS(UGwCombatSet, BaseDamage);
	ATTRIBUTE_ACCESSORS(UGwCombatSet, BaseHeal);

protected:

	UFUNCTION()
	void OnRep_BaseDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_BaseHeal(const FGameplayAttributeData& OldValue);

private:

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseDamage, Category = "Gw|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseHeal, Category = "Gw|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseHeal;
};
