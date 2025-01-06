// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableTarget.h"
#include "InteractionOption.h"
#include "InteractionQuery.h"
#include "GameFramework/Actor.h"
#include "GwMultipleRPG/Inventory/Pickupable.h"
#include "GwWorldCollectable.generated.h"

UCLASS(Abstract, Blueprintable)
class GWMULTIPLERPG_API AGwWorldCollectable : public AActor, public IInteractableTarget, public IPickupable
{
	GENERATED_BODY()

public:

	AGwWorldCollectable();

	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder) override;
	virtual FInventoryPickup GetPickupInventory() const override;

protected:
	UPROPERTY(EditAnywhere)
	FInteractionOption Option;

	UPROPERTY(EditAnywhere)
	FInventoryPickup StaticInventory;
};
