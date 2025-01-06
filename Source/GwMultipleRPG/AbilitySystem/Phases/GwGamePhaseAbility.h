// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GwMultipleRPG/AbilitySystem/Abilities/GwGameplayAbility.h"
#include "GwGamePhaseAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract, HideCategories = Input)
class GWMULTIPLERPG_API UGwGamePhaseAbility : public UGwGameplayAbility
{
	GENERATED_BODY()
public:

	UGwGamePhaseAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const FGameplayTag& GetGamePhaseTag() const { return GamePhaseTag; }

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gw|Game Phase")
	FGameplayTag GamePhaseTag;
};
