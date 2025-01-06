// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "GwQuickBarComponent.generated.h"


class UGwEquipmentManagerComponent;
class UGwEquipmentInstance;
class UGwInventoryItemInstance;

USTRUCT(BlueprintType)
struct FGwQuickBarSlotsChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TArray<TObjectPtr<UGwInventoryItemInstance>> Slots;
};


USTRUCT(BlueprintType)
struct FGwQuickBarActiveIndexChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 ActiveIndex = 0;
};

USTRUCT(BlueprintType)
struct FGwQuickBarSlotsChangedMessageSkill
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TArray<FName> SkillNames;
	
	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TArray<TObjectPtr<UGwInventoryItemInstance>> Slots;
};

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class GWMULTIPLERPG_API UGwQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	UGwQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Gw")
	void SetActiveSlotIndex(int32 NewIndex);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddItemToSlot(int32 SlotIndex, UGwInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddSkillToSlot(FName SkillName, UGwInventoryItemInstance* Item);

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	int32 GetNextFreeItemSlot() const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	TArray<UGwInventoryItemInstance*> GetSlots() const
	{
		return Slots;
	}

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UGwInventoryItemInstance* RemoveItemFromSlot(int32 SlotIndex);

private:
	void UnequipItemInSlot();
	void EquipItemInSlot();

	UGwEquipmentManagerComponent* FindEquipmentManager() const;

protected:
	UPROPERTY()
	int32 NumSlots = 3;

	UFUNCTION()
	void OnRep_Slots();

	UFUNCTION()
	void OnRep_SkillSlots();

	UFUNCTION()
	void OnRep_ActiveSlotIndex();

private:
	UPROPERTY(ReplicatedUsing=OnRep_Slots)
	TArray<TObjectPtr<UGwInventoryItemInstance>> Slots;

	UPROPERTY(ReplicatedUsing=OnRep_SkillSlots)
	FGwQuickBarSlotsChangedMessageSkill SkillSlots;

	UPROPERTY(ReplicatedUsing=OnRep_ActiveSlotIndex)
	int32 ActiveSlotIndex = -1;

	UPROPERTY()
	TObjectPtr<UGwEquipmentInstance> EquippedItem;
};
