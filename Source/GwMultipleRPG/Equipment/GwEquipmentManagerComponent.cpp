// Fill out your copyright notice in the Description page of Project Settings.


#include "GwEquipmentManagerComponent.h"

#include "AbilitySystemGlobals.h"
#include "GwEquipmentDefinition.h"
#include "GwEquipmentInstance.h"
#include "GwMultipleRPG/AbilitySystem/GwAbilitySet.h"
#include "GwMultipleRPG/AbilitySystem/GwAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"


UGwEquipmentInstance* FGwEquipmentList::AddEntry(TSubclassOf<UGwEquipmentDefinition> EquipmentDefinition)
{
	UGwEquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	const UGwEquipmentDefinition* EquipmentCDO = GetDefault<UGwEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UGwEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = UGwEquipmentInstance::StaticClass();
	}

	FGwAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UGwEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType); //@TODO: Using the actor instead of component as the outer due to UE-127172
	Result = NewEntry.Instance;

	if (UGwAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (const TObjectPtr<const UGwAbilitySet>& AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, /*inout*/ &NewEntry.GrantedHandles, Result);
		}
	}
	else
	{
		//@TODO: Warning logging?
	}

	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);


	MarkItemDirty(NewEntry);

	return Result;
}

void FGwEquipmentList::RemoveEntry(UGwEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FGwAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			if (UGwAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			Instance->DestroyEquipmentActors();


			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

UGwAbilitySystemComponent* FGwEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	return Cast<UGwAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

UGwEquipmentManagerComponent::UGwEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;

}

TArray<UGwEquipmentInstance*> UGwEquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<UGwEquipmentInstance> InstanceType) const
{
	TArray<UGwEquipmentInstance*> Results;
	for (const FGwAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UGwEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}

void UGwEquipmentManagerComponent::UnequipItem(UGwEquipmentInstance* ItemInstance)
{
	if (ItemInstance != nullptr)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(ItemInstance);
		}

		ItemInstance->OnUnequipped();
		EquipmentList.RemoveEntry(ItemInstance);
	}
}

void UGwEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("aaa"));
}

UGwEquipmentInstance* UGwEquipmentManagerComponent::EquipItem(TSubclassOf<UGwEquipmentDefinition> EquipmentClass)
{
	UGwEquipmentInstance* Result = nullptr;
	if (EquipmentClass != nullptr)
	{
		Result = EquipmentList.AddEntry(EquipmentClass);
		if (Result != nullptr)
		{
			Result->OnEquipped();

			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(Result);
			}
		}
	}
	return Result;
}


void UGwEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}
