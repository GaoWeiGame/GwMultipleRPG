// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GwGameplayAbility.h"
#include "GwGameplayAbilityJump.generated.h"

/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API UGwGameplayAbilityJump : public UGwGameplayAbility
{
	GENERATED_BODY()
public:
	UGwGameplayAbilityJump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "Gw|Ability")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Gw|Ability")
	void CharacterJumpStop();
};
