// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GwMultipleRPG/System/GameplayTagStack.h"
#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "GwInventoryItemInstance.generated.h"

struct FGameplayTag;
class UGwInventoryItemFragment;
class UGwInventoryItemDefinition;
/**
 * 
 */
UCLASS(BlueprintType)
class GWMULTIPLERPG_API UGwInventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	UGwInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool IsSupportedForNetworking() const override { return true; }

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(DeterminesOutputType=FragmentClass))
	const UGwInventoryItemFragment* FindFragmentByClass(TSubclassOf<UGwInventoryItemFragment> FragmentClass) const;
	
	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}
	
	TSubclassOf<UGwInventoryItemDefinition> GetItemDef() const
	{
		return ItemDef;
	}
	
private:
#if UE_WITH_IRIS
	/** Register all replication fragments */
	virtual void RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context, UE::Net::EFragmentRegistrationFlags RegistrationFlags) override;
#endif // UE_WITH_IRIS
	
	friend struct FGwInventoryList;

	void SetItemDef(TSubclassOf<UGwInventoryItemDefinition> InDef);

	UPROPERTY(Replicated)
	TSubclassOf<UGwInventoryItemDefinition> ItemDef;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;
};
