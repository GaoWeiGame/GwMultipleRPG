// Fill out your copyright notice in the Description page of Project Settings.


#include "GwGameplayAbilityJump.h"

#include "GwMultipleRPG/Gameplay/Character/GwCharacter.h"

UGwGameplayAbilityJump::UGwGameplayAbilityJump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UGwGameplayAbilityJump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
                                                FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGwGameplayAbilityJump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CharacterJumpStop();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGwGameplayAbilityJump::CharacterJumpStart()
{
	if (AGwCharacter* GwCharacter = GetGwCharacterFromActorInfo())
	{
		if (GwCharacter->IsLocallyControlled() && !GwCharacter->bPressedJump)
		{
			GwCharacter->UnCrouch();
			GwCharacter->Jump();
		}
	}
}

void UGwGameplayAbilityJump::CharacterJumpStop()
{
	if (AGwCharacter* GwCharacter = GetGwCharacterFromActorInfo())
	{
		if (GwCharacter->IsLocallyControlled() && GwCharacter->bPressedJump)
		{
			GwCharacter->StopJumping();
		}
	}
}
