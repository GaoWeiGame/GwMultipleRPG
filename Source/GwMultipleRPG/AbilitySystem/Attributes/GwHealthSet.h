// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GwAttributeSet.h"
#include "NativeGameplayTags.h"
#include "GwHealthSet.generated.h"


UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageImmunity);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageSelfDestruct);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_FellOutOfWorld);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gw_Damage_Message);


UCLASS(BlueprintType)
class GWMULTIPLERPG_API UGwHealthSet : public UGwAttributeSet
{
	GENERATED_BODY()

public:
	UGwHealthSet();

	ATTRIBUTE_ACCESSORS(UGwHealthSet, Health);
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);


	ATTRIBUTE_ACCESSORS(UGwHealthSet, MaxHealth);
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	mutable FGwAttributeEvent OnHealthChanged;

	mutable FGwAttributeEvent OnMaxHealthChanged;

	mutable FGwAttributeEvent OnOutOfHealth;


	ATTRIBUTE_ACCESSORS(UGwHealthSet, Damage);

	ATTRIBUTE_ACCESSORS(UGwHealthSet, Healing);


protected:
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:
	UPROPERTY(BlueprintReadOnly, Category="Gw|Health", Meta=(HideFromModifiers, AllowPrivateAccess=true))
	FGameplayAttributeData Damage;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes", Meta = ( AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category="Gw|Health", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Healing;

	float MaxHealthBeforeAttributeChange;
	float HealthBeforeAttributeChange;

	bool bOutOfHealth;
};
