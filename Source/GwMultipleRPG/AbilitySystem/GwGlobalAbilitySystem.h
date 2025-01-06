// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"
#include "GwGlobalAbilitySystem.generated.h"

struct FActiveGameplayEffectHandle;
class UGameplayAbility;
class UGameplayEffect;
struct FGameplayAbilitySpecHandle;
class UGwAbilitySystemComponent;

USTRUCT()
struct FGlobalAppliedAbilityList
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<TObjectPtr<UGwAbilitySystemComponent>, FGameplayAbilitySpecHandle> Handles;

	void AddToASC(TSubclassOf<UGameplayAbility> Ability, UGwAbilitySystemComponent* ASC);
	void RemoveFromASC(UGwAbilitySystemComponent* ASC);
	void RemoveFromAll();
};

USTRUCT()
struct FGlobalAppliedEffectList
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<TObjectPtr<UGwAbilitySystemComponent>, FActiveGameplayEffectHandle> Handles;

	void AddToASC(TSubclassOf<UGameplayEffect> Effect, UGwAbilitySystemComponent* ASC);
	void RemoveFromASC(UGwAbilitySystemComponent* ASC);
	void RemoveFromAll();
};


UCLASS()
class GWMULTIPLERPG_API UGwGlobalAbilitySystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UGwGlobalAbilitySystem();

	void RegisterASC(UGwAbilitySystemComponent* ASC);

	void UnregisterASC(UGwAbilitySystemComponent* ASC);
private:
	UPROPERTY()
	TMap<TSubclassOf<UGameplayAbility>, FGlobalAppliedAbilityList> AppliedAbilities;

	UPROPERTY()
	TMap<TSubclassOf<UGameplayEffect>, FGlobalAppliedEffectList> AppliedEffects;

	UPROPERTY()
	TArray<TObjectPtr<UGwAbilitySystemComponent>> RegisteredASCs;
};
