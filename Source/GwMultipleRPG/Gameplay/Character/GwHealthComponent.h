// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "GwHealthComponent.generated.h"

struct FGameplayEffectSpec;
class UGwHealthSet;
class UGwAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGwHealth_DeathEvent, AActor*, OwningActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FGwHealth_AttributeChanged, UGwHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);


UENUM(BlueprintType)
enum class EGwDeathState : uint8
{
	NotDead = 0,
	DeathStarted,
	DeathFinished
};


UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class GWMULTIPLERPG_API UGwHealthComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	UGwHealthComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "Gw|Health")
	static UGwHealthComponent* FindHealthComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UGwHealthComponent>() : nullptr); }

	UFUNCTION(BlueprintCallable, Category = "Gw|Health")
	void InitializeWithAbilitySystem(UGwAbilitySystemComponent* InASC);

	// Uninitialize the component, clearing any references to the ability system.
	UFUNCTION(BlueprintCallable, Category = "Gw|Health")
	void UninitializeFromAbilitySystem();
	
	UFUNCTION(BlueprintCallable, Category = "Gw|Health")
	float GetHealthNormalized() const;

	UFUNCTION(BlueprintCallable, Category = "Gw|Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Gw|Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Gw|Mana")
	float GetManaNormalized() const;
	
	virtual void StartDeath();

	virtual void FinishDeath();

	UFUNCTION(BlueprintCallable, Category = "Gw|Health")
	EGwDeathState GetDeathState() const { return DeathState; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Gw|Health", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsDeadOrDying() const { return (DeathState > EGwDeathState::NotDead); }
	
	UPROPERTY(BlueprintAssignable)
	FGwHealth_DeathEvent OnDeathStarted;

	UPROPERTY(BlueprintAssignable)
	FGwHealth_DeathEvent OnDeathFinished;

	UPROPERTY(BlueprintAssignable)
	FGwHealth_AttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FGwHealth_AttributeChanged OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FGwHealth_AttributeChanged OnManaChanged;

	virtual void DamageSelfDestruct(bool bFellOutOfWorld = false);

protected:
	UPROPERTY()
	TObjectPtr<UGwAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<const UGwHealthSet> HealthSet;

	virtual void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	virtual void HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	virtual void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

	virtual void HandleManaChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

	void ClearGameplayTags();


	UFUNCTION()
	virtual void OnRep_DeathState(EGwDeathState OldDeathState);
	
	UPROPERTY(ReplicatedUsing = OnRep_DeathState)
	EGwDeathState DeathState;
};
