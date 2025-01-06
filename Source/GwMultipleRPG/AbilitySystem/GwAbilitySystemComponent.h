// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "Abilities/GwGameplayAbility.h"

#include "GwAbilitySystemComponent.generated.h"

class UGwAbilityTagRelationshipMapping;
GWMULTIPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_AbilityInputBlocked);


UCLASS()
class GWMULTIPLERPG_API UGwAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UGwAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	void SetTagRelationshipMapping(UGwAbilityTagRelationshipMapping* NewMapping);

	void RemoveAbilityFromActivationGroup(EGwAbilityActivationGroup Group, UGwGameplayAbility* GwAbility);
	void AddAbilityToActivationGroup(EGwAbilityActivationGroup Group, UGwGameplayAbility* GwAbility);
	void CancelActivationGroupAbilities(EGwAbilityActivationGroup Group, UGwGameplayAbility* IgnoreGwAbility, bool bReplicateCancelAbility);
	bool IsActivationGroupBlocked(EGwAbilityActivationGroup Group) const;

	typedef TFunctionRef<bool(const UGwGameplayAbility* GwAbility, FGameplayAbilitySpecHandle Handle)> TShouldCancelAbilityFunc;
	void CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility);

protected:
	void TryActivateAbilitiesOnSpawn();

	virtual void NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability) override;
	virtual void NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason) override;
	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled) override;
	virtual void ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags) override;
	virtual void HandleChangeAbilityCanBeCanceled(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bCanBeCanceled) override;

	UFUNCTION(Client, Unreliable)
	void ClientNotifyAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason);

	void HandleAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason);

	
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	int32 ActivationGroupCounts[(uint8)EGwAbilityActivationGroup::MAX];

	UPROPERTY()
	TObjectPtr<UGwAbilityTagRelationshipMapping> TagRelationshipMapping;

};
