// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "GameFramework/PlayerState.h"
#include "GwMultipleRPG/AbilitySystem/GwAbilitySystemComponent.h"
#include "GwMultipleRPG/Gameplay/Character/GwPawnData.h"
#include "GwMultipleRPG/System/GameplayTagStack.h"
#include "GwMultipleRPG/Teams/GwTeamAgentInterface.h"

#include "GwPlayerState.generated.h"

class UGwExperienceDefinition;
class UGwPawnData;

/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API AGwPlayerState : public APlayerState, public IAbilitySystemInterface, public IGwTeamAgentInterface
{
	GENERATED_BODY()

public:
	AGwPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UFUNCTION(BlueprintCallable, Category = "Gw|PlayerState")
	UGwAbilitySystemComponent* GetGwAbilitySystemComponent() const { return AbilitySystemComponent; }

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const UGwPawnData* InPawnData);

	virtual void PostInitializeComponents() override;


	virtual void Reset() override;
	virtual void ClientInitialize(AController* C) override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	virtual void OnDeactivated() override;
	virtual void OnReactivated() override;

	UFUNCTION(BlueprintCallable)
	int32 GetTeamId() const
	{
		return GenericTeamIdToInteger(MyTeamID);
	}

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);
	
	static const FName NAME_GwAbilityReady;

	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnGwTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGwAbilitySystemComponent> AbilitySystemComponent;


	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const UGwPawnData> PawnData;
	UFUNCTION()
	void OnRep_PawnData();

private:
	void OnExperienceLoaded(const UGwExperienceDefinition* CurrentExperience);

	UPROPERTY()
	TObjectPtr<const class UGwHealthSet> HealthSet;

	UPROPERTY()
	TObjectPtr<const class UGwCombatSet> CombatSet;

	UPROPERTY(ReplicatedUsing=OnRep_MyTeamID)
	FGenericTeamId MyTeamID;

	UFUNCTION()
	void OnRep_MyTeamID(FGenericTeamId OldTeamID);

	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	UPROPERTY()
	FOnGwTeamIndexChangedDelegate OnTeamChangedDelegate;
};
