// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "GwInventoryManagerComponent.generated.h"


struct FGwInventoryList;
class UGwInventoryManagerComponent;
struct FGwInventoryEntry;
class UGwInventoryItemInstance;
class UGwInventoryItemDefinition;



USTRUCT(BlueprintType)
struct FGwInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

private:
	friend FGwInventoryList;

	friend UGwInventoryManagerComponent;
	
	UPROPERTY()
	TObjectPtr<UGwInventoryItemInstance> Instance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;
};

USTRUCT(BlueprintType)
struct FGwInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()
	FGwInventoryList()
		: OwnerComponent(nullptr)
	{
	}

	FGwInventoryList(UActorComponent* InOwnerComponent)
	: OwnerComponent(InOwnerComponent)
	{
	}
	
	UGwInventoryItemInstance* AddEntry(TSubclassOf<UGwInventoryItemDefinition> ItemClass, int32 StackCount);
	void AddEntry(UGwInventoryItemInstance* Instance);

	TArray<UGwInventoryItemInstance*> GetAllItems() const;

	void RemoveEntry(UGwInventoryItemInstance* Instance);

private:
	friend UGwInventoryManagerComponent;

	UPROPERTY()
	TArray<FGwInventoryEntry> Entries;
	
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;

};

USTRUCT(BlueprintType)
struct FGwInventoryChangeMessage
{
	GENERATED_BODY()

	//@TODO: Tag based names+owning actors for inventories instead of directly exposing the component?
	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<UActorComponent> InventoryOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<UGwInventoryItemInstance> Instance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 NewCount = 0;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 Delta = 0;
};


UCLASS(BlueprintType)
class GWMULTIPLERPG_API UGwInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGwInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UGwInventoryItemInstance* AddItemDefinition(TSubclassOf<UGwInventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void AddItemInstance(UGwInventoryItemInstance* ItemInstance);
	
	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure)
	UGwInventoryItemInstance* FindFirstItemStackByDefinition(TSubclassOf<UGwInventoryItemDefinition> ItemDef) const;

	//道具和技能
	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure=false)
	TArray<UGwInventoryItemInstance*> GetAllItems() const;

	//仅道具
	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure=false)
	TArray<UGwInventoryItemInstance*> GetAllProp() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void RemoveItemInstance(UGwInventoryItemInstance* ItemInstance);
	
private:
	UPROPERTY(Replicated)
	FGwInventoryList InventoryList;


};
