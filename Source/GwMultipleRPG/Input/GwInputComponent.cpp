// Fill out your copyright notice in the Description page of Project Settings.


#include "GwInputComponent.h"


UGwInputComponent::UGwInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UGwInputComponent::AddInputMappings(const UGwInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);
}

void UGwInputComponent::RemoveInputMappings(const UGwInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

}
