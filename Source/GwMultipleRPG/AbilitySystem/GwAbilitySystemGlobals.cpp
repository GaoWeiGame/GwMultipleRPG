// Fill out your copyright notice in the Description page of Project Settings.


#include "GwAbilitySystemGlobals.h"

#include "GwGameplayEffectContext.h"

FGameplayEffectContext* UGwAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FGwGameplayEffectContext();
}
