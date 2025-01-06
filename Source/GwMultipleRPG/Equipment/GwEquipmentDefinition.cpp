// Fill out your copyright notice in the Description page of Project Settings.


#include "GwEquipmentDefinition.h"

#include "GwEquipmentInstance.h"

UGwEquipmentDefinition::UGwEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = UGwEquipmentInstance::StaticClass();
}
