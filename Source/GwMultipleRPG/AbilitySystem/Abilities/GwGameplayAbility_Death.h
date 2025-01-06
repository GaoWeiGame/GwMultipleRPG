// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GwGameplayAbility.h"
#include "GwGameplayAbility_Death.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class GWMULTIPLERPG_API UGwGameplayAbility_Death : public UGwGameplayAbility
{
	GENERATED_BODY()
public:

	UGwGameplayAbility_Death(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "Gw|Ability")
	void StartDeath();

	// Finishes the death sequence.
	UFUNCTION(BlueprintCallable, Category = "Gw|Ability")
	void FinishDeath();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gw|Death")
	bool bAutoStartDeath;
};
