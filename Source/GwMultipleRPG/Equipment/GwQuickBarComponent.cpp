// Fill out your copyright notice in the Description page of Project Settings.


#include "GwQuickBarComponent.h"

#include "GwEquipmentDefinition.h"
#include "GwEquipmentInstance.h"
#include "GwEquipmentManagerComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GwMultipleRPG/Inventory/GwInventoryItemInstance.h"
#include "GwMultipleRPG/Inventory/InventoryFragment_EquippableItem.h"
#include "Net/UnrealNetwork.h"
#include "NativeGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GwQuickBarComponent)


UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Gw_QuickBar_Message_SlotsChanged, "Gw.QuickBar.Message.SlotsChanged");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Gw_QuickBar_Message_ActiveIndexChanged, "Gw.QuickBar.Message.ActiveIndexChanged");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Gw_QuickBar_Message_SlotsChangedSkill, "Gw.QuickBar.Message.SlotsChangedSkill");

UE_DISABLE_OPTIMIZATION

UGwQuickBarComponent::UGwQuickBarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UGwQuickBarComponent::SetActiveSlotIndex_Implementation(int32 NewIndex)
{
	if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
	{
		UnequipItemInSlot();

		ActiveSlotIndex = NewIndex;

		EquipItemInSlot();

		OnRep_ActiveSlotIndex();
	}
}

void UGwQuickBarComponent::AddItemToSlot(int32 SlotIndex, UGwInventoryItemInstance* Item)
{
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		if (Slots[SlotIndex] == nullptr)
		{
			Slots[SlotIndex] = Item;
			OnRep_Slots();
		}
	}
}

void UGwQuickBarComponent::AddSkillToSlot(FName SkillName, UGwInventoryItemInstance* Item)
{
	if (!SkillName.IsNone() && (Item != nullptr))
	{
		int32 SkillIndex;
		SkillSlots.SkillNames.Find(SkillName, SkillIndex);
		if (SkillIndex >= 0)
		{
			SkillSlots.SkillNames[SkillIndex] = SkillName;
			SkillSlots.Slots[SkillIndex] = Item;
		}
		else
		{
			SkillSlots.SkillNames.Add(SkillName);
			SkillSlots.Slots.Add(Item);
		}
		OnRep_SkillSlots();
	}
}

void UGwQuickBarComponent::BeginPlay()
{
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}
	Super::BeginPlay();
}

int32 UGwQuickBarComponent::GetNextFreeItemSlot() const
{
	int32 SlotIndex = 0;
	for (const TObjectPtr<UGwInventoryItemInstance>& ItemPtr : Slots)
	{
		if (ItemPtr == nullptr)
		{
			return SlotIndex;
		}
		++SlotIndex;
	}

	return INDEX_NONE;
}

UGwInventoryItemInstance* UGwQuickBarComponent::RemoveItemFromSlot(int32 SlotIndex)
{
	UGwInventoryItemInstance* Result = nullptr;

	if (ActiveSlotIndex == SlotIndex)
	{
		UnequipItemInSlot();
		ActiveSlotIndex = -1;
	}

	if (Slots.IsValidIndex(SlotIndex))
	{
		Result = Slots[SlotIndex];

		if (Result != nullptr)
		{
			Slots[SlotIndex] = nullptr;
			OnRep_Slots();
		}
	}

	return Result;
}

void UGwQuickBarComponent::UnequipItemInSlot()
{
	if (UGwEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		if (EquippedItem != nullptr)
		{
			EquipmentManager->UnequipItem(EquippedItem);
			EquippedItem = nullptr;
		}
	}
}

void UGwQuickBarComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(EquippedItem == nullptr);

	if (UGwInventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		if (const UInventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<UInventoryFragment_EquippableItem>())
		{
			TSubclassOf<UGwEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
			if (EquipDef != nullptr)
			{
				if (UGwEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);
					if (EquippedItem != nullptr)
					{
						EquippedItem->SetInstigator(SlotItem);
					}
				}
			}
		}
	}
}

UGwEquipmentManagerComponent* UGwQuickBarComponent::FindEquipmentManager() const
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			return Pawn->FindComponentByClass<UGwEquipmentManagerComponent>();
		}
	}
	return nullptr;
}

void UGwQuickBarComponent::OnRep_Slots()
{
	FGwQuickBarSlotsChangedMessage Message;
	Message.Owner = GetOwner();
	Message.Slots = Slots;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
	MessageSystem.BroadcastMessage(TAG_Gw_QuickBar_Message_SlotsChanged, Message);
}

void UGwQuickBarComponent::OnRep_SkillSlots()
{
	FGwQuickBarSlotsChangedMessageSkill Message = SkillSlots;
	Message.Owner = GetOwner();

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
	MessageSystem.BroadcastMessage(TAG_Gw_QuickBar_Message_SlotsChangedSkill, Message);
}

void UGwQuickBarComponent::OnRep_ActiveSlotIndex()
{
	FGwQuickBarActiveIndexChangedMessage Message;
	Message.Owner = GetOwner();
	Message.ActiveIndex = ActiveSlotIndex;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
	MessageSystem.BroadcastMessage(TAG_Gw_QuickBar_Message_ActiveIndexChanged, Message);
}

void UGwQuickBarComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, Slots);
	DOREPLIFETIME(ThisClass, SkillSlots);
}

UE_ENABLE_OPTIMIZATION
