// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GwGameplayAbility.h"
#include "GwGameplayAbility_Reset.generated.h"

/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API UGwGameplayAbility_Reset : public UGwGameplayAbility
{
	GENERATED_BODY()
public:
	UGwGameplayAbility_Reset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};

USTRUCT(BlueprintType)
struct FGwPlayerResetMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> OwnerPlayerState = nullptr;
};
