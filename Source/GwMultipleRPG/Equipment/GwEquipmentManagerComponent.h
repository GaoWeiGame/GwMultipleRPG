// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "GwMultipleRPG/AbilitySystem/GwAbilitySet.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "GwEquipmentManagerComponent.generated.h"


struct FGwEquipmentList;
class UGwAbilitySystemComponent;
class UGwEquipmentInstance;
class UGwEquipmentManagerComponent;
class UGwEquipmentDefinition;

USTRUCT(BlueprintType)
struct FGwAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()
	FGwAppliedEquipmentEntry()
	{
	}

private:
	friend UGwEquipmentManagerComponent;
	friend FGwEquipmentList;

	UPROPERTY()
	TObjectPtr<UGwEquipmentInstance> Instance = nullptr;

	UPROPERTY()
	TSubclassOf<UGwEquipmentDefinition> EquipmentDefinition;

	UPROPERTY(NotReplicated)
	FGwAbilitySet_GrantedHandles GrantedHandles;

};

USTRUCT(BlueprintType)
struct FGwEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FGwEquipmentList()
		: OwnerComponent(nullptr)
	{
	}

	FGwEquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	UGwEquipmentInstance* AddEntry(TSubclassOf<UGwEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UGwEquipmentInstance* Instance);
	
private:
	friend UGwEquipmentManagerComponent;

	UGwAbilitySystemComponent* GetAbilitySystemComponent() const;

	UPROPERTY()
	TArray<FGwAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class GWMULTIPLERPG_API UGwEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UGwEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UGwEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UGwEquipmentInstance> InstanceType) const;

	UFUNCTION(BlueprintAuthorityOnly)
	void UnequipItem(UGwEquipmentInstance* ItemInstance);

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UGwEquipmentInstance* EquipItem(TSubclassOf<UGwEquipmentDefinition> EquipmentClass);
	
private:
	UPROPERTY(Replicated)
	FGwEquipmentList EquipmentList;
};
