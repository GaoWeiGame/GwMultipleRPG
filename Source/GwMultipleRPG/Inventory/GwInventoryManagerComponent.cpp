// Fill out your copyright notice in the Description page of Project Settings.


#include "GwInventoryManagerComponent.h"

#include "Net/UnrealNetwork.h"
#include "GwInventoryItemDefinition.h"
#include "GwInventoryItemFragment.h"
#include "GwInventoryItemInstance.h"
#include "InventoryFragment_SkillIcon.h"

UE_DISABLE_OPTIMIZATION

UGwInventoryItemInstance* FGwInventoryList::AddEntry(TSubclassOf<UGwInventoryItemDefinition> ItemDef, int32 StackCount)
{
	UGwInventoryItemInstance* Result = nullptr;

	check(ItemDef != nullptr);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());


	FGwInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UGwInventoryItemInstance>(OwnerComponent->GetOwner()); //@TODO: Using the actor instead of component as the outer due to UE-127172
	NewEntry.Instance->SetItemDef(ItemDef);
	for (UGwInventoryItemFragment* Fragment : GetDefault<UGwInventoryItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	NewEntry.StackCount = StackCount;
	Result = NewEntry.Instance;

	MarkItemDirty(NewEntry);

	return Result;
}

void FGwInventoryList::AddEntry(UGwInventoryItemInstance* Instance)
{
	unimplemented();
}

TArray<UGwInventoryItemInstance*> FGwInventoryList::GetAllItems() const
{
	TArray<UGwInventoryItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FGwInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr)
		{
			Results.Add(Entry.Instance);
		}
	}
	return Results;
}

void FGwInventoryList::RemoveEntry(UGwInventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FGwInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

UGwInventoryManagerComponent::UGwInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , InventoryList(this)
{
	SetIsReplicatedByDefault(true);
}

UGwInventoryItemInstance* UGwInventoryManagerComponent::AddItemDefinition(TSubclassOf<UGwInventoryItemDefinition> ItemDef, int32 StackCount)
{
	UGwInventoryItemInstance* Result = nullptr;
	if (ItemDef != nullptr)
	{
		Result = InventoryList.AddEntry(ItemDef, StackCount);
		bool a = IsUsingRegisteredSubObjectList();
		bool b = IsReadyForReplication();
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}
	return Result;
}

void UGwInventoryManagerComponent::AddItemInstance(UGwInventoryItemInstance* ItemInstance)
{
	InventoryList.AddEntry(ItemInstance);
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
		AddReplicatedSubObject(ItemInstance);
	}
}

UGwInventoryItemInstance* UGwInventoryManagerComponent::FindFirstItemStackByDefinition(TSubclassOf<UGwInventoryItemDefinition> ItemDef) const
{
	for (const FGwInventoryEntry& Entry : InventoryList.Entries)
	{
		UGwInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

TArray<UGwInventoryItemInstance*> UGwInventoryManagerComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

TArray<UGwInventoryItemInstance*> UGwInventoryManagerComponent::GetAllProp() const
{
	TArray<UGwInventoryItemInstance*> outPut;
	for (UGwInventoryItemInstance* Item : InventoryList.GetAllItems())
	{
		if (!Item->FindFragmentByClass<UInventoryFragment_SkillIcon>())
		{
			outPut.Add(Item);
		}
	}
	return outPut;
}

void UGwInventoryManagerComponent::RemoveItemInstance(UGwInventoryItemInstance* ItemInstance)
{
	InventoryList.RemoveEntry(ItemInstance);

	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}

void UGwInventoryManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, InventoryList);
}

UE_ENABLE_OPTIMIZATION
