// Fill out your copyright notice in the Description page of Project Settings.


#include "GwCombatSet.h"

#include "Net/UnrealNetwork.h"

UGwCombatSet::UGwCombatSet()
	: BaseDamage(0.0f)
	  , BaseHeal(0.0f)
{
}

void UGwCombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGwCombatSet, BaseDamage, OldValue);
}

void UGwCombatSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGwCombatSet, BaseHeal, OldValue);
}


void UGwCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGwCombatSet, BaseDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGwCombatSet, BaseHeal, COND_OwnerOnly, REPNOTIFY_Always);
}
