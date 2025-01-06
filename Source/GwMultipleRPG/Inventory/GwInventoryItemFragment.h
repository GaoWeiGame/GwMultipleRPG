// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GwInventoryItemFragment.generated.h"

class UGwInventoryItemInstance;
/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class GWMULTIPLERPG_API UGwInventoryItemFragment : public UObject
{
	GENERATED_BODY()
public:
	virtual void OnInstanceCreated(UGwInventoryItemInstance* Instance) const {}
};
