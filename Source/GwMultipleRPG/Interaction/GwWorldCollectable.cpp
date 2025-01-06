// Fill out your copyright notice in the Description page of Project Settings.


#include "GwWorldCollectable.h"


// Sets default values
AGwWorldCollectable::AGwWorldCollectable()
{
}

void AGwWorldCollectable::GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}

FInventoryPickup AGwWorldCollectable::GetPickupInventory() const
{
	return StaticInventory;
}
